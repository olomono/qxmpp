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
    /// \brief Publish one item to a PEP node
    ///
    /// This is a convenience method equivalent to calling
    /// \ref QXmppPubSubManager#publishPepItem with no publish options
    ///
    /// \param nodeName the name of the PEP node to publish the item to
    /// \param item the item to publish
    /// \return the ID of the resulting IQ stanza if sent. Empty string otherwise
    QString publishPepItem(const QString &nodeName, const QXmppPubSubItem &item);

    /// \brief Publish one item to a PEP node
    ///
    /// This is a convenience method equivalent to calling
    /// \ref QXmppPubSubManager#publishPepItems with a single item in the items list
    ///
    /// \param nodeName the name of the PEP node to publish the item to
    /// \param item the item to publish
    /// \param publishOptions publish-options for fine tuning (optional). Pass
    /// an empty form to honor the default options of the PEP node
    /// \return the ID of the resulting IQ stanza if sent. Empty string otherwise
    QString publishPepItem(const QString &nodeName, const QXmppPubSubItem &item, const QXmppDataForm &publishOptions);

    /// \brief Publish items to a PEP node
    ///
    /// This is a convenience method equivalent to calling
    /// \ref QXmppPubSubManager#publishPepItems with no publish options
    ///
    /// \param nodeName the name of the PEP node to publish the items to
    /// \param items the items to publish
    /// \return the ID of the resulting IQ stanza if sent. Empty string otherwise
    QString publishPepItems(const QString &nodeName, const QList<QXmppPubSubItem> &items);

    /// \brief Publish items to a PEP node
    ///
    /// This is a convenience method equivalent to calling
    /// \ref QXmppPubSubManager#publishItems on the current account's bare JID
    ///
    /// \param nodeName the name  of the PEP node to publish the items to
    /// \param items the items to publish
    /// \param publishOptions publish-options for fine tuning (optional). Pass
    /// an empty form to honor the default options of the PEP node
    /// \return the ID of the resulting IQ stanza if sent. Empty string otherwise
    QString publishPepItems(const QString &nodeName, const QList<QXmppPubSubItem> &items, const QXmppDataForm &publishOptions);

    /// \brief Delete an item from a PEP node
    ///
    /// This is a convenience method equivalent to calling
    /// \ref QXmppPubSubManager#retractItem on the current account's bare JID
    ///
    /// \param nodeName the name of the PEP node to delete the item from
    /// \param itemId the ID of the item to delete
    /// \return the ID of the resulting IQ stanza if sent. Empty string otherwise
    QString retractPepItem(const QString &nodeName, const QString &itemId);

    /// \brief Create an empty PEP node with the default configuration
    ///
    /// This is a convenience method equivalent to calling
    /// \ref QXmppPubSubManager#createNode on the current account's bare JID
    ///
    /// Calling this before \ref QXmppPubSubManager#publishPepItems is usually not
    /// necessary when publishing to a node for the first time if the service
    /// suppports the auto-create feature (Section 7.1.4 of XEP-0060)
    ///
    /// \param nodeName the name of the PEP node to be created
    /// \return the ID of the resulting IQ stanza if sent. Empty string otherwise
    QString createPepNode(const QString &nodeName);

    /// \brief Delete a PEP node
    ///
    /// This is a convenience method equivalent to calling
    /// \ref QXmppPubSubManager#deleteNode on the current account's bare JID
    ///
    /// \param nodeName the name of the PEP node to delete along with all of its items
    /// \return the ID of the resulting IQ stanza if sent. Empty string otherwise
    QString deletePepNode(const QString &nodeName);

    // Generic pubsub (the pubsub service is the given entity)
    /// \brief Create an empty pubsub node with the default configuration
    ///
    /// Calling this before \ref QXmppPubSubManager#publishItems is usually not
    /// necessary when publishing to a node for the first time if the service
    /// suppports the auto-create feature (Section 7.1.4 of XEP-0060)
    ///
    /// \param jid Jabber ID of the entity hosting the pubsub service
    /// \param nodeName the name of the node to be created
    /// \return the ID of the resulting IQ stanza if sent. Empty string otherwise
    QString createNode(const QString &jid, const QString &nodeName);

    /// \brief Delete a pubsub node
    ///
    /// \param jid Jabber ID of the entity hosting the pubsub service
    /// \param nodeName the name of the node to delete along with all of its items
    /// \return the ID of the resulting IQ stanza if sent. Empty string otherwise
    QString deleteNode(const QString &jid, const QString &nodeName);

    /// \brief Publish one item to a pubsub node
    ///
    /// This is a convenience method equivalent to calling
    /// \ref QXmppPubSubManager#publishItem with no publish options
    ///
    /// \param jid Jabber ID of the entity hosting the pubsub service
    /// \param nodeName the name of the node to publish the item to
    /// \param item the item to publish
    /// \return the ID of the resulting IQ stanza if sent. Empty string otherwise
    QString publishItem(const QString &jid, const QString &nodeName, const QXmppPubSubItem &item);

    /// \brief Publish one item to a pubsub node
    ///
    /// This is a convenience method equivalent to calling
    /// \ref QXmppPubSubManager#publishItems with a single item in the items list
    ///
    /// \param jid Jabber ID of the entity hosting the pubsub service
    /// \param nodeName the name of the node to publish the item to
    /// \param item the item to publish
    /// \param publishOptions publish-options for the item (optional). Pass
    /// an empty form to honor the default options of the node
    /// \return the ID of the resulting IQ stanza if sent. Empty string otherwise
    QString publishItem(const QString &jid, const QString &nodeName, const QXmppPubSubItem &item, const QXmppDataForm &publishOptions);

    /// \brief Publish items to a pubsub node
    ///
    /// This is a convenience method equivalent to calling
    /// \ref QXmppPubSubManager#publishItems with no publish options
    ///
    /// \param jid Jabber ID of the entity hosting the pubsub service
    /// \param nodeName the name of the node to publish the items to
    /// \param items the items to publish
    /// \return the ID of the resulting IQ stanza if sent. Empty string otherwise
    QString publishItems(const QString &jid, const QString &nodeName, const QList<QXmppPubSubItem> &items);

    /// \brief Publish items to a pubsub node
    ///
    /// \param jid Jabber ID of the entity hosting the pubsub service
    /// \param nodeName the name of the node to publish the items to
    /// \param items the items to publish
    /// \param publishOptions publish-options for the items (optional). Pass
    /// an empty form to honor the default options of the node
    /// \return the ID of the resulting IQ stanza if sent. Empty string otherwise
    QString publishItems(const QString &jid, const QString &nodeName, const QList<QXmppPubSubItem> &items, const QXmppDataForm &publishOptions);

    /// \brief Delete an item from a pubsub node
    ///
    /// \param jid Jabber ID of the entity hosting the pubsub service
    /// \param nodeName the name of the node to delete the item from
    /// \param itemId the ID of the item to delete
    /// \return the ID of the resulting IQ stanza if sent. Empty string otherwise
    QString retractItem(const QString &jid, const QString &nodeName, const QString &itemId);

    /// \brief Request one item of an entity's node by ID
    ///
    /// This is a convenience method equivalent to calling
    /// \ref QXmppPubSubManager#requestItems with a list of item IDs containing
    /// only the given item ID
    ///
    /// \param jid Jabber ID of the entity hosting the pubsub service. For PEP this should be an account's bare JID
    /// \param nodeName the name of the node to query
    /// \param itemId the ID of the item to retrieve
    /// \return the ID of the resulting IQ stanza if sent. Empty string otherwise
    QString requestItem(const QString &jid, const QString &nodeName, const QString &itemId);

    /// \brief Request all items of an entity's node
    ///
    /// This is a convenience method equivalent to calling
    /// \ref QXmppPubSubManager#requestItems with an empty item ID list
    ///
    /// \param jid Jabber ID of the entity hosting the pubsub service. For PEP this should be an account's bare JID
    /// \param nodeName the name of the node to query
    /// \return the ID of the resulting IQ stanza if sent. Empty string otherwise
    QString requestItems(const QString &jid, const QString &nodeName);

    /// \brief Request items of an entity's node
    ///
    /// \param jid Jabber ID of the entity hosting the pubsub service. For PEP this should be an account's bare JID
    /// \param nodeName the name of the node to query
    /// \param itemIds the ID's of the items to retrieve. If empty, retrieves all the items
    /// \return the ID of the resulting IQ stanza if sent. Empty string otherwise
    QString requestItems(const QString &jid, const QString &nodeName, const QStringList &itemIds);

    // QXmppClientExtension interface
public:
    /// \cond
    bool handleStanza(const QDomElement &element) override;
    /// \endcond

signals:
    /// \brief Emitted when a pubsub event notification arrives
    void eventNotificationReceived(const QXmppMessage&);

    /// \brief Emitted when items are received after calling \ref QXmppPubSubManager#requestItems
    void itemsReceived(const QXmppPubSubIq&);

    // QXmppClientExtension interface
protected:
    /// \cond
    void setClient(QXmppClient *client) override;
    /// \endcond

private slots:
    void _q_messageReceived(const QXmppMessage &message);

private:
    /* This is here to allow adding attributes to the manager in the future
     * without breaking binary compatibility. Feel free to define QXmppPubSubManagerPrivate
     * when needed and remove this comment afterwards.
     */
    QXmppPubSubManagerPrivate *d;
};

#endif // QXMPPPUBSUBMANAGER_H
