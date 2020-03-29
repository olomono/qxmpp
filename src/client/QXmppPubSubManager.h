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

#ifndef QXMPPPUBSUBMANAGER_H
#define QXMPPPUBSUBMANAGER_H

#include "QXmppClientExtension.h"
#include "QXmppMessage.h"
#include "QXmppPubSubIq.h"

class QXmppPubSubManagerPrivate;

/// \brief The QXmppPubSubManager aims to provide Publish-Subscribe
/// functionality as specified in XEP-0060
///
/// However, it currently only supports a few pubsub use cases and all of the
/// Personal Eventing Protocol ones. The latter following XEP-0163, which allows
/// a standard XMPP user account to function as a virtual pubsub service.
///
/// To make use of this manager, you need to instantiate it and load it into
/// the QXmppClient instance as follows:
///
/// \code
/// QXmppPubSubManager *manager = new QXmppPubSubManager;
/// client->addExtension(manager);
/// \endcode
///
/// \note To subscribe to PEP event notifications use the \ref QXmppClientExtension#discoveryFeatures
/// method of your client extension according to section 9.2 of XEP-0060. For example:
/// \code
/// QStringList YourExtension::discoveryFeatures() const
/// {
///    return QStringList() << "http://jabber.org/protocol/tune+notify";
/// }
/// \endcode
///
/// \ingroup Managers
///
class QXMPP_EXPORT QXmppPubSubManager : public QXmppClientExtension
{
    Q_OBJECT

public:
    // PEP-specific (the pubsub service is the current account)
    QString createPepNode(const QString &nodeName);
    QString deletePepNode(const QString &nodeName);
    QString publishPepItem(const QString &nodeName, const QXmppPubSubItem &item);
    QString publishPepItem(const QString &nodeName, const QXmppPubSubItem &item, const QXmppDataForm &publishOptions);
    QString publishPepItems(const QString &nodeName, const QList<QXmppPubSubItem> &items);
    QString publishPepItems(const QString &nodeName, const QList<QXmppPubSubItem> &items, const QXmppDataForm &publishOptions);
    QString retractPepItem(const QString &nodeName, const QString &itemId);

    // Generic pubsub (the pubsub service is the given entity)
    QString createNode(const QString &jid, const QString &nodeName);
    QString deleteNode(const QString &jid, const QString &nodeName);
    QString publishItem(const QString &jid, const QString &nodeName, const QXmppPubSubItem &item);
    QString publishItem(const QString &jid, const QString &nodeName, const QXmppPubSubItem &item, const QXmppDataForm &publishOptions);
    QString publishItems(const QString &jid, const QString &nodeName, const QList<QXmppPubSubItem> &items);
    QString publishItems(const QString &jid, const QString &nodeName, const QList<QXmppPubSubItem> &items, const QXmppDataForm &publishOptions);
    QString retractItem(const QString &jid, const QString &nodeName, const QString &itemId);
    QString requestItem(const QString &jid, const QString &nodeName, const QString &itemId);
    QString requestItems(const QString &jid, const QString &nodeName);
    QString requestItems(const QString &jid, const QString &nodeName, const QStringList &itemIds);

    /// \cond
    bool handleStanza(const QDomElement &element) override;
    /// \endcond

signals:
    /// \brief Emitted when a pubsub event notification arrives
    ///
    /// \param &message
    ///
    void eventNotificationReceived(const QXmppMessage &message);

    /// \brief Emitted when items are received after calling \ref QXmppPubSubManager#requestItems
    ///
    /// \param &iq
    ///
    void itemsReceived(const QXmppPubSubIq &iq);

    // QXmppClientExtension interface
protected:
    /// \cond
    void setClient(QXmppClient *client) override;
    /// \endcond

private slots:
    void _q_messageReceived(const QXmppMessage &message);

private:
    /// This is here to allow adding attributes to the manager in the future
    /// without breaking binary compatibility. Feel free to define QXmppPubSubManagerPrivate
    /// when needed and remove this comment afterwards.
    ///
    QXmppPubSubManagerPrivate *d;
};

#endif // QXMPPPUBSUBMANAGER_H
