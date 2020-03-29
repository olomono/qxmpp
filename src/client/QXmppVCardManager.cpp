/*
 * Copyright (C) 2008-2020 The QXmpp developers
 *
 * Author:
 *  Manjeet Dahiya
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

#include "QXmppVCardManager.h"

#include "QXmppClient.h"
#include "QXmppConstants_p.h"
#include "QXmppUtils.h"
#include "QXmppVCardIq.h"

class QXmppVCardManagerPrivate
{
public:
    QXmppVCardIq ownVCard;
    bool isOwnVCardReceived;
};

QXmppVCardManager::QXmppVCardManager()
    : d(new QXmppVCardManagerPrivate)
{
    d->isOwnVCardReceived = false;
}

QXmppVCardManager::~QXmppVCardManager()
{
    delete d;
}

/// Requests the vCard of a bare JID.
/// Once received, either contactVCardReceived() or ownVCardReceived() is emitted.
///
/// \param bareJid bare Jid for which the vCard is requested
///
QString QXmppVCardManager::requestVCard(const QString& bareJid)
{
    QXmppVCardIq request(bareJid);
    if (client()->sendPacket(request))
        return request.id();
    else
        return QString();
}

/// Requests the vCard of the user whose client is connected.
/// Once receivedn either contactVCardReceived or ownVCardReceived() is emitted.
/// The received vCard can be retrieved by using ownVCard().
///
QString QXmppVCardManager::requestOwnVCard()
{
    return requestVCard();
}

/// Returns true if the vCard of the user whose client is connected has been received, otherwise false.
///
/// \return bool
///
bool QXmppVCardManager::isOwnVCardReceived() const
{
    return d->isOwnVCardReceived;
}

/// Returns the vCard of the user whose client is connected.
///
/// \return QXmppVCard
///
const QXmppVCardIq& QXmppVCardManager::ownVCard() const
{
    return d->ownVCard;
}

/// Sets the vCard of the user whose client is connected.
///
/// \param clientVCard QXmppVCard
///
void QXmppVCardManager::setOwnVCard(const QXmppVCardIq& clientVCard)
{
    d->ownVCard = clientVCard;
    d->ownVCard.setTo("");
    d->ownVCard.setFrom("");
    d->ownVCard.setType(QXmppIq::Set);
    client()->sendPacket(d->ownVCard);
}

/// \cond
QStringList QXmppVCardManager::discoveryFeatures() const
{
    // XEP-0054: vcard-temp
    return QStringList() << ns_vcard;
}

bool QXmppVCardManager::handleStanza(const QDomElement& element)
{
    if (element.tagName() == "iq" && QXmppVCardIq::isVCard(element)) {
        QXmppVCardIq vCardIq;
        vCardIq.parse(element);

        if (vCardIq.type() == QXmppIq::Error && vCardIq.error().type() == QXmppIq::Error::Cancel) {
            switch(vCardIq.error().condition()) {
            case QXmppIq::Error::ServiceUnavailable:
                emit noContactVCardExists();
                return true;
                break;
            case QXmppIq::Error::ItemNotFound:
                d->ownVCard = QXmppVCardIq();
                emit noOwnVCardExists();
                return true;
                break;
            default:
                return false;
            }
        }

        if (vCardIq.from().isEmpty() || vCardIq.from() == client()->configuration().jidBare()) {
            d->ownVCard = vCardIq;


            if (vCardIq == QXmppVCardIq()) {
                emit noOwnVCardExists();
                return true;
            }

            d->isOwnVCardReceived = true;
            emit ownVCardReceived(vCardIq);
            return true;
        }

        emit contactVCardReceived(vCardIq);
        return true;
    }

    return false;
}
/// \endcond
