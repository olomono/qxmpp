// SPDX-FileCopyrightText: 2010 Jeremy Lainé <jeremy.laine@m4x.org>
// SPDX-FileCopyrightText: 2022 Melvin Keskin <melvo@olomono.de>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "QXmppCallManager.h"

#include "QXmppCall.h"
#include "QXmppCallManager_p.h"
#include "QXmppCall_p.h"
#include "QXmppClient.h"
#include "QXmppConstants_p.h"
#include "QXmppFutureUtils_p.h"
#include "QXmppJingleIq.h"
#include "QXmppStun.h"
#include "QXmppUtils.h"

#include <gst/gst.h>

#include <QDomElement>
#include <QFuture>
#include <QTimer>

using namespace QXmpp::Private;

/// \cond
QXmppCallManagerPrivate::QXmppCallManagerPrivate(QXmppCallManager *qq)
    : turnPort(0),
      q(qq)
{
    // Initialize GStreamer
    gst_init(nullptr, nullptr);
}

QXmppCall *QXmppCallManagerPrivate::findCall(const QString &sid) const
{
    for (auto *call : calls)
        if (call->sid() == sid)
            return call;
    return nullptr;
}

QXmppCall *QXmppCallManagerPrivate::findCall(const QString &sid, QXmppCall::Direction direction) const
{
    for (auto *call : calls)
        if (call->sid() == sid && call->direction() == direction)
            return call;
    return nullptr;
}

///
/// Prepares the addition of a Muji content.
///
/// \param groupChatJid JID of the call's group chat
/// \param content content to be added
///
/// \return the result of the preparation
///
/// \since QXmpp 1.6
///
QFuture<QXmpp::SendResult> QXmppCallManagerPrivate::prepareMujiContentAddition(const QString &groupChatJid, const QXmppJingleIq::Content &content)
{
    return prepareGroupCall(groupChatJid, [=](QXmppPresence &presence) {
        auto &groupChatMujiContents = mujiContents[groupChatJid];
        groupChatMujiContents.append(content);
        presence.setMujiContents(groupChatMujiContents);
    });
}

///
/// Prepares the removal of a Muji content.
///
/// \param groupChatJid JID of the call's group chat
/// \param contentCreator creator of the content
/// \param contentName name of the content
///
/// \return the result of the preparation
///
/// \since QXmpp 1.6
///
QFuture<QXmpp::SendResult> QXmppCallManagerPrivate::prepareMujiContentRemoval(const QString &groupChatJid, const QString &contentCreator, const QString &contentName)
{
    QXmppPresence presence;
    presence.setTo(groupChatJid);

    auto &groupChatMujiContents = mujiContents[groupChatJid];

    for (auto itr = groupChatMujiContents.begin(); itr != groupChatMujiContents.end();) {
        if (itr->creator() == contentCreator && itr->name() == contentName) {
            groupChatMujiContents.erase(itr);
            break;
        } else {
            ++itr;
        }
    }

    presence.setMujiContents(groupChatMujiContents);

    return q->client()->send(std::move(presence));
}

///
/// Prepares a group call.
///
/// A preparing presence stanza is sent first.
/// As soon as all Muji participants are ready, the second presence stanza manipulated by the
/// preparation function is sent.
///
/// \param groupChatJid JID of the call's group chat
/// \param preparation function doing preparations and modifying the second presence stanza
///
/// \return the result of the preparation
///
/// \since QXmpp 1.6
///
template<typename F>
QFuture<QXmpp::SendResult> QXmppCallManagerPrivate::prepareGroupCall(const QString &groupChatJid, F preparation)
{
    QFutureInterface<QXmpp::SendResult> interface(QFutureInterfaceBase::Started);

    QXmppPresence presence;
    presence.setTo(groupChatJid);
    presence.setIsPreparingMujiSession(true);

    auto future = q->client()->send(std::move(presence));
    await(future, q, [=](QXmpp::SendResult result) mutable {
        if (const auto *error = std::get_if<QXmpp::SendError>(&result)) {
            reportFinishedResult(interface, { *error });
        } else {
            q->connect(q, &QXmppCallManager::groupCallParticipantsPrepared, [=](const QString &groupChatJid) mutable {
                // TODO: How can only this slot be disconnected (in case of multiple connections at the same time)?
                q->disconnect(q, &QXmppCallManager::groupCallParticipantsPrepared, nullptr, nullptr);
                QXmppPresence presence;
                presence.setTo(groupChatJid);

                preparation(presence);

                auto future = q->client()->send(std::move(presence));
                await(future, q, [=](QXmpp::SendResult result) mutable {
                    reportFinishedResult(interface, result);
                });
            });
        }
    });

    return interface.future();
}
/// \endcond

