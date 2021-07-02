/*
 * Copyright (C) 2008-2021 The QXmpp developers
 *
 * Author:
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

#include "QXmppOmemoManager.h"

#include <QDomElement>

#include <fingerprint.h>

#include "QXmppClient.h"
#include "QXmppConstants_p.h"
#include "QXmppDiscoveryManager.h"
#include "QXmppStreamFeatures.h"
#include "QXmppUtils.h"

class QXmppOmemoManagerPrivate
{
public:
	QXmppOmemoManagerPrivate();
};

QXmppOmemoManagerPrivate::QXmppOmemoManagerPrivate()
{
}

///
/// Default constructor.
///
QXmppOmemoManager::QXmppOmemoManager()
	: d(new QXmppOmemoManagerPrivate)
{


//    signal_context *global_context;
//    signal_context_create(&global_context, user_data);
//    signal_context_set_crypto_provider(global_context, &provider);
//    signal_context_set_locking_functions(global_context, lock_function, unlock_function);
}

QXmppOmemoManager::~QXmppOmemoManager() = default;

///
/// This adds the \c  namespace to the features.
///
QStringList QXmppOmemoManager::discoveryFeatures() const
{
    return QStringList {
        ns_omemo
    };
}

/// \cond
bool QXmppOmemoManager::handleStanza(const QDomElement &stanza)
{
//    fingerprint fp = fingerprint_create();


    // TODO
    return false;
}
/// \endcond

void QXmppOmemoManager::setClient(QXmppClient *client)
{
//    QXmppClientExtension::setClient(client);
//    // get service discovery manager
//    auto *disco = client->findExtension<QXmppDiscoveryManager>();
//    if (disco) {
//		connect(disco, &QXmppDiscoveryManager::infoReceived, this, &QXmppOmemoManager::handleDiscoInfo);
//    }

//    connect(client, &QXmppClient::disconnected, [=]() {
//        setSupportedByServer(false);
//    });
}
