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

#include "QXmppOmemoDeviceListElement.h"

class QXmppOmemoDeviceListElementPrivate : public QSharedData
{
public:
    quint32 id = 0;
    QString label;
};

QXmppOmemoDeviceListElement::QXmppOmemoDeviceListElement() : d(new QXmppOmemoDeviceListElementPrivate) {}

/// \brief Constructs a copy of \a other.
///
/// \param other
QXmppOmemoDeviceListElement::QXmppOmemoDeviceListElement(const QXmppOmemoDeviceListElement &other) : d(other.d) {}

/// \brief Assigns \a other to this event
///
/// \param other
QXmppOmemoDeviceListElement &QXmppOmemoDeviceListElement::operator=(const QXmppOmemoDeviceListElement &other)
{
    d = other.d;
    return *this;
}

quint32 QXmppOmemoDeviceListElement::id() const
{
    return d->id;
}

void QXmppOmemoDeviceListElement::setId(const quint32 id)
{
    d->id = id;
}

QString QXmppOmemoDeviceListElement::label() const
{
    return d->label;
}

void QXmppOmemoDeviceListElement::setLabel(const QString &label)
{
    d->label = label;
}

inline bool QXmppOmemoDeviceListElement::operator==(const QXmppOmemoDeviceListElement &other)
{
    return d->id == other.id();
}

inline uint QXmppOmemoDeviceListElement::qHash()
{
    return d->id;
}

void QXmppOmemoDeviceListElement::parse(const QDomElement &element)
{
    // TODO: Implement
}

void QXmppOmemoDeviceListElement::toXml(QXmlStreamWriter *writer) const
{
    // TODO: Implement
}
