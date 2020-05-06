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

#include <QByteArray>
#include <QMap>
#include "QXmppOmemoDeviceBundle.h"

class QXmppOmemoDeviceBundlePrivate : public QSharedData
{
public:
    QByteArray publicIdentityKey;
    QByteArray publicSignedPreKey;
    quint32 publicSignedPreKeyId = 0;
    QByteArray publicSignedPreKeySignature;
    QMap<quint64, QByteArray> publicPreKeys;
};

QXmppOmemoDeviceBundle::QXmppOmemoDeviceBundle() : d(new QXmppOmemoDeviceBundlePrivate) {}

/// \brief Constructs a copy of \a other.
///
/// \param other
QXmppOmemoDeviceBundle::QXmppOmemoDeviceBundle(const QXmppOmemoDeviceBundle &other) : d(other.d) {}

/// \brief Assigns \a other to this event
///
/// \param other
QXmppOmemoDeviceBundle &QXmppOmemoDeviceBundle::operator=(const QXmppOmemoDeviceBundle &other)
{
    d = other.d;
    return *this;
}

QByteArray QXmppOmemoDeviceBundle::publicIdentityKey() const
{
    return d->publicIdentityKey;
}

void QXmppOmemoDeviceBundle::setPublicIdentityKey(const QByteArray &key)
{
    d->publicIdentityKey = key;
}

QByteArray QXmppOmemoDeviceBundle::publicSignedPreKey() const
{
    return d->publicSignedPreKey;
}

void QXmppOmemoDeviceBundle::setPublicSignedPreKey(const QByteArray &key)
{
    d->publicSignedPreKey = key;
}

/// Returns the id of the signed pre-key or 0 if unset

quint32 QXmppOmemoDeviceBundle::publicSignedPreKeyId() const
{
    return d->publicSignedPreKeyId;
}

void QXmppOmemoDeviceBundle::setPublicSignedPreKeyId(const quint32 id)
{
    d->publicSignedPreKeyId = id;
}

QByteArray QXmppOmemoDeviceBundle::publicSignedPreKeySignature() const
{
    return d->publicSignedPreKeySignature;
}

void QXmppOmemoDeviceBundle::setPublicSignedPreKeySignature(const QByteArray &signature)
{
    d->publicSignedPreKeySignature = signature;
}

QMap<quint64, QByteArray> QXmppOmemoDeviceBundle::publicPreKeys() const
{
    return d->publicPreKeys;
}

void QXmppOmemoDeviceBundle::setPublicPreKeys(const QMap<quint64, QByteArray> &keys)
{
    d->publicPreKeys = keys;
}
