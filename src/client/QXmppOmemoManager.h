/*
 * Copyright (C) 2008-2020 The QXmpp developers
 *
 * Author:
 *  Melvin Keskin
 *  Germán Márquez Mejía
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

#ifndef QXMPPOMEMOMANAGER_H
#define QXMPPOMEMOMANAGER_H

#include "QXmppClientExtension.h"

#include <QScopedPointer>

class QXmppOmemoManagerPrivate;

///
/// \brief The QXmppOmemoManager class manages OMEMO encryption as defined in \xep{0384}: OMEMO Encryption.
///
/// \ingroup Managers
///
/// \since QXmpp TODO: Add version
///
class QXMPP_EXPORT QXmppOmemoManager : public QXmppClientExtension
{
    Q_OBJECT

public:
	QXmppOmemoManager();
	~QXmppOmemoManager();

    QStringList discoveryFeatures() const override;

    /// \cond
    bool handleStanza(const QDomElement &stanza) override;
    /// \endcond

Q_SIGNALS:
	// TODO: Add signals

protected:
    void setClient(QXmppClient *client) override;

private Q_SLOTS:
    void handleDiscoInfo(const QXmppDiscoveryIq &iq);

private:
	// TODO: Add private methods

	QScopedPointer<QXmppOmemoManagerPrivate> d;
};

#endif  // QXMPPOMEMOMANAGER_H
