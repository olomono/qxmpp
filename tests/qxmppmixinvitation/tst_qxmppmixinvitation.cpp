/*
 * Copyright (C) 2008-2020 The QXmpp developers
 *
 * Author:
 *  Melvin Keskin
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

#include "QXmppMixInvitation.h"

#include "util.h"
#include <QDomDocument>
#include <QObject>

class tst_QXmppMixInvitation : public QObject
{
    Q_OBJECT

private slots:
    void testInvitation();
    void testIsInvitation();
};

void tst_QXmppMixInvitation::testInvitation()
{
    const QByteArray xml(
        "<invitation xmlns=\"urn:xmpp:mix:misc:0\">"
            "<inviter>hag66@shakespeare.example</inviter>"
            "<invitee>cat@shakespeare.example</invitee>"
            "<channel>coven@mix.shakespeare.example</channel>"
            "<token>ABCDEF</token>"
        "</invitation>"
    );

    QXmppMixInvitation invitation;
    parsePacket(invitation, xml);

    QCOMPARE(invitation.inviterJid(), "hag66@shakespeare.example");
    QCOMPARE(invitation.inviteeJid(), "cat@shakespeare.example");
    QCOMPARE(invitation.channelJid(), "coven@mix.shakespeare.example");
    QCOMPARE(invitation.token(), "ABCDEF");

    QXmppElement element = invitation.toElement();
    serializePacket(element, xml);

    invitation.setInviterJid("hag66@shakespeare.example");
    invitation.setInviteeJid("cat@shakespeare.example");
    invitation.setChannelJid("coven@mix.shakespeare.example");
    invitation.setToken("ABCDEF");

    QCOMPARE(invitation.inviterJid(), "hag66@shakespeare.example");
    QCOMPARE(invitation.inviteeJid(), "cat@shakespeare.example");
    QCOMPARE(invitation.channelJid(), "coven@mix.shakespeare.example");
    QCOMPARE(invitation.token(), "ABCDEF");
}

void tst_QXmppMixInvitation::testIsInvitation()
{
    QDomDocument doc;
    QDomElement element;

    const QByteArray correctInvitationWithNamespace(
        "<invitation xmlns=\"urn:xmpp:mix:misc:0\">"
            "<inviter>hag66@shakespeare.example</inviter>"
            "<invitee>cat@shakespeare.example</invitee>"
            "<channel>coven@mix.shakespeare.example</channel>"
            "<token>ABCDEF</token>"
        "</invitation>"
    );
    QCOMPARE(doc.setContent(correctInvitationWithNamespace, true), true);
    element = doc.documentElement();
    QVERIFY(QXmppMixInvitation::isMixInvitation(element));

    const QByteArray correctInvitationWithoutNamespace(
        "<invitation>"
            "<inviter>hag66@shakespeare.example</inviter>"
            "<invitee>cat@shakespeare.example</invitee>"
            "<channel>coven@mix.shakespeare.example</channel>"
            "<token>ABCDEF</token>"
        "</invitation>"
    );
    QCOMPARE(doc.setContent(correctInvitationWithoutNamespace, true), true);
    element = doc.documentElement();
    QVERIFY(QXmppMixInvitation::isMixInvitation(element));

    const QByteArray incorrectInvitation(
        "<invitation xmlns=\"urn:xmpp:example\">"
            "<inviter>hag66@shakespeare.example</inviter>"
            "<invitee>cat@shakespeare.example</invitee>"
            "<channel>coven@mix.shakespeare.example</channel>"
            "<token>ABCDEF</token>"
        "</invitation>"
    );
    QCOMPARE(doc.setContent(incorrectInvitation, true), true);
    element = doc.documentElement();
    QVERIFY(!QXmppMixInvitation::isMixInvitation(element));
}

QTEST_MAIN(tst_QXmppMixInvitation)
#include "tst_qxmppmixinvitation.moc"
