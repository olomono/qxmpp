/*
 * Copyright (C) 2008-2020 The QXmpp developers
 *
 * Author:
 *  Jeremy Lainé
 *  Germán Márquez Mejía
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

#include "QXmppOmemoEnvelope.h"

#include "util.h"
#include <QObject>

class tst_QXmppOmemoEnvelope : public QObject
{
    Q_OBJECT

private:
    bool containsKeyValueField(const QList<QXmppDataForm::Field> fields, const QLatin1String &key, const QLatin1String &value);

private slots:
    void testOmemoEnvelope();
};

void tst_QXmppOmemoEnvelope::testOmemoEnvelope()
{
    const QByteArray xml(
        "<iq"
        " id=\"items1\""
        " to=\"pubsub.shakespeare.lit\""
        " from=\"francisco@denmark.lit/barracks\""
        " type=\"result\">"
        "<pubsub xmlns=\"http://jabber.org/protocol/pubsub\">"
        "<publish node=\"storage:bookmarks\">"
        "<item id=\"current\">"
        "<storage xmlns=\"storage:bookmarks\">"
        "<conference"
        " autojoin=\"true\""
        " jid=\"theplay@conference.shakespeare.lit\""
        " name=\"The Play's the Thing\">"
        "<nick>JC</nick>"
        "</conference>"
        "</storage>"
        "</item>"
        "</publish>"
        "<publish-options>"
        "<x xmlns=\"jabber:x:data\" type=\"submit\">"
        "<field type=\"hidden\" var=\"FORM_TYPE\">"
        "<value>http://jabber.org/protocol/pubsub#publish-options</value>"
        "</field>"
        "<field type=\"text-single\" var=\"pubsub#access_model\">"
        "<value>presence</value>"
        "</field>"
        "</x>"
        "</publish-options>"
        "</pubsub>"
        "</iq>");

    QXmppPubSubIq iq;
    parsePacket(iq, xml);
    QCOMPARE(iq.id(), QLatin1String("items1"));
    QCOMPARE(iq.to(), QLatin1String("pubsub.shakespeare.lit"));
    QCOMPARE(iq.from(), QLatin1String("francisco@denmark.lit/barracks"));
    QCOMPARE(iq.type(), QXmppIq::Result);
    QCOMPARE(iq.queryType(), QXmppPubSubIq::PublishQuery);
    QCOMPARE(iq.queryJid(), QString());
    QCOMPARE(iq.queryNode(), QLatin1String("storage:bookmarks"));
    QCOMPARE(iq.publishOptions().type(), QXmppDataForm::Type::Submit);
    QCOMPARE(iq.publishOptions().fields().size(), 2);
    QVERIFY(containsKeyValueField(iq.publishOptions().fields(), QLatin1String("pubsub#access_model"), QLatin1String("presence")));
    serializePacket(iq, xml);
}

QTEST_MAIN(tst_QXmppOmemoEnvelope)
#include "tst_qxmppomemoenvelope.moc"
