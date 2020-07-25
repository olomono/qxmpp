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

#ifndef QXMPPOMEMODEVICEBUNDLE_H
#define QXMPPOMEMODEVICEBUNDLE_H

#include <QByteArray>
#include <QMap>
#include <QSharedDataPointer>

#include "QXmppStanza.h"

class QXmppOmemoDeviceBundlePrivate;

///
/// \brief The QXmppOmemoDeviceBundle class represents an OMEMO bundle.
///
class QXMPP_EXPORT QXmppOmemoDeviceBundle
{
public:
    QXmppOmemoDeviceBundle();
    QXmppOmemoDeviceBundle(const QXmppOmemoDeviceBundle &other);
    ~QXmppOmemoDeviceBundle();

    QXmppOmemoDeviceBundle& operator=(const QXmppOmemoDeviceBundle &other);

    QByteArray publicIdentityKey() const;
    void setPublicIdentityKey(const QByteArray &key);

    QByteArray publicSignedPreKey() const;
    void setPublicSignedPreKey(const QByteArray &key);

    quint32 publicSignedPreKeyId() const;
    void setPublicSignedPreKeyId(const quint32 id);

    QByteArray publicSignedPreKeySignature() const;
    void setPublicSignedPreKeySignature(const QByteArray &signature);

    QMap<quint64, QByteArray> publicPreKeys() const;
    void setPublicPreKeys(const QMap<quint64, QByteArray> &keys);

    /// \cond
    void parse(const QDomElement &element);
    void toXml(QXmlStreamWriter *writer) const;
    /// \endcond

private:
    QSharedDataPointer<QXmppOmemoDeviceBundlePrivate> d;
};

Q_DECLARE_TYPEINFO(QXmppOmemoDeviceBundle, Q_MOVABLE_TYPE);

#endif // QXMPPOMEMODEVICEBUNDLE_H
