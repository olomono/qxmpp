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
#include "QXmppOmemoEnvelope.h"
#include "util.h"

Q_DECLARE_METATYPE(QXmppOmemoEnvelope);

class tst_QXmppOmemoData : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();

    void testOmemoElement_data();
    void testOmemoElement();

private:
};

void tst_QXmppOmemoData::initTestCase()
{
}

void tst_QXmppOmemoData::testOmemoElement_data()
{
//    QTest::addColumn<QString>("xml");

//    QTest::addRow("validOmemoElement")
}

void tst_QXmppOmemoData::testOmemoElement()
{

}

QTEST_MAIN(tst_QXmppOmemoData)
#include "tst_qxmppomemodata.moc"
