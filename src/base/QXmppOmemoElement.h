/*
 * Copyright (C) 2008-2020 The QXmpp developers
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

#ifndef QXMPPOMEMOELEMENT_H
#define QXMPPOMEMOELEMENT_H

#include "QXmppOmemoEnvelope.h"
#include "QXmppStanza.h"

#include <QSharedDataPointer>

class QDomElement;
class QXmlStreamWriter;

class QXmppOmemoElementPrivate;

///
/// \brief The QXmppOmemoElement class represents an OMEMO element.
///
class QXMPP_EXPORT QXmppOmemoElement
{
public:
    QXmppOmemoElement();
    QXmppOmemoElement(const QXmppOmemoElement &other);
    ~QXmppOmemoElement();

    QXmppOmemoElement& operator=(const QXmppOmemoElement &other);

    quint32 senderDeviceId() const;
    void setSenderDeviceId(const quint32 id);

    QByteArray payload() const;
    void setPayload(const QByteArray &payload);

    void addEnvelope(const QString recipientJid, QXmppOmemoEnvelope &envelope);
    QXmppOmemoEnvelope *searchEnvelope(const QString recipientJid, quint32 recipientDeviceId);

    /// \cond
    void parse(const QDomElement &element);
    void toXml(QXmlStreamWriter *writer) const;
    /// \endcond

private:
    QSharedDataPointer<QXmppOmemoElementPrivate> d;
};

Q_DECLARE_TYPEINFO(QXmppOmemoElement, Q_MOVABLE_TYPE);

#endif // QXMPPOMEMOELEMENT_H
