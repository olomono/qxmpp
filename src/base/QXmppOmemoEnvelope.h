/*
 * Copyright (C) 2008-2019 The QXmpp developers
 *
 * Author:
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


#ifndef QXMPPOMEMOENVELOPE_H
#define QXMPPOMEMOENVELOPE_H

#include "QXmppGlobal.h"
class QXmppOmemoEnvelopePrivate;
#include "QXmppStanza.h"

class QDomElement;
#include <QSharedData>
class QXmlStreamWriter;

///
/// \brief The QXmppOmemoEnvelope class represents an OMEMO envelope.
///
class QXMPP_EXPORT QXmppOmemoEnvelope
{
public:
    QXmppOmemoEnvelope();
    QXmppOmemoEnvelope(const QXmppOmemoEnvelope &other);
    ~QXmppOmemoEnvelope() = default;

    QXmppOmemoEnvelope& operator=(const QXmppOmemoEnvelope &other);

    quint32 recipientDeviceId() const;
    void setRecipientDeviceId(const quint32 id);

    bool isUsedForKeyExchange() const;
    void setIsUsedForKeyExchange(const bool isUsed);

    QByteArray data() const;
    void setData(const QByteArray &data);

    /// \cond
    void parse(const QDomElement &element);
    void toXml(QXmlStreamWriter *writer) const;
    /// \endcond

private:
    QSharedDataPointer<QXmppOmemoEnvelopePrivate> d;
};

Q_DECLARE_TYPEINFO(QXmppOmemoEnvelope, Q_MOVABLE_TYPE);

#endif // QXMPPOMEMOENVELOPE_H
