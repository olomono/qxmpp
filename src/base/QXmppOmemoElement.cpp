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

#include <QDomElement>
#include <QMultiMap>

#include "QXmppOmemoElement.h"

#include "QXmppConstants_p.h"
#include "QXmppUtils.h"

class QXmppOmemoElementPrivate : public QSharedData
{
public:
        quint32 senderDeviceId = 0;
        QByteArray payload;
        QMultiMap<QString, QXmppOmemoEnvelope> envelopes;
};

QXmppOmemoElement::QXmppOmemoElement() : d(new QXmppOmemoElementPrivate) {}

/// \brief Constructs a copy of \a other.
///
/// \param other
QXmppOmemoElement::QXmppOmemoElement(const QXmppOmemoElement &other) : d(other.d) {}

QXmppOmemoElement::~QXmppOmemoElement() = default;

/// \brief Assigns \a other to this event
///
/// \param other
QXmppOmemoElement &QXmppOmemoElement::operator=(const QXmppOmemoElement &other)
{
    d = other.d;
    return *this;
}

/// \brief Returns the ID of the sender's device.
///
/// This is the sender id (sid) according to XEP-0384.
///
/// \return the sender's device ID. 0 if unset
quint32 QXmppOmemoElement::senderDeviceId() const
{
    return d->senderDeviceId;
}

/// \brief Sets the ID of the sender's device.
///
/// \param id the sender's device ID
void QXmppOmemoElement::setSenderDeviceId(const quint32 id)
{
    d->senderDeviceId = id;
}

QByteArray QXmppOmemoElement::payload() const
{
    return d->payload;
}

void QXmppOmemoElement::setPayload(const QByteArray &payload)
{
    d->payload = payload;
}

void QXmppOmemoElement::addEnvelope(const QString recipientJid, QXmppOmemoEnvelope &envelope)
{
    d->envelopes.insert(recipientJid, envelope);
}

/// \brief Searches for an envelope by its recipient JID and device ID ('rid' attribute).
///
/// \param recipientJid the recipient's JID
/// \param recipientDeviceId the recipient's device ID
///
/// \return a pointer to the found envelope or nullptr otherwise
QXmppOmemoEnvelope *QXmppOmemoElement::searchEnvelope(const QString recipientJid, quint32 recipientDeviceId)
{
    for (auto &envelope : d->envelopes.values(recipientJid)) {
        if (envelope.recipientDeviceId() == recipientDeviceId)
            return &envelope;
    }

    return nullptr;
}

void QXmppOmemoElement::parse(const QDomElement &element)
{
    QDomElement header = element.firstChildElement("header");

    d->senderDeviceId = header.attribute("sid").toUInt();

    QDomElement recipient = header.firstChildElement("keys");
    while (!recipient.isNull()) {
        QString recipientJid = recipient.attribute("jid");
        QDomElement key = recipient.firstChildElement("key");

        while (!key.isNull()) {
            QXmppOmemoEnvelope envelope;
            envelope.parse(key);
            addEnvelope(recipientJid, envelope);

            key = key.nextSiblingElement("key");
        }

        recipient = recipient.nextSiblingElement("keys");
    }

    d->payload = QByteArray::fromBase64(element.firstChildElement("payload").text().toLatin1());
}

void QXmppOmemoElement::toXml(QXmlStreamWriter *writer) const
{
    writer->writeStartElement("encrypted");
    writer->writeAttribute("xmlns", ns_omemo);

    writer->writeStartElement("header");
    writer->writeAttribute("sid", QString::number(d->senderDeviceId));

    const QList<QString> recipientJids = d->envelopes.uniqueKeys();

    for (QString recipientJid : recipientJids) {
        writer->writeStartElement("keys");
        writer->writeAttribute("jid", recipientJid);

        for (const auto envelope : d->envelopes.values(recipientJid))
            envelope.toXml(writer);

        writer->writeEndElement(); // keys
    }

    writer->writeEndElement(); // header

    helperToXmlAddTextElement(writer, "payload", d->payload.toBase64());

    writer->writeEndElement(); // encrypted
}
