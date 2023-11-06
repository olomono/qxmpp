// SPDX-FileCopyrightText: 2019 Linus Jahn <lnj@kaidan.im>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include "QXmppBitsOfBinaryData.h"

#include <QVector>

class QDomElement;
class QXmlStreamWriter;

class QXMPP_EXPORT QXmppBitsOfBinaryDataList : public QVector<QXmppBitsOfBinaryData>
{
public:
    QXmppBitsOfBinaryDataList();
    ~QXmppBitsOfBinaryDataList();

    /// \cond
    QXmppBitsOfBinaryDataList(const QVector<QXmppBitsOfBinaryData> &data) : QVector<QXmppBitsOfBinaryData>(data) { }
    QXmppBitsOfBinaryDataList(QVector<QXmppBitsOfBinaryData> &&data) : QVector<QXmppBitsOfBinaryData>(std::move(data)) { }

    void parse(const QDomElement &element);
    void toXml(QXmlStreamWriter *writer) const;
    /// \endcond
};
