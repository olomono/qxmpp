/*
 * Copyright (C) 2008-2020 The QXmpp developers
 *
 * Authors:
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
#include "QXmppClient.h"
#include "QXmppVCardIq.h"
#include "QXmppVCardManager.h"
#include "util.h"

Q_DECLARE_METATYPE(QXmppVCardIq);

class tst_QXmppVCardManager : public QObject
{
    Q_OBJECT

private slots:
    void testHandleStanza_data();
    void testHandleStanza();

private:
    QXmppClient m_client;
};

void tst_QXmppVCardManager::testHandleStanza_data()
{
    QTest::addColumn<QXmppVCardIq>("expectedIq");
    QTest::addColumn<bool>("accept");
    QTest::addColumn<bool>("success");
    QTest::addColumn<bool>("ownVCard");

    QXmppVCardIq iqForContactVCard;
    iqForContactVCard.setType(QXmppIq::Result);
    iqForContactVCard.setFrom("jer@jabber.org");
    iqForContactVCard.setTo("stpeter@jabber.org/roundabout");
    iqForContactVCard.setFullName("JeremieMiller");

    QTest::newRow("iqForContactVCard")
        << iqForContactVCard
        << true
        << true
        << false;

    QXmppVCardIq iqForNoContactVCard;
    iqForNoContactVCard.setType(QXmppIq::Error);
    iqForNoContactVCard.setTo("stpeter@jabber.org/roundabout");
    iqForNoContactVCard.setError({QXmppStanza::Error::Cancel, QXmppStanza::Error::ServiceUnavailable});

    QTest::newRow("iqForNoContactVCard")
        << iqForNoContactVCard
        << true
        << false
        << false;

    QXmppVCardIq iqWithoutFromAttributeForOwnVCard;
    iqWithoutFromAttributeForOwnVCard.setType(QXmppIq::Result);
    iqWithoutFromAttributeForOwnVCard.setTo("stpeter@jabber.org/roundabout");
    iqWithoutFromAttributeForOwnVCard.setFullName("Peter Saint-Andre");

    QTest::newRow("iqWithoutFromAttributeForOwnVCard")
        << iqWithoutFromAttributeForOwnVCard
        << true
        << true
        << true;

    QXmppVCardIq iqWithFromAttributeForOwnVCard;
    iqWithFromAttributeForOwnVCard.setType(QXmppIq::Result);
    iqWithFromAttributeForOwnVCard.setFrom("stpeter@jabber.org");
    iqWithFromAttributeForOwnVCard.setTo("stpeter@jabber.org/roundabout");
    iqWithFromAttributeForOwnVCard.setFullName("Peter Saint-Andre");

    QTest::newRow("iqWithFromAttributeForOwnVCard")
        << iqWithFromAttributeForOwnVCard
        << true
        << true
        << true;

    QXmppVCardIq iqWithoutFromAttributeForNoOwnVCard;
    iqWithoutFromAttributeForNoOwnVCard.setType(QXmppIq::Error);
    iqWithoutFromAttributeForNoOwnVCard.setTo("stpeter@jabber.org/roundabout");
    iqWithoutFromAttributeForNoOwnVCard.setError({QXmppStanza::Error::Cancel, QXmppStanza::Error::ItemNotFound});

    QTest::newRow("iqWithoutFromAttributeForNoOwnVCard")
        << iqWithoutFromAttributeForNoOwnVCard
        << true
        << false
        << true;

    QXmppVCardIq iqWithFromAttributeForNoOwnVCard;
    iqWithFromAttributeForNoOwnVCard.setType(QXmppIq::Error);
    iqWithFromAttributeForNoOwnVCard.setFrom("stpeter@jabber.org");
    iqWithFromAttributeForNoOwnVCard.setTo("stpeter@jabber.org/roundabout");
    iqWithFromAttributeForNoOwnVCard.setError({QXmppStanza::Error::Cancel, QXmppStanza::Error::ItemNotFound});

    QTest::newRow("iqWithFromAttributeForNoOwnVCard")
        << iqWithFromAttributeForNoOwnVCard
        << true
        << false
        << true;

    QXmppVCardIq iqWithoutFromAttributeForEmptyOwnVCard;
    iqWithoutFromAttributeForEmptyOwnVCard.setType(QXmppIq::Result);
    iqWithoutFromAttributeForEmptyOwnVCard.setTo("stpeter@jabber.org/roundabout");

    QTest::newRow("iqWithoutFromAttributeForEmptyOwnVCard")
        << iqWithoutFromAttributeForEmptyOwnVCard
        << true
        << false
        << true;

    QXmppVCardIq iqWithFromAttributeForEmptyOwnVCard;
    iqWithFromAttributeForEmptyOwnVCard.setType(QXmppIq::Result);
    iqWithFromAttributeForEmptyOwnVCard.setFrom("stpeter@jabber.org");
    iqWithFromAttributeForEmptyOwnVCard.setTo("stpeter@jabber.org/roundabout");

    QTest::newRow("iqWithFromAttributeForEmptyOwnVCard")
        << iqWithFromAttributeForEmptyOwnVCard
        << true
        << false
        << true;

    QXmppVCardIq iqForSuccessfulOwnVCardUpdate;
    iqForSuccessfulOwnVCardUpdate.setType(QXmppIq::Result);
    iqForSuccessfulOwnVCardUpdate.setTo("stpeter@jabber.org/roundabout");

    QTest::newRow("iqForSuccessfulOwnVCardUpdate")
        << iqForSuccessfulOwnVCardUpdate
        << true
        // TODO: This should be "true" but the IQ stanza does not include an empty vcard element and is therefore not handled by "handleStanza()".
        // TODO: Maybe storing the ID of the IQ stanza and checking it for euqality could help.
        << false
        << true;
}

void tst_QXmppVCardManager::testHandleStanza()
{
    QXmppVCardManager *manager = new QXmppVCardManager();
    m_client.addExtension(manager);

    QFETCH(QXmppVCardIq, expectedIq);
    QFETCH(bool, accept);
    QFETCH(bool, success);
    QFETCH(bool, ownVCard);

    bool signalContactVCardReceivedTriggered = false;
    bool signalOwnVCardReceivedTriggered = false;
    bool signalNoContactVCardFoundTriggered = false;
    bool signalNoOwnVCardFoundTriggered = false;

    QObject context;
    connect(manager, &QXmppVCardManager::contactVCardReceived, &context, [&] (QXmppVCardIq iq) {
        signalContactVCardReceivedTriggered = true;
        QCOMPARE(iq, expectedIq);
    });
    connect(manager, &QXmppVCardManager::ownVCardReceived, &context, [&] (QXmppVCardIq iq) {
        signalOwnVCardReceivedTriggered = true;
        QCOMPARE(iq, expectedIq);
    });
    connect(manager, &QXmppVCardManager::noContactVCardExists, &context, [&] {
        signalNoContactVCardFoundTriggered = true;
    });
    connect(manager, &QXmppVCardManager::noOwnVCardExists, &context, [&] {
        signalNoOwnVCardFoundTriggered = true;
    });

    m_client.connectToServer("stpeter@jabber.org", {});
    m_client.disconnectFromServer();

    QByteArray data;
    QXmlStreamWriter xmlStream(&data);
    expectedIq.toXml(&xmlStream);

    QDomDocument doc;
    QCOMPARE(doc.setContent(data, true), true);
    QDomElement domElement = doc.documentElement();
    bool accepted = manager->handleStanza(domElement);

    QCOMPARE(accepted, accept);

    QCOMPARE(manager->ownVCard(), accept && success && ownVCard ? expectedIq : QXmppVCardIq());
    QCOMPARE(manager->isOwnVCardReceived(), accept && success && ownVCard);

    QCOMPARE(signalContactVCardReceivedTriggered, accept && success && !ownVCard);
    QCOMPARE(signalOwnVCardReceivedTriggered, accept && success && ownVCard);
    QCOMPARE(signalNoContactVCardFoundTriggered, accept && !success && !ownVCard);
    QCOMPARE(signalNoOwnVCardFoundTriggered, accept && !success && ownVCard);

    // Reset the manager because the next one does not have a set client vCard.
    m_client.removeExtension(manager);
}

QTEST_MAIN(tst_QXmppVCardManager)
#include "tst_qxmppvcardmanager.moc"
