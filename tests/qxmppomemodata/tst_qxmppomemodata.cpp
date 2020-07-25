/*
 * Copyright (C) 2008-2020 The QXmpp developers
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
#include "QXmppOmemoDeviceBundle.h"
#include "QXmppOmemoDeviceList.h"
#include "QXmppOmemoDeviceListElement.h"
#include "util.h"

Q_DECLARE_METATYPE(QXmppOmemoDeviceBundle);

class tst_QXmppOmemoData : public QObject
{
    Q_OBJECT

private slots:
    void testOmemoDeviceBundle();
    void testOmemoDeviceList();
};

void tst_QXmppOmemoData::testOmemoDeviceBundle()
{
    const QByteArray xml(
        "<bundle xmlns='urn:xmpp:omemo:1'>"
          "<spk id='0'>Oy5TSG9vVVV4Wz9wUkUvI1lUXiVLIU5bbGIsUV0wRngK</spk>"
          "<spks>PTEoSk91VnRZSXBzcFlPXy4jZ3NKcGVZZ2d3YVJbVj8K</spks>"
          "<ik>a012U0R9WixWKUYhYipucnZOWG06akFOR3Q1NGNOOmUK</ik>"
          "<prekeys>"
            "<pk id='0'>eDM2cnBiTmo4MmRGQ1RYTkZ0YnVwajJtNWdPdzkxZ0gK</pk>"
            "<pk id='1'>aDRHdkcxNDNYUmJSNWVObnNWd0RCSzE1QlVKVGQ1RVEK</pk>"
            "<pk id='99'>YlNLNFZJWDU2Z1hlUFZPRHg2VXZPQTE1VmFYUlFNWEQK</pk>"
          "</prekeys>"
        "</bundle>"
    );

    QMap<quint64, QByteArray> expectedPublicPreKeys = {
        {0, QByteArray::fromBase64("eDM2cnBiTmo4MmRGQ1RYTkZ0YnVwajJtNWdPdzkxZ0gK")},
        {1, QByteArray::fromBase64("a012U0R9WixWKUYhYipucnZOWG06akFOR3Q1NGNOOmUK")},
        {99, QByteArray::fromBase64("YlNLNFZJWDU2Z1hlUFZPRHg2VXZPQTE1VmFYUlFNWEQK")}
    };

    QXmppOmemoDeviceBundle bundle;
    parsePacket(bundle, xml);

    QCOMPARE(bundle.publicSignedPreKeyId(), 0);
    QCOMPARE(bundle.publicSignedPreKey().toBase64(), "Oy5TSG9vVVV4Wz9wUkUvI1lUXiVLIU5bbGIsUV0wRngK");
    QCOMPARE(bundle.publicSignedPreKeySignature().toBase64(), "PTEoSk91VnRZSXBzcFlPXy4jZ3NKcGVZZ2d3YVJbVj8K");
    QCOMPARE(bundle.publicIdentityKey().toBase64(), "a012U0R9WixWKUYhYipucnZOWG06akFOR3Q1NGNOOmUK");
    QCOMPARE(bundle.publicPreKeys(), expectedPublicPreKeys);

    serializePacket(bundle, xml);

    bundle = QXmppOmemoDeviceBundle();
    bundle.setPublicSignedPreKeyId(0);
    bundle.setPublicSignedPreKey(QByteArray::fromBase64("Oy5TSG9vVVV4Wz9wUkUvI1lUXiVLIU5bbGIsUV0wRngK"));
    bundle.setPublicSignedPreKeySignature(QByteArray::fromBase64("PTEoSk91VnRZSXBzcFlPXy4jZ3NKcGVZZ2d3YVJbVj8K"));
    bundle.setPublicIdentityKey(QByteArray::fromBase64("a012U0R9WixWKUYhYipucnZOWG06akFOR3Q1NGNOOmUK"));
    bundle.setPublicPreKeys(expectedPublicPreKeys);

    serializePacket(bundle, xml);
}

void tst_QXmppOmemoData::testOmemoDeviceList()
{
    const QByteArray xml(
        "<devices xmlns='urn:xmpp:omemo:1'>"
          "<device id='12345' label='Dino on Lenovo Thinkpad T495'/>"
          "<device id='4223'/>"
          "<device id='31415' label='Conversations on Pixel 3'/>"
        "</devices>"
    );

    QSet<QXmppOmemoDeviceListElement> expectedDeviceListElements = {
        QXmppOmemoDeviceListElement {12345, "Dino on Lenovo Thinkpad T495"},
        QXmppOmemoDeviceListElement {4223, {}},
        QXmppOmemoDeviceListElement {31415, "Conversations on Pixel 3"}
    };

    QXmppOmemoDeviceList deviceList;
    parsePacket(deviceList, xml);

    QCOMPARE(deviceList, expectedDeviceListElements);

    serializePacket(deviceList, xml);

    deviceList = QXmppOmemoDeviceList();

    // TODO: Add setters

    serializePacket(deviceList, xml);
}

QTEST_MAIN(tst_QXmppOmemoData)
#include "tst_qxmppomemodata.moc"
