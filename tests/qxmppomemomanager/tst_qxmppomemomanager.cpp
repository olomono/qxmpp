/*
 * Copyright (C) 2008-2021 The QXmpp developers
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

#include <chrono>
#include <QObject>
#include <QXmppPubSubManager.h>
#include "QXmppClient.h"
#include "QXmppPubSubIq.h"
#include "QXmppMessage.h"
#include "QXmppDiscoveryManager.h"
#include "QXmppPubSubManager.h"
#include "QXmppPubSubItem.h"
#include "util.h"

#include "IntegrationTesting.h"

using namespace std::chrono_literals;

template<typename T, typename Handler>
void await(const QFuture<T> &future, Handler handler)
{
    auto *watcher = new QFutureWatcher<T>();
    QObject::connect(watcher, &QFutureWatcherBase::finished,
                     [watcher, handler { std::move(handler) }]() {
        handler(watcher->result());
        watcher->deleteLater();
    });
    watcher->setFuture(future);
}

class tst_QXmppOmemoManager : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();

    void testRequestNode();

signals:
    void itemsReceived();

private:
    QXmppPubSubManager *m_manager;
    QXmppClient m_client;
    QXmppLogger m_logger;
};

void tst_QXmppOmemoManager::initTestCase()
{
    m_manager = new QXmppPubSubManager();
    m_client.addExtension(m_manager);
    m_logger.setLoggingType(QXmppLogger::SignalLogging);
    m_client.setLogger(&m_logger);
}

void tst_QXmppOmemoManager::testRequestNode()
{
    SKIP_IF_INTEGRATION_TESTS_DISABLED();

    auto client = std::make_unique<QXmppClient>();
    QXmppPubSubManager manager;
    client->addExtension(&manager);
    auto config = IntegrationTests::clientConfiguration();

    QSignalSpy connectSpy(client.get(), &QXmppClient::connected);
    QSignalSpy disconnectSpy(client.get(), &QXmppClient::disconnected);

    // connect to server
    client->connectToServer(config);
    QVERIFY2(connectSpy.wait(), "Could not connect to server!");

    QSignalSpy itemsReceivedSpy(this, &tst_QXmppOmemoManager::itemsReceived);

//    await(client->findExtension<QXmppDiscoveryManager>()->requestDiscoItems("melvo3@olomono.de"), [=](QXmppDiscoveryManager::ItemsResult &&result) {
//        if (const auto items = std::get_if<QList<QXmppDiscoveryIq::Item>>(&result)) {
//            qDebug() << "Success";
//            for (const auto &item : *items) {
//                qDebug() << item.node();
//            }
//        } else if (auto error = std::get_if<QXmppStanza::Error>(&result)) {
//            qDebug() << "Result:" << error->text();
//        }
//        emit itemsReceived();
//    });

//    await(manager.createNode("melvo3@olomono.de", "einnode"), [=](QXmppPubSubManager::Result &&result) {
//        if (std::holds_alternative<QXmppPubSubManager::Success>(result)) {
//            qDebug() << "Success";
//        } else if (auto error = std::get_if<QXmppStanza::Error>(&result)) {
//            qDebug() << "Result:" << error->text();
//        }
//        emit itemsReceived();
//    });


    auto future = m_manager->requestItems<QXmppPubSubItem>("melvo1@olomono.de", "eu.siacs.conversations.axolotl.devicelist");
    await(future, [=](QXmppPubSubManager::ItemsResult<QXmppPubSubItem> &&result) {
        if (auto items = std::get_if<QXmppPubSubManager::Items<QXmppPubSubItem>>(&result)) {
            qDebug() << "Result:" << items->items.size();
        } else if (auto error = std::get_if<QXmppStanza::Error>(&result)) {
            qDebug() << "Result:" << error->text();
        }
        emit itemsReceived();
    });

    QVERIFY2(itemsReceivedSpy.wait(), "Request timed out!");

//    auto result = future.result();
//    if (auto items = std::get_if<QXmppPubSubManager::Items<QXmppPubSubItem>>(&result)) {
//        qDebug() << "TEST 1";
//        qDebug() << "Result:" << items->items.size();
//    } else if (auto error = std::get_if<QXmppStanza::Error>(&result)) {
//        qDebug() << "TEST 2";
//        qDebug() << "Result:" << error->text();
//    }
}

QTEST_MAIN(tst_QXmppOmemoManager)
#include "tst_qxmppomemomanager.moc"
