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

#include "QXmppOmemoDeviceList.h"

class QXmppOmemoDeviceListPrivate : public QSharedData {};

QXmppOmemoDeviceList::QXmppOmemoDeviceList() : d(new QXmppOmemoDeviceListPrivate) {}

/// \brief Constructs a copy of \a other.
///
/// \param other
QXmppOmemoDeviceList::QXmppOmemoDeviceList(const QXmppOmemoDeviceList &other) : d(other.d) {}

/// \brief Assigns \a other to this event
///
/// \param other
QXmppOmemoDeviceList &QXmppOmemoDeviceList::operator=(const QXmppOmemoDeviceList &other)
{
    d = other.d;
    return *this;
}
