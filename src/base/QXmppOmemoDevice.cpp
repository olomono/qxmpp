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

#include "QXmppOmemoDevice.h"

class QXmppOmemoDevicePrivate : public QSharedData
{
public:
    quint32 id = 0;
    QString label;
    QString jid;
    QByteArray publicIdentityKey;
    QByteArray session;
    quint32 trustLevel = 0;
    quint32 status = 0;
};

QXmppOmemoDevice::QXmppOmemoDevice() : d(new QXmppOmemoDevicePrivate) {}

/// \brief Constructs a copy of \a other.
///
/// \param other
QXmppOmemoDevice::QXmppOmemoDevice(const QXmppOmemoDevice &other) : d(other.d) {}

/// \brief Assigns \a other to this event
///
/// \param other
QXmppOmemoDevice &QXmppOmemoDevice::operator=(const QXmppOmemoDevice &other)
{
    d = other.d;
    return *this;
}

/// Returns the device id or 0 if not set
quint32 QXmppOmemoDevice::id() const
{
    return d->id;
}

void QXmppOmemoDevice::setId(const quint32 id)
{
    d->id = id;
}

QString QXmppOmemoDevice::label() const
{
    return d->label;
}

void QXmppOmemoDevice::setLabel(const QString &label)
{
    d->label = label;
}

QString QXmppOmemoDevice::jid() const
{
    return d->jid;
}

void QXmppOmemoDevice::setJid(const QString &jid)
{
    d->jid = jid;
}

QByteArray QXmppOmemoDevice::publicIdentityKey() const
{
    return d->publicIdentityKey;
}

void QXmppOmemoDevice::setPublicIdentityKey(const QByteArray &key)
{
    d->publicIdentityKey = key;
}

QByteArray QXmppOmemoDevice::session() const
{
    return d->session;
}

void QXmppOmemoDevice::setSession(const QByteArray &session)
{
    d->session = session;
}

/// \brief Returns the trust level of this device's key.
///
/// The possible trust levels are determined by the implementor.
/// This library makes no assumptions about the stored value.
///
/// \return the trust level. 0 if unset
quint32 QXmppOmemoDevice::trustLevel() const
{
    return d->trustLevel;
}

/// \brief Sets the trust level of this device's key.
///
/// \see trustLevel
///
/// \param level the trust level
void QXmppOmemoDevice::setTrustLevel(const quint32 level)
{
    d->trustLevel = level;
}

/// \brief Returns the device status.
///
/// The possible device statuses are determined by the implementor.
/// This library makes no assumptions about the stored value.
///
/// \return the device status. 0 if unset
quint32 QXmppOmemoDevice::status() const
{
    return d->status;
}

/// \brief Sets the device status.
///
/// \see status
///
/// \param status device status
void QXmppOmemoDevice::setStatus(const quint32 status)
{
    d->status = status;
}
