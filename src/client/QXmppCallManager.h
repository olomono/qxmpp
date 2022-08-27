// SPDX-FileCopyrightText: 2010 Jeremy Lainé <jeremy.laine@m4x.org>
// SPDX-FileCopyrightText: 2022 Melvin Keskin <melvo@olomono.de>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#ifndef QXMPPCALLMANAGER_H
#define QXMPPCALLMANAGER_H

#include "QXmppCall.h"
#include "QXmppClientExtension.h"
#include "QXmppLogger.h"

#include <QFuture>
#include <QIODevice>
#include <QMetaType>
#include <QObject>
#include <QXmppJingleIq.h>
#include <QXmppSendResult.h>

class QHostAddress;
class QXmppCallManagerPrivate;
class QXmppIq;
class QXmppJingleCandidate;
class QXmppJingleIq;
class QXmppJinglePayloadType;
class QXmppPresence;

/// \brief The QXmppCallManager class provides support for making and
/// receiving voice calls.
///
/// Session initiation is performed as described by \xep{0166}: Jingle,
/// \xep{0167}: Jingle RTP Sessions and \xep{0176}: Jingle ICE-UDP Transport
/// Method.
///
/// The data stream is connected using Interactive Connectivity Establishment
/// (RFC 5245) and data is transferred using Real Time Protocol (RFC 3550)
/// packets.
///
/// To make use of this manager, you need to instantiate it and load it into
/// the QXmppClient instance as follows:
///
/// \code
/// QXmppCallManager *manager = new QXmppCallManager;
/// client->addExtension(manager);
/// \endcode
///
/// \ingroup Managers

class QXMPP_EXPORT QXmppCallManager : public QXmppClientExtension
{
    Q_OBJECT

public:
    QXmppCallManager();
    ~QXmppCallManager() override;
    void setStunServers(const QList<QPair<QHostAddress, quint16>> &servers);
    void setStunServer(const QHostAddress &host, quint16 port = 3478);
    void setTurnServer(const QHostAddress &host, quint16 port = 3478);
    void setTurnUser(const QString &user);
    void setTurnPassword(const QString &password);

    QFuture<QXmpp::SendResult> prepareGroupCallStart(const QString &groupChatJid);
    QFuture<QXmpp::SendResult> prepareGroupCallEnd(const QString &groupChatJid);

    /// \cond
    QStringList discoveryFeatures() const override;
    bool handleStanza(const QDomElement &element) override;
    /// \endcond

Q_SIGNALS:
    /// This signal is emitted when a new incoming call is received.
    ///
    /// To accept the call, invoke the call's QXmppCall::accept() method.
    /// To refuse the call, invoke the call's QXmppCall::hangup() method.
    void callReceived(QXmppCall *call);

    /// This signal is emitted when a call (incoming or outgoing) is started.
    void callStarted(QXmppCall *call);

    void groupCallParticipantsPrepared(const QString &groupChatJid);

public Q_SLOTS:
    QXmppCall *call(const QString &jid);

protected:
    /// \cond
    void setClient(QXmppClient *client) override;
    /// \endcond

private Q_SLOTS:
    void _q_callDestroyed(QObject *object);
    void _q_disconnected();
    void _q_iqReceived(const QXmppIq &iq);
    void _q_jingleIqReceived(const QXmppJingleIq &iq);
    void _q_presenceReceived(const QXmppPresence &presence);

private:
    QXmppCallManagerPrivate *d;
    friend class QXmppCall;
    friend class QXmppCallPrivate;
    friend class QXmppCallManagerPrivate;
};

#endif
