/*
 * Copyright (C) 2008-2020 The QXmpp developers
 *
 * Author:
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

#include <QDomElement>

#include "QXmppClient.h"
#include "QXmppPubSubManager.h"
#include "QXmppUtils.h"

QString QXmppPubSubManager::publishPepItem(const QString &nodeName, const QXmppPubSubItem &item)
{
    return publishPepItem(nodeName, item, QXmppDataForm());
}

QString QXmppPubSubManager::publishPepItem(const QString &nodeName, const QXmppPubSubItem &item, const QXmppDataForm &publishOptions)
{
    return publishPepItems(nodeName, QList<QXmppPubSubItem>({item}), publishOptions);
}

QString QXmppPubSubManager::publishPepItems(const QString &nodeName, const QList<QXmppPubSubItem> &items)
{
    return publishPepItems(nodeName, items, QXmppDataForm());
}

QString QXmppPubSubManager::publishPepItems(const QString &nodeName, const QList<QXmppPubSubItem> &items, const QXmppDataForm &publishOptions)
{
    return publishItems(client()->configuration().jidBare(), nodeName, items, publishOptions);
}

QString QXmppPubSubManager::retractPepItem(const QString &nodeName, const QString &itemId)
{
    return retractItem(client()->configuration().jidBare(), nodeName, itemId);
}

QString QXmppPubSubManager::createPepNode(const QString &nodeName)
{
    return createNode(client()->configuration().jidBare(), nodeName);
}

QString QXmppPubSubManager::deletePepNode(const QString &nodeName)
{
    return deleteNode(client()->configuration().jidBare(), nodeName);
}

QString QXmppPubSubManager::createNode(const QString &jid, const QString &nodeName)
{
    QXmppPubSubIq request;
    request.setType(QXmppIq::Set);
    request.setQueryType(QXmppPubSubIq::QueryType::CreateQuery);
    request.setQueryNode(nodeName);
    request.setTo(jid);

    if (client()->sendPacket(request))
        return request.id();
    return QString();
}

QString QXmppPubSubManager::deleteNode(const QString &jid, const QString &nodeName)
{
    QXmppPubSubIq request;
    request.setType(QXmppIq::Set);
    request.setQueryType(QXmppPubSubIq::QueryType::DeleteQuery);
    request.setQueryNode(nodeName);
    request.setTo(jid);

    if (client()->sendPacket(request))
        return request.id();
    return QString();
}

QString QXmppPubSubManager::publishItem(const QString &jid, const QString &nodeName, const QXmppPubSubItem &item)
{
    return publishItem(jid, nodeName, item, QXmppDataForm());
}

QString QXmppPubSubManager::publishItem(const QString &jid, const QString &nodeName, const QXmppPubSubItem &item, const QXmppDataForm &publishOptions)
{
    return publishItems(jid, nodeName, QList<QXmppPubSubItem>({item}), publishOptions);
}

QString QXmppPubSubManager::publishItems(const QString &jid, const QString &nodeName, const QList<QXmppPubSubItem> &items)
{
    return publishItems(jid, nodeName, items, QXmppDataForm());
}

QString QXmppPubSubManager::publishItems(const QString &jid, const QString &nodeName, const QList<QXmppPubSubItem> &items, const QXmppDataForm &publishOptions)
{
    QXmppPubSubIq request;
    request.setType(QXmppIq::Set);
    request.setQueryType(QXmppPubSubIq::QueryType::PublishQuery);
    request.setQueryNode(nodeName);
    request.setItems(items);

    if (!publishOptions.isNull())
        request.setPublishOptions(publishOptions);

    request.setTo(jid);

    if (client()->sendPacket(request))
        return request.id();
    return QString();
}

QString QXmppPubSubManager::retractItem(const QString &jid, const QString &nodeName, const QString &itemId)
{
    QXmppPubSubIq request;
    request.setType(QXmppIq::Set);
    request.setQueryType(QXmppPubSubIq::QueryType::RetractQuery);
    request.setQueryNode(nodeName);
    request.setItems(QList<QXmppPubSubItem>({QXmppPubSubItem(itemId)}));
    request.setTo(jid);

    if (client()->sendPacket(request))
        return request.id();
    return QString();
}

QString QXmppPubSubManager::requestItem(const QString &jid, const QString &nodeName, const QString &itemId)
{
    return requestItems(jid, nodeName, QStringList(itemId));
}

QString QXmppPubSubManager::requestItems(const QString &jid, const QString &nodeName)
{
    return requestItems(jid, nodeName, QStringList());
}

QString QXmppPubSubManager::requestItems(const QString &jid, const QString &nodeName, const QStringList &itemIds)
{
    QXmppPubSubIq request;
    request.setType(QXmppIq::Get);
    request.setQueryType(QXmppPubSubIq::QueryType::ItemsQuery);
    request.setQueryNode(nodeName);

    if (!itemIds.isEmpty()) {
        QList<QXmppPubSubItem> items;
        for (const QString &id : itemIds) {
            items << QXmppPubSubItem(id);
        }
        request.setItems(items);
    }

    request.setTo(jid);

    if (client()->sendPacket(request))
        return request.id();
    return QString();
}

/// \cond
bool QXmppPubSubManager::handleStanza(const QDomElement &element)
{
    if (element.tagName() == "iq" && QXmppPubSubIq::isPubSubIq(element)) {
        QXmppPubSubIq iq;
        iq.parse(element);

        if (iq.queryType() == QXmppPubSubIq::QueryType::ItemsQuery) {
            emit itemsReceived(iq);
            return true;
        }
    }

    return false;
}

void QXmppPubSubManager::setClient(QXmppClient* client)
{
    QXmppClientExtension::setClient(client);

    connect(client, &QXmppClient::messageReceived,
            this, &QXmppPubSubManager::_q_messageReceived);
}
/// \endcond

void QXmppPubSubManager::_q_messageReceived(const QXmppMessage &message)
{
    if (message.isPubSubEvent())
        emit eventNotificationReceived(message);
}
