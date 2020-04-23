/*
 * Copyright (C) 2008-2021 The QXmpp developers
 *
 * Author:
 *  Linus Jahn <lnj@kaidan.im>
 *  Melvin Keskin <melvo@olomono.de>
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

#ifndef QXMPPMIXMANAGER_H
#define QXMPPMIXMANAGER_H

#include "QXmppClientExtension.h"
#include "QXmppMixIq.h"
#include "QXmppMixItem.h"
#include "QXmppPubSubIq.h"

class QXmppDiscoveryManager;
class QXmppMessage;
class QXmppPubSubManager;

class QXmppMixService
{
public:
    QString jid() const;
    void setJid(const QString &jid);

    bool searchable() const;
    void setSearchable(bool searchable);

    bool canCreateChannel() const;
    void setCanCreateChannel(bool canCreateChannel);

    bool operator==(const QXmppMixService &other) const;

private:
    QString m_jid;
    bool m_searchable = false;
    bool m_canCreateChannel = false;
};

///
/// This class manages group chat communication as specified in the following XEPs:
/// \xep{0369}: Mediated Information eXchange (MIX)
/// \xep{0403}: Mediated Information eXchange (MIX): Presence Support (TODO: implement that)
/// \xep{0404}: Mediated Information eXchange (MIX): JID Hidden Channels (TODO: implement that)
/// \xep{0405}: Mediated Information eXchange (MIX): Participant Server Requirements
/// \xep{0406}: Mediated Information eXchange (MIX): MIX Administration
/// \xep{0407}: Mediated Information eXchange (MIX): Miscellaneous Capabilities
/// \xep{0408}: Mediated Information eXchange (MIX): Co-existence with MUC (TODO: implement that)
///
/// If you want to be informed about results of the methods in this class by the corresponding signals, please make sure to subscribe by the relevant subsciption methods before.
/// That way is chosen to keep the structure simple and not notify two times by one signal for the same result.
///
/// For sending a message to a MIX channel, you have to set the type QXmppMessage::GroupChat.
///
/// Example:
/// \code
/// message->setType(QXmppMessage::GroupChat);
/// client->sendPacket(message);
/// \endcode
///
class QXmppMixManager : public QXmppClientExtension
{
    Q_OBJECT
    Q_PROPERTY(bool isSupportedByServer READ isSupportedByServer NOTIFY isSupportedByServerChanged)
    Q_PROPERTY(bool isArchivingSupportedByServer READ isArchivingSupportedByServer NOTIFY isArchivingSupportedByServerChanged)

public:
    QXmppMixManager();

    QStringList discoveryFeatures() const override;

    QList<QXmppMixService> services() const;

    bool isSupportedByServer() const;
    bool isArchivingSupportedByServer() const;

    void createChannel(const QString &channelOrServiceJid, const bool isPublic = false);
    void joinChannel(const QString &channelJid, const QString &nick = {}, const bool subscribeToMemberJids = true, const bool subscribeToBannedJids = true, const bool subscribeToConfiguration = true, const bool subscribeToInformation = true, const bool subscribeToMessages = true, const bool subscribeToParticipants = true, const bool subscribeToPresences = true);

    void inviteContactToChannel(const QString &channelJid, const QString &contactJid);

    void renameChannel(const QString &channelJid, const QString &name);
    void setNick(const QString &channelJid, const QString &requestedNick);

    void requestChannelAccessibility(const QString &channelJid) const;
    void makeChannelPrivate(const QString &channelJid);
    void makeChannelPublic(const QString &channelJid) const;

    void subscribeToMemberJids(const QString &channelJid);
    void subscribeToBannedJids(const QString &channelJid);
    void subscribeToConfiguration(const QString &channelJid);
    void subscribeToInformation(const QString &channelJid);
    void subscribeToMessages(const QString &channelJid);
    void subscribeToParticipants(const QString &channelJid);
    void subscribeToPresences(const QString &channelJid);

    void requestMemberJids(const QString &channelJid) const;
    void requestBannedJids(const QString &channelJid) const;
    void requestParticipants(const QString &channelJid) const;

    void addMemberJid(const QString &channelJid, const QString &jid) const;
    void removeMemberJid(const QString &channelJid, const QString &jid) const;
    void addBannedJid(const QString &channelJid, const QString &jid);
    void removeBannedJid(const QString &channelJid, const QString &jid) const;

    void deleteChannel(const QString &channelJid);
    void leaveChannel(const QString &channelJid);

    bool handleStanza(const QDomElement &element) override;

signals:
    ///
    /// Emitted when the server enabled or disabled supporting MIX.
    ///
    /// \param isSupportedByServer whether the server supports MIX
    ///
    void isSupportedByServerChanged(bool isSupportedByServer);

    ///
    /// Emitted when the server enabled or disabled supporting archiving for MIX.
    ///
    /// \param isArchivingSupportedByServer whether the server supports archiving for MIX
    ///
    void isArchivingSupportedByServerChanged(bool isArchivingSupportedByServer);

    ///
    /// Emitted when a service providing MIX is discovered.
    ///
    /// \param service service providing MIX
    ///
    void serviceDiscovered(const QXmppMixService &service);

    ///
    /// Emitted when a channel is created.
    ///
    /// \param channelJid JID of the channel which is created
    ///
    void channelCreated(const QString &channelJid);

    ///
    /// Emitted when creating a channel failed.
    ///
    /// \param channelJid JID of the channel which coul not be created
    /// \param error error provided by the server
    ///
    void channelCreationFailed(const QString &channelJid, const QXmppStanza::Error &error);

    ///
    /// Emitted when a channel is joined.
    ///
    /// If not all nodes declared by the join request could be subscribed, only the subset of subscribed nodes is provided.
    ///
    /// \param channelJid JID of the channel which is joined
    /// \param nodes nodes which are subscribed
    ///
    void channelJoined(const QString &channelJid, const QStringList &nodes);

    ///
    /// Emitted when joining a channel failed.
    ///
    /// Besides other issues, that could also be the case if none of the nodes declared in the join request to subscribe could be subscribed.
    ///
    /// \param channelJid JID of the channel which coul not be joined
    /// \param error JID of the channel which coul not be joined
    ///
    void channelJoiningFailed(const QString &channelJid, const QXmppStanza::Error &error);

    ///
    /// Emitted when the user is invited to a channel.
    ///
    /// \param channelJid JID of the channel which the user is invited to
    /// \param inviterJid JID of the inviter
    ///
    void invitedToChannel(const QString &channelJid, const QString &inviterJid);

    ///
    /// Emitted when a contact is invited to a channel.
    ///
    /// \param channelJid JID of the channel which the contact is invited to
    /// \param inviteeJid JID of the invited contact
    ///
    void contactInvitedToChannel(const QString &channelJid, const QString &inviteeJid);

    ///
    /// Emitted when the user's nick is changed.
    ///
    /// \param channelJid JID of the channel for which the user's nick is changed
    /// \param nick user's new nick
    ///
    void nickChanged(const QString &channelJid, const QString &nick);

    ///
    /// Emitted when the information of a channel's accessibility (private / public) is received.
    ///
    /// That happens if a node was added to the corresponding attribute of the channel's configuration node or after requesting the channel's accessibility manually by requestChannelAccessibility().
    ///
    /// \param channelJid JID of the channel having the node
    /// \param isPublic true if the channel is a public one, otherwise false
    ///
    void channelAccessibilityReceived(const QString &channelJid, const bool isPublic);

    ///
    /// Emitted when a channel is made private.
    ///
    /// \param channelJid JID of the channel which is made private
    ///
    void channelMadePrivate(const QString &channelJid);

    ///
    /// Emitted when a channel is made public.
    ///
    /// \param channelJid JID of the channel which is made public
    ///
    void channelMadePublic(const QString &channelJid);

    void subscribedToMemberJids(const QString &channelJid);
    void subscribedToBannedJids(const QString &channelJid);
    void subscribedToConfiguration(const QString &channelJid);
    void subscribedToInformation(const QString &channelJid);
    void subscribedToMessages(const QString &channelJid);
    void subscribedToParticipants(const QString &channelJid);
    void subscribedToPresences(const QString &channelJid);

    void unsubscribedFromMemberJids(const QString &channelJid);
    void unsubscribedFromBannedJids(const QString &channelJid);
    void unsubscribedFromConfiguration(const QString &channelJid);
    void unsubscribedFromInformation(const QString &channelJid);
    void unsubscribedFromMessages(const QString &channelJid);
    void unsubscribedFromParticipants(const QString &channelJid);
    void unsubscribedFromPresences(const QString &channelJid);

    ///
    /// Emitted when a JID allowed to join a channel is received after calling requestMemberJids().
    ///
    /// \param channelJid JID of the channel which might be joined by the passed JID
    /// \param jid JID which is allowed to join the channel if there is no restriction on the banned JIDs
    ///
    void memberJidReceived(const QString &channelJid, const QString &jid);

    ///
    /// Emitted when a JID not allowed to join a channel is received after calling requestBannedJids().
    ///
    /// \param channelJid JID of the channel which cannot be joined by the passed JID
    /// \param jid JID which is not allowed to join the channel
    ///
    void bannedJidReceived(const QString &channelJid, const QString &jid);

    ///
    /// Emitted when a participant is received after calling requestParticipants().
    ///
    /// \param channelJid JID of the channel which is joined by the participant
    /// \param participantItem item for the joined participant
    ///
    void participantReceived(const QString &channelJid, const QXmppMixParticipantItem &participantItem);

    ///
    /// Emitted when a JID allowed to join a channel is added.
    ///
    /// That happens if a JID was added to the member JIDs of the channel after calling addMemberJid() or by another user.
    ///
    /// \param channelJid JID of the channel which might be joined by the passed JID
    /// \param jid JID which is allowed to join the channel now if there is no restriction on the banned JIDs
    ///
    void memberJidAdded(const QString &channelJid, const QString &jid);

    ///
    /// Emitted when a JID allowed to join a channel is removed.
    ///
    /// That happens if a JID was removed from the member JIDs of the channel after calling removeMemberJid() or by another user.
    ///
    /// \param channelJid JID of the channel which can no longer be joined by the passed JID
    /// \param jid JID which is no longer allowed to join the channel
    ///
    void memberJidRemoved(const QString &channelJid, const QString &jid);

    ///
    /// Emitted when a JID not allowed to join a channel is added.
    ///
    /// That happens if a JID was added to the banned JIDs of the channel after calling addBannedJid() or by another user.
    ///
    /// \param channelJid JID of the channel which cannot be joined by the passed JID
    /// \param jid JID which is not allowed to join the channel anymore
    ///
    void bannedJidAdded(const QString &channelJid, const QString &jid);

    ///
    /// Emitted when a JID not allowed to join a channel is removed.
    ///
    /// That happens if a JID was added to the banned JIDs of the channel after calling removeBannedJid() or by another user.
    ///
    /// \param channelJid JID of the channel which might be joined again by the passed JID
    /// \param jid JID which is allowed to join the channel now if there is no restriction on the member JIDs
    ///
    void bannedJidRemoved(const QString &channelJid, const QString &jid);

    ///
    /// Emitted when a user joined a channel or a participant changed the nick.
    ///
    /// \param channelJid JID of the channel which is joined by the user or for which the participant changed the nick
    /// \param participantItem item for the new or modified participant
    ///
    void userJoinedOrParticipantModified(const QString &channelJid, const QXmppMixParticipantItem &participantItem);

    ///
    /// Emitted when a participant left the channel.
    ///
    /// \param channelJid JID of the channel which is left by the participant
    /// \param participantId ID of the left participant
    ///
    void participantLeft(const QString &channelJid, const QString &participantId);

    ///
    /// Emitted when a channel is deleted.
    ///
    /// \param channelJid JID of the deleted channel
    ///
    void channelDeleted(const QString &channelJid);

    ///
    /// Emitted when deleting a channel failed.
    ///
    /// \param channelJid JID of the channel which could not be deleted
    /// \param error error provided by the server
    ///
    void channelDeletionFailed(const QString &channelJid, const QXmppStanza::Error &error);

    ///
    /// Emitted when a channel is left.
    ///
    /// \param channelJid JID of the left channel
    ///
    void channelLeft(const QString &channelJid);

protected:
    void setClient(QXmppClient *client) override;

private slots:
    void handleDiscoInfo(const QXmppDiscoveryIq &iq);
    void handleDiscoItems(const QXmppDiscoveryIq &iq);

    void handlePubSubItems(const QXmppPubSubIq &iq);
    void handleEventNotification(const QXmppMessage &message);

private:
    QString makeChannelPrivateAndReturnStanzaId(const QString &channelJid) const;

    void requestAccessibleNodes(const QString &channelJid) const;

    void subscribeToNodes(const QString &channelJid, const QStringList &nodes);
    void unsubscribeFromNodes(const QString &channelJid, const QStringList &nodes);

//    void removeParticipant(const QString &channelJid, const QString &participantJid);

    void setIsSupportedByServer(bool isSupportedByServer);
    void setIsArchivingSupportedByServer(bool isArchivingSupportedByServer);

    QXmppPubSubManager *m_pubSubManager;
    QXmppDiscoveryManager *m_discoveryManager;
    QList<QXmppMixService> m_services;
    bool m_isSupportedByServer = false;
    bool m_isArchivingSupportedByServer = false;

    // stanza IDs  mapped to channel JIDs
    QMap<QString, QString> m_requestsBeingProcessed;

    // pairs of stanza ID and PubSub IQ query type mapped to channel JIDs
    QMap<QPair<QString, QXmppPubSubIq::QueryType>, QString> m_pubSubChannelRequestsBeingProcessed;

    // pairs of stanza ID and PubSub IQ query type mapped to member or banned JIDs
    QMap<QPair<QString, QXmppPubSubIq::QueryType>, QString> m_pubSubNodeRequestsBeingProcessed;
};

#endif // QXMPPMIXMANAGER_H
