/*
 * Copyright (C) 2008-2019 The QXmpp developers
 *
 * Authors:
 *  Germán Márquez Mejía
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

#include <QObject>
#include "QXmppPubSubIq.h"
#include "QXmppMessage.h"
#include "QXmppPubSubManager.h"
#include "util.h"

class QXmppPubSubTestHelper  : public QObject
{
    Q_OBJECT

public slots:
    void iqSent(const QXmppPubSubIq& iq);
    void iqReceived(const QXmppPubSubIq& iq);
    void messageSent(const QXmppMessage& msg);
    void messageReceived(const QXmppMessage& msg);

public:
    QXmppPubSubIq m_expectedIq;
    QXmppMessage m_expectedMessage;
    bool m_expectSent;
    bool m_signalTriggered;

    void compareIqs(const QXmppPubSubIq& lhs, const QXmppPubSubIq& rhs);
    void compareMessages(const QXmppMessage& lhs, const QXmppMessage& rhs);
};

class tst_QXmppPubSubManager : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();

    void testHandleStanza_data();
    void testMessageReceived_data();
    void testHandleStanza();

private:
    QXmppPubSubTestHelper m_helper;
    QXmppPubSubManager m_manager;
};

void tst_QXmppPubSubManager::initTestCase()
{
    connect(&m_manager, &QXmppPubSubManager::itemsReceived,
            &m_helper, &QXmppPubSubTestHelper::iqReceived);

    connect(&m_manager, &QXmppPubSubManager::eventNotificationReceived,
            &m_helper, &QXmppPubSubTestHelper::messageReceived);
}

void tst_QXmppPubSubManager::testHandleStanza_data()
{
    QTest::addColumn<QByteArray>("xml");
    QTest::addColumn<bool>("accept");
    QTest::addColumn<bool>("sent");

    QTest::newRow("received1")
        << QByteArray(
               "<iq type='result'"
               "    from='pubsub.shakespeare.lit'"
               "    to='francisco@denmark.lit/barracks'"
               "    id='items1'>"
               "  <pubsub xmlns='http://jabber.org/protocol/pubsub'>"
               "    <items node='princely_musings'>"
               "      <item id='368866411b877c30064a5f62b917cffe'>"
               "        <entry xmlns='http://www.w3.org/2005/Atom'>"
               "          <title>The Uses of This World</title>"
               "          <summary>"
               "O, that this too too solid flesh would melt"
               "Thaw and resolve itself into a dew!"
               "          </summary>"
               "          <link rel='alternate' type='text/html'"
               "                href='http://denmark.lit/2003/12/13/atom03'/>"
               "          <id>tag:denmark.lit,2003:entry-32396</id>"
               "          <published>2003-12-12T17:47:23Z</published>"
               "          <updated>2003-12-12T17:47:23Z</updated>"
               "        </entry>"
               "      </item>"
               "      <item id='3300659945416e274474e469a1f0154c'>"
               "        <entry xmlns='http://www.w3.org/2005/Atom'>"
               "          <title>Ghostly Encounters</title>"
               "          <summary>"
               "O all you host of heaven! O earth! what else?"
               "And shall I couple hell? O, fie! Hold, hold, my heart;"
               "And you, my sinews, grow not instant old,"
               "But bear me stiffly up. Remember thee!"
               "          </summary>"
               "          <link rel='alternate' type='text/html'"
               "                href='http://denmark.lit/2003/12/13/atom03'/>"
               "          <id>tag:denmark.lit,2003:entry-32396</id>"
               "          <published>2003-12-12T23:21:34Z</published>"
               "          <updated>2003-12-12T23:21:34Z</updated>"
               "        </entry>"
               "      </item>"
               "      <item id='4e30f35051b7b8b42abe083742187228'>"
               "        <entry xmlns='http://www.w3.org/2005/Atom'>"
               "          <title>Alone</title>"
               "          <summary>"
               "Now I am alone."
               "O, what a rogue and peasant slave am I!"
               "          </summary>"
               "          <link rel='alternate' type='text/html'"
               "                href='http://denmark.lit/2003/12/13/atom03'/>"
               "          <id>tag:denmark.lit,2003:entry-32396</id>"
               "          <published>2003-12-13T11:09:53Z</published>"
               "          <updated>2003-12-13T11:09:53Z</updated>"
               "        </entry>"
               "      </item>"
               "      <item id='ae890ac52d0df67ed7cfdf51b644e901'>"
               "        <entry xmlns='http://www.w3.org/2005/Atom'>"
               "          <title>Soliloquy</title>"
               "          <summary>"
               "To be, or not to be: that is the question:"
               "Whether 'tis nobler in the mind to suffer"
               "The slings and arrows of outrageous fortune,"
               "Or to take arms against a sea of troubles,"
               "And by opposing end them?"
               "          </summary>"
               "          <link rel='alternate' type='text/html'"
               "                href='http://denmark.lit/2003/12/13/atom03'/>"
               "          <id>tag:denmark.lit,2003:entry-32397</id>"
               "          <published>2003-12-13T18:30:02Z</published>"
               "          <updated>2003-12-13T18:30:02Z</updated>"
               "        </entry>"
               "      </item>"
               "    </items>"
               "  </pubsub>"
               "</iq>"
               )
        << true << false;
}

void tst_QXmppPubSubManager::testMessageReceived_data()
{
    QTest::addColumn<QByteArray>("xml");
    QTest::addColumn<bool>("accept");
    QTest::addColumn<bool>("sent");

    QTest::newRow("received1")
        << QByteArray(
           "<message from='pubsub.shakespeare.lit' to='francisco@denmark.lit' id='foo'>"
              "<event xmlns='http://jabber.org/protocol/pubsub#event'>"
                "<items node='princely_musings'>"
                  "<item id='ae890ac52d0df67ed7cfdf51b644e901'>"
                    "<entry xmlns='http://www.w3.org/2005/Atom'>"
                      "<title>Soliloquy</title>"
                      "<summary>"
            "To be, or not to be: that is the question:"
            "Whether 'tis nobler in the mind to suffer"
            "The slings and arrows of outrageous fortune,"
            "Or to take arms against a sea of troubles,"
            "And by opposing end them?"
                      "</summary>"
                      "<link rel='alternate' type='text/html'"
                            "href='http://denmark.lit/2003/12/13/atom03'/>"
                      "<id>tag:denmark.lit,2003:entry-32397</id>"
                      "<published>2003-12-13T18:30:02Z</published>"
                      "<updated>2003-12-13T18:30:02Z</updated>"
                    "</entry>"
                  "</item>"
                "</items>"
              "</event>"
            "</message>"
        )
        << true << false;
}

void tst_QXmppPubSubManager::testHandleStanza()
{
    QFETCH(QByteArray, xml);
    QFETCH(bool, accept);
    QFETCH(bool, sent);

    m_helper.m_expectedIq = QXmppPubSubIq();
    parsePacket(m_helper.m_expectedIq, xml);

    m_helper.m_expectSent = sent;
    m_helper.m_signalTriggered = false;

    QDomDocument doc;
    QCOMPARE(doc.setContent(xml, true), true);
    QDomElement element = doc.documentElement();

    bool accepted = m_manager.handleStanza(element);

    QCOMPARE(accepted, accept);
    QCOMPARE(m_helper.m_signalTriggered, accept);
}

void QXmppPubSubTestHelper::iqSent(const QXmppPubSubIq &iq)
{
    m_signalTriggered = true;
    QCOMPARE(m_expectSent, true);

    compareIqs(m_expectedIq, iq);
}

void QXmppPubSubTestHelper::iqReceived(const QXmppPubSubIq &iq)
{
    m_signalTriggered = true;
    QCOMPARE(m_expectSent, false);

    compareIqs(m_expectedIq, iq);
}

void QXmppPubSubTestHelper::messageSent(const QXmppMessage &msg)
{
    m_signalTriggered = true;
    QCOMPARE(m_expectSent, true);

    compareMessages(m_expectedMessage, msg);
}

void QXmppPubSubTestHelper::messageReceived(const QXmppMessage &msg)
{
    m_signalTriggered = true;
    QCOMPARE(m_expectSent, false);

    compareMessages(m_expectedMessage, msg);
}

void QXmppPubSubTestHelper::compareIqs(const QXmppPubSubIq &lhs, const QXmppPubSubIq &rhs)
{
    QCOMPARE(lhs.from(), rhs.from());
    QCOMPARE(lhs.id(), rhs.id());
    QCOMPARE(lhs.lang(), rhs.lang());
    QCOMPARE(lhs.to(), rhs.to());
    QCOMPARE(lhs.type(), rhs.type());
    QCOMPARE(lhs.queryType(), rhs.queryType());
    QCOMPARE(lhs.queryNode(), rhs.queryNode());
    QCOMPARE(lhs.publishOptions().type(), rhs.publishOptions().type());
}

void QXmppPubSubTestHelper::compareMessages(const QXmppMessage &lhs, const QXmppMessage &rhs)
{
    QCOMPARE(lhs.pubSubEvent().node(), rhs.pubSubEvent().node());
    QCOMPARE(lhs.from(), rhs.from());
    QCOMPARE(lhs.id(), rhs.id());
    QCOMPARE(lhs.isAttentionRequested(), rhs.isAttentionRequested());
    QCOMPARE(lhs.isMarkable(), rhs.isMarkable());
    QCOMPARE(lhs.isPrivate(), rhs.isPrivate());
    QCOMPARE(lhs.isReceiptRequested(), rhs.isReceiptRequested());
    QCOMPARE(lhs.lang(), rhs.lang());
    QCOMPARE(lhs.to(), rhs.to());
    QCOMPARE(lhs.thread(), rhs.thread());
    QCOMPARE(lhs.stamp(), rhs.stamp());
    QCOMPARE(lhs.type(), rhs.type());
}

QTEST_MAIN(tst_QXmppPubSubManager)
#include "tst_qxmpppubsubmanager.moc"
