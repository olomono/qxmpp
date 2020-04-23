/*
 * Copyright (C) 2008-2020 The QXmpp developers
 *
 * Author:
 *  Linus Jahn <lnj@kaidan.im>
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

#include "util.h"
#include <QObject>

Q_DECLARE_METATYPE(QXmppIq::Type);
Q_DECLARE_METATYPE(QXmppMixIq::Type);

class tst_QXmppMixIq : public QObject
{
    Q_OBJECT

private slots:
    void testBase_data();
    void testBase();
    void testDefaults();
    void testSetters();
    void testIsMixIq();
};

void tst_QXmppMixIq::testBase_data()
{
    QByteArray joinC2sSetXml(
        "<iq id=\"E6E10350-76CF-40C6-B91B-1EA08C332FC7\" "
        "to=\"hag66@shakespeare.example\" "
        "from=\"hag66@shakespeare.example/UUID-a1j/7533\" "
        "type=\"set\">"
        "<client-join xmlns=\"urn:xmpp:mix:pam:2\" channel=\"coven@mix.shakespeare.example\">"
        "<join xmlns=\"urn:xmpp:mix:core:1\">"
        "<subscribe node=\"urn:xmpp:mix:nodes:messages\"/>"
        "<subscribe node=\"urn:xmpp:mix:nodes:presence\"/>"
        "<subscribe node=\"urn:xmpp:mix:nodes:participants\"/>"
        "<subscribe node=\"urn:xmpp:mix:nodes:info\"/>"
        "<nick>third witch</nick>"
        "</join>"
        "</client-join>"
        "</iq>");
    QByteArray joinS2sSetXml(
        "<iq id=\"E6E10350-76CF-40C6-B91B-1EA08C332FC7\" "
        "to=\"coven@mix.shakespeare.example\" "
        "from=\"hag66@shakespeare.example\" "
        "type=\"set\">"
        "<join xmlns=\"urn:xmpp:mix:core:1\">"
        "<subscribe node=\"urn:xmpp:mix:nodes:messages\"/>"
        "<subscribe node=\"urn:xmpp:mix:nodes:presence\"/>"
        "<subscribe node=\"urn:xmpp:mix:nodes:participants\"/>"
        "<subscribe node=\"urn:xmpp:mix:nodes:info\"/>"
        "<nick>stpeter</nick>"
        "</join>"
        "</iq>");
    QByteArray joinS2sResultXml(
        "<iq id=\"E6E10350-76CF-40C6-B91B-1EA08C332FC7\" "
        "to=\"hag66@shakespeare.example\" "
        "from=\"coven@mix.shakespeare.example\" "
        "type=\"result\">"
        "<join xmlns=\"urn:xmpp:mix:core:1\" id=\"123456\">"
        "<subscribe node=\"urn:xmpp:mix:nodes:messages\"/>"
        "<subscribe node=\"urn:xmpp:mix:nodes:presence\"/>"
        "<subscribe node=\"urn:xmpp:mix:nodes:participants\"/>"
        "<subscribe node=\"urn:xmpp:mix:nodes:info\"/>"
        "<nick>third witch</nick>"
        "</join>"
        "</iq>");
    QByteArray joinC2sResultXml(
        "<iq id=\"E6E10350-76CF-40C6-B91B-1EA08C332FC7\" "
        "to=\"hag66@shakespeare.example/UUID-a1j/7533\" "
        "from=\"hag66@shakespeare.example\" "
        "type=\"result\">"
        "<client-join xmlns=\"urn:xmpp:mix:pam:2\">"
        "<join xmlns=\"urn:xmpp:mix:core:1\" "
        "id=\"123456\">"
        "<subscribe node=\"urn:xmpp:mix:nodes:messages\"/>"
        "<subscribe node=\"urn:xmpp:mix:nodes:presence\"/>"
        "<subscribe node=\"urn:xmpp:mix:nodes:participants\"/>"
        "<subscribe node=\"urn:xmpp:mix:nodes:info\"/>"
        "</join>"
        "</client-join>"
        "</iq>");
    QByteArray leaveC2sSetXml(
        "<iq id=\"E6E10350-76CF-40C6-B91B-1EA08C332FC7\" "
        "to=\"hag66@shakespeare.example\" "
        "from=\"hag66@shakespeare.example/UUID-a1j/7533\" "
        "type=\"set\">"
        "<client-leave xmlns=\"urn:xmpp:mix:pam:2\" channel=\"coven@mix.shakespeare.example\">"
        "<leave xmlns=\"urn:xmpp:mix:core:1\"/>"
        "</client-leave>"
        "</iq>");
    QByteArray leaveS2sSetXml(
        "<iq id=\"E6E10350-76CF-40C6-B91B-1EA08C332FC7\" "
        "to=\"coven@mix.shakespeare.example\" "
        "from=\"hag66@shakespeare.example\" "
        "type=\"set\">"
        "<leave xmlns=\"urn:xmpp:mix:core:1\"/>"
        "</iq>");
    QByteArray leaveS2sResultXml(
        "<iq id=\"E6E10350-76CF-40C6-B91B-1EA08C332FC7\" "
        "to=\"hag66@shakespeare.example\" "
        "from=\"coven@mix.shakespeare.example\" "
        "type=\"result\">"
        "<leave xmlns=\"urn:xmpp:mix:core:1\"/>"
        "</iq>");
    QByteArray leaveC2sResultXml(
        "<iq id=\"E6E10350-76CF-40C6-B91B-1EA08C332FC7\" "
        "to=\"hag66@shakespeare.example/UUID-a1j/7533\" "
        "from=\"hag66@shakespeare.example\" "
        "type=\"result\">"
        "<client-leave xmlns=\"urn:xmpp:mix:pam:2\">"
        "<leave xmlns=\"urn:xmpp:mix:core:1\"/>"
        "</client-leave>"
        "</iq>");
    QByteArray updateSubscriptionSetXml(
        "<iq id=\"E6E10350-76CF-40C6-B91B-1EA08C332FC7\" "
        "to=\"hag66@shakespeare.example\" "
        "from=\"hag66@shakespeare.example/UUID-a1j/7533\" "
        "type=\"set\">"
        "<update-subscription xmlns=\"urn:xmpp:mix:core:1\">"
        "<subscribe node=\"urn:xmpp:mix:nodes:messages\"/>"
        "<unsubscribe node=\"urn:xmpp:mix:nodes:presence\"/>"
        "</update-subscription>"
        "</iq>");
    QByteArray updateSubscriptionResultXml(
        "<iq id=\"E6E10350-76CF-40C6-B91B-1EA08C332FC7\" "
        "to=\"hag66@shakespeare.example/UUID-a1j/7533\" "
        "from=\"hag66@shakespeare.example\" "
        "type=\"result\">"
        "<update-subscription xmlns=\"urn:xmpp:mix:core:1\">"
        "<subscribe node=\"urn:xmpp:mix:nodes:messages\"/>"
        "<unsubscribe node=\"urn:xmpp:mix:nodes:presence\"/>"
        "</update-subscription>"
        "</iq>");
    QByteArray setNickSetXml(
        "<iq id=\"E6E10350-76CF-40C6-B91B-1EA08C332FC7\" "
        "to=\"hag66@shakespeare.example\" "
        "from=\"hag66@shakespeare.example/UUID-a1j/7533\" "
        "type=\"set\">"
        "<setnick xmlns=\"urn:xmpp:mix:core:1\">"
        "<nick>thirdwitch</nick>"
        "</setnick>"
        "</iq>");
    QByteArray setNickResultXml(
        "<iq id=\"E6E10350-76CF-40C6-B91B-1EA08C332FC7\" "
        "to=\"hag66@shakespeare.example/UUID-a1j/7533\" "
        "from=\"hag66@shakespeare.example\" "
        "type=\"result\">"
        "<setnick xmlns=\"urn:xmpp:mix:core:1\">"
        "<nick>thirdwitch</nick>"
        "</setnick>"
        "</iq>");
    QByteArray createXml(
        "<iq id=\"E6E10350-76CF-40C6-B91B-1EA08C332FC7\" "
        "to=\"hag66@shakespeare.example\" "
        "from=\"hag66@shakespeare.example/UUID-a1j/7533\" "
        "type=\"set\">"
        "<create xmlns=\"urn:xmpp:mix:core:1\" channel=\"coven\"/>"
        "</iq>");
    QByteArray createWithoutNameXml(
        "<iq id=\"E6E10350-76CF-40C6-B91B-1EA08C332FC7\" "
        "to=\"hag66@shakespeare.example\" "
        "from=\"hag66@shakespeare.example/UUID-a1j/7533\" "
        "type=\"set\">"
        "<create xmlns=\"urn:xmpp:mix:core:1\"/>"
        "</iq>");
    QByteArray createResultXml(
        "<iq id=\"E6E10350-76CF-40C6-B91B-1EA08C332FC7\" "
        "to=\"hag66@shakespeare.example/UUID-a1j/7533\" "
        "from=\"hag66@shakespeare.example\" "
        "type=\"result\">"
        "<create xmlns=\"urn:xmpp:mix:core:1\" channel=\"coven\"/>"
        "</iq>");
    QByteArray destroyXml(
        "<iq id=\"E6E10350-76CF-40C6-B91B-1EA08C332FC7\" "
        "to=\"hag66@shakespeare.example\" "
        "from=\"hag66@shakespeare.example/UUID-a1j/7533\" "
        "type=\"set\">"
        "<destroy xmlns=\"urn:xmpp:mix:core:1\" channel=\"coven\"/>"
        "</iq>");
    QByteArray emptyXml(
        "<iq id=\"E6E10350-76CF-40C6-B91B-1EA08C332FC7\" "
        "to=\"hag66@shakespeare.example\" "
        "from=\"hag66@shakespeare.example/UUID-a1j/7533\" "
        "type=\"set\"/>");

    QStringList emptyNodes;
    QStringList defaultNodes;
    defaultNodes << "urn:xmpp:mix:nodes:messages"
                 << "urn:xmpp:mix:nodes:presence"
                 << "urn:xmpp:mix:nodes:participants"
                 << "urn:xmpp:mix:nodes:info";

    QTest::addColumn<QByteArray>("xml");
    QTest::addColumn<QXmppIq::Type>("type");
    QTest::addColumn<QXmppMixIq::Type>("actionType");
    QTest::addColumn<QString>("channelJid");
    QTest::addColumn<QString>("channelId");
    QTest::addColumn<QString>("participantId");
    QTest::addColumn<QStringList>("nodesBeingSubscribed");
    QTest::addColumn<QStringList>("nodesBeingUnsubscribed");
    QTest::addColumn<QString>("nick");

    QTest::newRow("join-c2s-set")
        << joinC2sSetXml
        << QXmppIq::Set
        << QXmppMixIq::ClientJoin
        << "coven@mix.shakespeare.example"
        << ""
        << ""
        << defaultNodes
        << emptyNodes
        << "third witch";
    QTest::newRow("join-s2s-set")
        << joinS2sSetXml
        << QXmppIq::Set
        << QXmppMixIq::Join
        << ""
        << ""
        << ""
        << defaultNodes
        << emptyNodes
        << "stpeter";
    QTest::newRow("join-s2s-result")
        << joinS2sResultXml
        << QXmppIq::Result
        << QXmppMixIq::Join
        << ""
        << ""
        << "123456"
        << defaultNodes
        << emptyNodes
        << "third witch";
    QTest::newRow("join-c2s-result")
        << joinC2sResultXml
        << QXmppIq::Result
        << QXmppMixIq::ClientJoin
        << ""
        << ""
        << "123456"
        << defaultNodes
        << emptyNodes
        << "";
    QTest::newRow("leave-c2s-set")
        << leaveC2sSetXml
        << QXmppIq::Set
        << QXmppMixIq::ClientLeave
        << "coven@mix.shakespeare.example"
        << ""
        << ""
        << emptyNodes
        << emptyNodes
        << "";
    QTest::newRow("leave-s2s-set")
        << leaveS2sSetXml
        << QXmppIq::Set
        << QXmppMixIq::Leave
        << ""
        << ""
        << ""
        << emptyNodes
        << emptyNodes
        << "";
    QTest::newRow("leave-s2s-result")
        << leaveS2sResultXml
        << QXmppIq::Result
        << QXmppMixIq::Leave
        << ""
        << ""
        << ""
        << emptyNodes
        << emptyNodes
        << "";
    QTest::newRow("leave-c2s-result")
        << leaveC2sResultXml
        << QXmppIq::Result
        << QXmppMixIq::ClientLeave
        << ""
        << ""
        << ""
        << emptyNodes
        << emptyNodes
        << "";
    QTest::newRow("update-subscription-set")
        << updateSubscriptionSetXml
        << QXmppIq::Set
        << QXmppMixIq::UpdateSubscription
        << ""
        << ""
        << ""
        << (QStringList() << "urn:xmpp:mix:nodes:messages")
        << (QStringList() << "urn:xmpp:mix:nodes:presence")
        << "";
    QTest::newRow("update-subscription-result")
        << updateSubscriptionResultXml
        << QXmppIq::Result
        << QXmppMixIq::UpdateSubscription
        << ""
        << ""
        << ""
        << (QStringList() << "urn:xmpp:mix:nodes:messages")
        << (QStringList() << "urn:xmpp:mix:nodes:presence")
        << "";
    QTest::newRow("setnick-set")
        << setNickSetXml
        << QXmppIq::Set
        << QXmppMixIq::SetNick
        << ""
        << ""
        << ""
        << emptyNodes
        << emptyNodes
        << "thirdwitch";
    QTest::newRow("setnick-result")
        << setNickResultXml
        << QXmppIq::Result
        << QXmppMixIq::SetNick
        << ""
        << ""
        << ""
        << emptyNodes
        << emptyNodes
        << "thirdwitch";
    QTest::newRow("create")
        << createXml
        << QXmppIq::Set
        << QXmppMixIq::Create
        << ""
        << "coven"
        << ""
        << emptyNodes
        << emptyNodes
        << "";
    QTest::newRow("create-without-name")
        << createWithoutNameXml
        << QXmppIq::Set
        << QXmppMixIq::Create
        << ""
        << ""
        << ""
        << emptyNodes
        << emptyNodes
        << "";
    QTest::newRow("create-result")
        << createResultXml
        << QXmppIq::Result
        << QXmppMixIq::Create
        << ""
        << "coven"
        << ""
        << emptyNodes
        << emptyNodes
        << "";
    QTest::newRow("destroy")
        << destroyXml
        << QXmppIq::Set
        << QXmppMixIq::Destroy
        << ""
        << "coven"
        << ""
        << emptyNodes
        << emptyNodes
        << "";
    QTest::newRow("empty")
        << emptyXml
        << QXmppIq::Set
        << QXmppMixIq::None
        << ""
        << ""
        << ""
        << emptyNodes
        << emptyNodes
        << "";
}

void tst_QXmppMixIq::testBase()
{
    QFETCH(QByteArray, xml);
    QFETCH(QXmppIq::Type, type);
    QFETCH(QXmppMixIq::Type, actionType);
    QFETCH(QString, channelJid);
    QFETCH(QString, channelId);
    QFETCH(QStringList, nodesBeingSubscribed);
    QFETCH(QStringList, nodesBeingUnsubscribed);
    QFETCH(QString, nick);

    QXmppMixIq iq;
    parsePacket(iq, xml);
    QCOMPARE(iq.type(), type);
    QCOMPARE(iq.actionType(), actionType);
    QCOMPARE(iq.channelJid(), channelJid);
    QCOMPARE(iq.channelId(), channelId);
    QCOMPARE(iq.nodesBeingSubscribed(), nodesBeingSubscribed);
    QCOMPARE(iq.nodesBeingUnsubscribed(), nodesBeingUnsubscribed);
    QCOMPARE(iq.nick(), nick);
    serializePacket(iq, xml);
}

void tst_QXmppMixIq::testDefaults()
{
    QXmppMixIq iq;
    QCOMPARE(iq.actionType(), QXmppMixIq::None);
    QCOMPARE(iq.channelJid(), QString());
    QCOMPARE(iq.channelId(), QString());
    QCOMPARE(iq.nodesBeingSubscribed(), QStringList());
    QCOMPARE(iq.nodesBeingUnsubscribed(), QStringList());
    QCOMPARE(iq.nick(), QString());
}

void tst_QXmppMixIq::testSetters()
{
    QXmppMixIq iq;
    iq.setActionType(QXmppMixIq::Join);
    QCOMPARE(iq.actionType(), QXmppMixIq::Join);
    iq.setParticipantId("123456");
    QCOMPARE(iq.participantId(), QString("123456"));
    iq.setChannelId("interestingnews");
    QCOMPARE(iq.channelId(), QString("interestingnews"));
    iq.setNodesBeingSubscribed(QStringList() << "com:example:mix:node:custom");
    QCOMPARE(iq.nodesBeingSubscribed(), QStringList() << "com:example:mix:node:custom");
    iq.setNodesBeingUnsubscribed(QStringList() << "com:example:mix:node:custom");
    QCOMPARE(iq.nodesBeingUnsubscribed(), QStringList() << "com:example:mix:node:custom");
    iq.setNick("SMUDO");
    QCOMPARE(iq.nick(), QString("SMUDO"));
}

void tst_QXmppMixIq::testIsMixIq()
{
    const QByteArray trueXml(
        "<iq id=\"E6E10350-76CF-40C6-B91B-1EA08C332FC7\" "
        "to=\"hag66@shakespeare.example\" "
        "from=\"hag66@shakespeare.example/UUID-a1j/7533\" "
        "type=\"set\">"
        "<destroy xmlns=\"urn:xmpp:mix:core:1\" channel=\"coven\"/>"
        "</iq>");
    const QByteArray truePamXml(
        "<iq id=\"E6E10350-76CF-40C6-B91B-1EA08C332FC7\" "
        "to=\"hag66@shakespeare.example\" "
        "from=\"hag66@shakespeare.example/UUID-a1j/7533\" "
        "type=\"set\">"
        "<client-leave xmlns=\"urn:xmpp:mix:pam:2\" channel=\"coven@mix.shakespeare.example\">"
        "<leave xmlns=\"urn:xmpp:mix:core:1\"/>"
        "</client-leave>"
        "</iq>");
    const QByteArray falseXml(
        "<iq id=\"E6E10350-76CF-40C6-B91B-1EA08C332FC7\" "
        "to=\"hag66@shakespeare.example\" "
        "from=\"hag66@shakespeare.example/UUID-a1j/7533\" "
        "type=\"set\">"
        "<destroy xmlns=\"something:else\" channel=\"coven\"/>"
        "</iq>");

    QDomDocument doc;
    doc.setContent(trueXml, true);
    QDomElement trueElement = doc.documentElement();
    QVERIFY(QXmppMixIq::isMixIq(trueElement));

    doc.setContent(truePamXml, true);
    QDomElement truePamElement = doc.documentElement();
    QVERIFY(QXmppMixIq::isMixIq(truePamElement));

    doc.setContent(falseXml, true);
    QDomElement falseElement = doc.documentElement();
    QVERIFY(!QXmppMixIq::isMixIq(falseElement));
}

QTEST_MAIN(tst_QXmppMixIq)
#include "tst_qxmppmixiq.moc"
