/*
 * Copyright (C) 2008-2021 The QXmpp developers
 *
 * Author:
 *  Linus Jahn
 *
 * Source:
 *  https://github.com/qxmpp-project/qxmpp
 *
 * This file is a part of QXmpp library.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 */

#include "QXmppMixIq.h"

#include "QXmppConstants_p.h"
#include "QXmppDataForm.h"
#include "QXmppUtils.h"

#include <QDomElement>
#include <QSharedData>

static const QStringList MIX_ACTION_TYPES = {
    QString(),
    QStringLiteral("client-join"),
    QStringLiteral("client-leave"),
    QStringLiteral("join"),
    QStringLiteral("leave"),
    QStringLiteral("update-subscription"),
    QStringLiteral("setnick"),
    QStringLiteral("create"),
    QStringLiteral("destroy")
};

class QXmppMixIqPrivate : public QSharedData
{
public:
    QString participantId;
    QString channelId;
    QString channelJid;
    QStringList nodesBeingSubscribed;
    QStringList nodesBeingUnsubscribed;
    QString nick;
    QXmppMixIq::Type actionType = QXmppMixIq::None;
};

QXmppMixIq::QXmppMixIq()
    : QXmppIq(), d(new QXmppMixIqPrivate)
{
}

QXmppMixIq::QXmppMixIq(const QXmppMixIq &other) = default;

QXmppMixIq::~QXmppMixIq() = default;

QXmppMixIq& QXmppMixIq::operator=(const QXmppMixIq &other) = default;

///
/// Returns the participant ID for Join / ClientJoin results.
///
QString QXmppMixIq::participantId() const
{
    return d->participantId;
}

///
/// Sets the participant ID for Join / ClientJoin results.
///
/// @param participantId ID of the user in the channel
///
void QXmppMixIq::setParticipantId(const QString &participantId)
{
    d->participantId = participantId;
}

///
/// Returns the channel's ID (the local part of the channel JID).
/// This may still be empty if a JID was set.
///
QString QXmppMixIq::channelId() const
{
    return d->channelId;
}

///
/// Sets the channel's ID (the local part of the channel JID) for creating or destroying a specific channel.
/// When you create a new channel, this can also be left empty to let the server generate a name.
///
/// @param channelId channel ID to be set
///
void QXmppMixIq::setChannelId(const QString &channelId)
{
    d->channelId = channelId;
}

///
/// Returns the channel's JID.
///
QString QXmppMixIq::channelJid() const
{
    return d->channelJid;
}

///
/// Sets the channel's JID.
///
/// @param channelJid JID to be set
///
void QXmppMixIq::setChannelJid(const QString &channelJid)
{
    d->channelJid = channelJid;
}

///
/// Returns the list of nodes to subscribe to.
///
QStringList QXmppMixIq::nodesBeingSubscribed() const
{
    return d->nodesBeingSubscribed;
}

///
/// Sets the nodes to subscribe to. Note that for UpdateSubscription queries you only need to include the new subscriptions.
///
void QXmppMixIq::setNodesBeingSubscribed(const QStringList &nodes)
{
    d->nodesBeingSubscribed = nodes;
}

///
/// Returns the list of nodes to unsubscribe from.
///
QStringList QXmppMixIq::nodesBeingUnsubscribed() const
{
    return d->nodesBeingUnsubscribed;
}

///
/// Sets the nodes to unsubscribe from.
///
void QXmppMixIq::setNodesBeingUnsubscribed(const QStringList &nodes)
{
    d->nodesBeingUnsubscribed = nodes;
}

///
/// Returns the user's nickname in the channel.
///
QString QXmppMixIq::nick() const
{
    return d->nick;
}

///
/// Sets the nickname for the channel.
///
void QXmppMixIq::setNick(const QString &nick)
{
    d->nick = nick;
}

///
/// Returns the MIX channel action type.
///
QXmppMixIq::Type QXmppMixIq::actionType() const
{
    return d->actionType;
}

///
/// Sets the channel action.
///
void QXmppMixIq::setActionType(QXmppMixIq::Type type)
{
    d->actionType = type;
}

/// \cond
bool QXmppMixIq::isMixIq(const QDomElement &element)
{
    const QDomElement& child = element.firstChildElement();
    return !child.isNull() && (child.namespaceURI() == ns_mix || child.namespaceURI() == ns_mix_pam);
}

void QXmppMixIq::parseElementFromChild(const QDomElement &element)
{
    QDomElement child = element.firstChildElement();
    // determine action type
    d->actionType = (QXmppMixIq::Type) MIX_ACTION_TYPES.indexOf(child.tagName());

    if (child.namespaceURI() == ns_mix_pam) {
        if (child.hasAttribute(QStringLiteral("channel")))
            d->channelJid = child.attribute(QStringLiteral("channel"));

        child = child.firstChildElement();
    }

    if (!child.isNull() && child.namespaceURI() == ns_mix) {
        // TODO: Will those attributes finally be adapted by the XEP?
        if (child.hasAttribute(QStringLiteral("id")))
            d->participantId = child.attribute(QStringLiteral("id"));
        if (child.hasAttribute(QStringLiteral("jid")) && d->actionType != QXmppMixIq::UpdateSubscription)
            d->channelJid = (child.attribute(QStringLiteral("jid"))).split("#").last();
        if (child.hasAttribute(QStringLiteral("channel")))
            d->channelId = child.attribute(QStringLiteral("channel"));

        QDomElement subChild = child.firstChildElement();
        while (!subChild.isNull()) {
            if (subChild.tagName() == QStringLiteral("subscribe"))
                d->nodesBeingSubscribed << subChild.attribute(QStringLiteral("node"));
            else if (subChild.tagName() == QStringLiteral("unsubscribe"))
                d->nodesBeingUnsubscribed << subChild.attribute(QStringLiteral("node"));
            else if (subChild.tagName() == QStringLiteral("nick"))
                d->nick = subChild.text();

            subChild = subChild.nextSiblingElement();
        }
    }
}

void QXmppMixIq::toXmlElementFromChild(QXmlStreamWriter* writer) const
{
    if (d->actionType == None)
        return;

    writer->writeStartElement(MIX_ACTION_TYPES.at(d->actionType));
    if (d->actionType == ClientJoin || d->actionType == ClientLeave) {
        writer->writeDefaultNamespace(ns_mix_pam);
        if (type() == Set)
            helperToXmlAddAttribute(writer, QStringLiteral("channel"), d->channelJid);

        if (d->actionType == ClientJoin)
            writer->writeStartElement(QStringLiteral("join"));
        else if (d->actionType == ClientLeave)
            writer->writeStartElement(QStringLiteral("leave"));
    }

    writer->writeDefaultNamespace(ns_mix);
    helperToXmlAddAttribute(writer, QStringLiteral("channel"), d->channelId);
    if (type() == Result)
        helperToXmlAddAttribute(writer, QStringLiteral("id"), d->participantId);

    for (const auto& node : d->nodesBeingSubscribed) {
        writer->writeStartElement(QStringLiteral("subscribe"));
        writer->writeAttribute(QStringLiteral("node"), node);
        writer->writeEndElement();
    }

    for (const auto& node : d->nodesBeingUnsubscribed) {
        writer->writeStartElement(QStringLiteral("unsubscribe"));
        writer->writeAttribute(QStringLiteral("node"), node);
        writer->writeEndElement();
    }

    if (!d->nick.isEmpty())
        writer->writeTextElement(QStringLiteral("nick"), d->nick);

    writer->writeEndElement();
    if (d->actionType == ClientJoin || d->actionType == ClientLeave)
        writer->writeEndElement();
}
/// \endcond
