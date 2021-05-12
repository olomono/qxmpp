/*
 * Copyright (C) 2008-2021 The QXmpp developers
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

#ifndef QXMPPOMEMODEVICE_H
#define QXMPPOMEMODEVICE_H

#include <QByteArray>
#include <QString>
#include <QSharedData>

#include "QXmppStanza.h"

class QXmppOmemoDevicePrivate;

///
/// \brief The QXmppOmemoDevice class represents an OMEMO device.
///
class QXMPP_EXPORT QXmppOmemoDevice
{
public:
    QXmppOmemoDevice();
    QXmppOmemoDevice(const QXmppOmemoDevice &other);
    ~QXmppOmemoDevice() = default;

    QXmppOmemoDevice& operator=(const QXmppOmemoDevice &other);

    int id() const;
    void setId(const int id);

    QString label() const;
    void setLabel(const QString &label);

    QString jid() const;
    void setJid(const QString &jid);

    QByteArray publicIdentityKey() const;
    void setPublicIdentityKey(const QByteArray &key);

    QByteArray session() const;
    void setSession(const QByteArray &session);

    quint32 trustLevel() const;
    void setTrustLevel(const quint32 level);

    quint32 status() const;
    void setStatus(const quint32 status);

private:
    QSharedDataPointer<QXmppOmemoDevicePrivate> d;
};

Q_DECLARE_TYPEINFO(QXmppOmemoDevice, Q_MOVABLE_TYPE);

#endif // QXMPPOMEMODEVICE_H