///
/// Constructs a QXmppCallManager object to handle incoming and outgoing
/// Voice-Over-IP calls.
///
QXmppCallManager::QXmppCallManager()
{
    d = new QXmppCallManagerPrivate(this);
}

///
/// Destroys the QXmppCallManager object.
///
QXmppCallManager::~QXmppCallManager()
{
    delete d;
}

/// \cond
QStringList QXmppCallManager::discoveryFeatures() const
{
    return {
        ns_jingle,      // XEP-0166 : Jingle
        ns_jingle_rtp,  // XEP-0167 : Jingle RTP Sessions
        ns_jingle_rtp_audio,
        ns_jingle_rtp_video,
        ns_jingle_ice_udp,  // XEP-0176 : Jingle ICE-UDP Transport Method
    };
}

bool QXmppCallManager::handleStanza(const QDomElement &element)
{
    if (element.tagName() == "iq") {
        // XEP-0166: Jingle
        if (QXmppJingleIq::isJingleIq(element)) {
            QXmppJingleIq jingleIq;
            jingleIq.parse(element);
            _q_jingleIqReceived(jingleIq);
            return true;
        }
    }

    return false;
}

void QXmppCallManager::setClient(QXmppClient *client)
{
    QXmppClientExtension::setClient(client);

    connect(client, &QXmppClient::disconnected,
            this, &QXmppCallManager::_q_disconnected);

    connect(client, &QXmppClient::iqReceived,
            this, &QXmppCallManager::_q_iqReceived);

    connect(client, &QXmppClient::presenceReceived,
            this, &QXmppCallManager::_q_presenceReceived);
}
/// \endcond

///
/// \fn QXmppCallManager::groupCallParticipantsPrepared(const QString &groupChatJid)
///
/// Emitted when all formerly preparing group call participants are ready.
///
/// \param groupChatJid JID of the call's group chat
///
/// \since QXmpp 1.6
///

///
/// Prepares the start of a group call.
///
/// \param groupChatJid JID of the call's group chat
///
/// \return the result of the preparation
///
/// \since QXmpp 1.6
///
QFuture<QXmpp::SendResult> QXmppCallManager::prepareGroupCallStart(const QString &groupChatJid)
{
    return d->prepareGroupCall(groupChatJid, [=](QXmppPresence &presence) {
        QVector<QXmppJingleIq::Content> mujiContents;
        QXmppCall call(groupChatJid, QXmppCall::OutgoingDirection, this);

        // TODO: Is this the right way to get the supported contents?
        for (const auto &media : { "audio", "video" }) {
            auto *stream = call.d->createStream(media, "initiator", "microphone");
            Q_ASSERT(stream);
            mujiContents.append(call.d->localContent(stream));
        }

        d->mujiContents.insert(groupChatJid, mujiContents);
        presence.setMujiContents(mujiContents);
    });
}

///
/// Prepares the end of a group call.
///
/// \param groupChatJid JID of the call's group chat
///
/// \return the result of the preparation
///
/// \since QXmpp 1.6
///
QFuture<QXmpp::SendResult> QXmppCallManager::prepareGroupCallEnd(const QString &groupChatJid)
{
    d->mujiContents.remove(groupChatJid);

    QXmppPresence presence;
    presence.setTo(groupChatJid);
    presence.setMujiContents(d->mujiContents.value(groupChatJid));

    return client()->send(std::move(presence));
}

///
/// Initiates a new outgoing call to the specified recipient.
///
/// \param jid
///
QXmppCall *QXmppCallManager::call(const QString &jid)
{
    if (jid.isEmpty()) {
        warning("Refusing to call an empty jid");
        return nullptr;
    }

    if (jid == client()->configuration().jid()) {
        warning("Refusing to call self");
        return nullptr;
    }

    QXmppCall *call = new QXmppCall(jid, QXmppCall::OutgoingDirection, this);
    QXmppCallStream *stream = call->d->createStream("audio", "initiator", "microphone");
    call->d->streams << stream;
    call->d->sid = QXmppUtils::generateStanzaHash();

    // register call
    d->calls << call;
    connect(call, &QObject::destroyed,
            this, &QXmppCallManager::_q_callDestroyed);
    emit callStarted(call);

    call->d->sendInvite();

    return call;
}

///
/// Sets multiple STUN servers to use to determine server-reflexive addresses
/// and ports.
///
/// \note This may only be called prior to calling bind().
///
/// \param servers List of the STUN servers.
///
/// \since QXmpp 1.3
///
void QXmppCallManager::setStunServers(const QList<QPair<QHostAddress, quint16>> &servers)
{
    d->stunServers = servers;
}

///
/// Sets a single STUN server to use to determine server-reflexive addresses
/// and ports.
///
/// \note This may only be called prior to calling bind().
///
/// \param host The address of the STUN server.
/// \param port The port of the STUN server.
///
void QXmppCallManager::setStunServer(const QHostAddress &host, quint16 port)
{
    d->stunServers.clear();
    d->stunServers.push_back(qMakePair(host, port));
}

