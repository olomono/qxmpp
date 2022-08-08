// SPDX-FileCopyrightText: 2019 Jeremy Lainé <jeremy.laine@m4x.org>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#ifndef QXMPPCALLMANAGER_P_H
#define QXMPPCALLMANAGER_P_H

#include "QXmppCall.h"
#include <QXmppJingleIq.h>
#include <QXmppSendResult.h>

#include <QFuture>
#include <QHostAddress>
#include <QList>

class QXmppCallManager;

//  W A R N I N G
//  -------------
//
// This file is not part of the QXmpp API.
// This header file may change from version to version without notice,
// or even be removed.
//
// We mean it.
//

class QXmppCallManagerPrivate
{
public:
    struct Preparing
    {
    };

    // state of a Muji participant
    // A participant can either prepare a session or already sent supported contents.
    using MujiParticipantState = std::variant<Preparing, QVector<QXmppJingleIq::Content>>;

    QXmppCallManagerPrivate(QXmppCallManager *qq);
    QXmppCall *findCall(const QString &sid) const;
    QXmppCall *findCall(const QString &sid, QXmppCall::Direction direction) const;

    QFuture<QXmpp::SendResult> prepareMujiContentAddition(const QString &groupChatJid, const QXmppJingleIq::Content &content);
    QFuture<QXmpp::SendResult> prepareMujiContentRemoval(const QString &groupChatJid, const QString &contentCreator, const QString &contentName);

    template<typename F>
    QFuture<QXmpp::SendResult> prepareGroupCall(const QString &groupChatJid, F preparation);

    QList<QXmppCall *> calls;
    QList<QPair<QHostAddress, quint16>> stunServers;
    QHostAddress turnHost;
    quint16 turnPort;
    QString turnUser;
    QString turnPassword;

    // JIDs of Muji participants mapped to their states
    QHash<QString, MujiParticipantState> mujiParticipantStates;

    // JIDs of Muji sessions mapped to their contents
    QHash<QString, QVector<QXmppJingleIq::Content>> mujiContents;

private:
    QXmppCallManager *q;
};

#endif