///
/// Sets the TURN server to use to relay packets in double-NAT configurations.
///
/// \param host The address of the TURN server.
/// \param port The port of the TURN server.
///
void QXmppCallManager::setTurnServer(const QHostAddress &host, quint16 port)
{
    d->turnHost = host;
    d->turnPort = port;
}

///
/// Sets the \a user used for authentication with the TURN server.
///
/// \param user
///
void QXmppCallManager::setTurnUser(const QString &user)
{
    d->turnUser = user;
}

///
/// Sets the \a password used for authentication with the TURN server.
///
/// \param password
///
void QXmppCallManager::setTurnPassword(const QString &password)
{
    d->turnPassword = password;
}

///
/// Handles call destruction.
///
void QXmppCallManager::_q_callDestroyed(QObject *object)
{
    d->calls.removeAll(static_cast<QXmppCall *>(object));
}

///
/// Handles disconnection from server.
///
void QXmppCallManager::_q_disconnected()
{
    for (auto *call : std::as_const(d->calls)) {
        call->d->terminate(QXmppJingleIq::Reason::Gone);
    }
}

///
/// Handles acknowledgements.
///
void QXmppCallManager::_q_iqReceived(const QXmppIq &ack)
{
    if (ack.type() != QXmppIq::Result)
        return;

    // find request
    for (auto *call : std::as_const(d->calls)) {
        call->d->handleAck(ack);
    }
}

///
/// Handles a Jingle IQ.
///
void QXmppCallManager::_q_jingleIqReceived(const QXmppJingleIq &iq)
{

    if (iq.type() != QXmppIq::Set)
        return;

    if (iq.action() == QXmppJingleIq::SessionInitiate) {
        // build call
        QXmppCall *call = new QXmppCall(iq.from(), QXmppCall::IncomingDirection, this);
        call->d->sid = iq.sid();

        const auto content = iq.contents().isEmpty() ? QXmppJingleIq::Content()
                                                     : iq.contents().constFirst();
        auto *stream = call->d->createStream(content.descriptionMedia(), content.creator(), content.name());
        if (!stream)
            return;
        call->d->streams << stream;

        // send ack
        call->d->sendAck(iq);

        // check content description and transport
        if (!call->d->handleDescription(stream, content) ||
            !call->d->handleTransport(stream, content)) {

            // terminate call
            call->d->terminate(QXmppJingleIq::Reason::FailedApplication);
            call->terminated();
            delete call;
            return;
        }

        // register call
        d->calls << call;
        connect(call, &QObject::destroyed,
                this, &QXmppCallManager::_q_callDestroyed);

        // send ringing indication
        QXmppJingleIq ringing;
        ringing.setTo(call->jid());
        ringing.setType(QXmppIq::Set);
        ringing.setAction(QXmppJingleIq::SessionInfo);
        ringing.setSid(call->sid());
        ringing.setRinging(true);
        call->d->sendRequest(ringing);

        // notify user
        emit callReceived(call);
        return;

    } else {

        // for all other requests, require a valid call
        QXmppCall *call = d->findCall(iq.sid());
        if (!call) {
            warning(QString("Remote party %1 sent a request for an unknown call %2").arg(iq.from(), iq.sid()));
            return;
        }
        call->d->handleRequest(iq);
    }
}

///
/// Handles a presence.
///
void QXmppCallManager::_q_presenceReceived(const QXmppPresence &presence)
{
    const auto determineParticipantsPrepared = [=]() {
        const auto allParticipantsPrepared = std::all_of(d->mujiParticipantStates.cbegin(), d->mujiParticipantStates.cend(), [=](const QXmppCallManagerPrivate::MujiParticipantState &participantState) {
            if (std::holds_alternative<QVector<QXmppJingleIq::Content>>(participantState)) {
                return true;
            }
            return false;
        });

        if (allParticipantsPrepared) {
            emit groupCallParticipantsPrepared(QXmppUtils::jidToBareJid(presence.from()));
        }
    };

    if (presence.from() == client()->configuration().jid()) {
        determineParticipantsPrepared();
    } else if (presence.isPreparingMujiSession()) {
        d->mujiParticipantStates.insert(presence.from(), QXmppCallManagerPrivate::Preparing());
    } else if (const auto contents = presence.mujiContents(); !contents.isEmpty()) {
        d->mujiParticipantStates.insert(presence.from(), contents);
        determineParticipantsPrepared();
    }

    if (presence.type() == QXmppPresence::Unavailable) {
        for (auto *call : std::as_const(d->calls)) {
            if (presence.from() == call->jid()) {
                // the remote party has gone away, terminate call
                call->d->terminate(QXmppJingleIq::Reason::Gone);
            }
        }
    }
}
