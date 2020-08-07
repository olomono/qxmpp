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

#include "QXmppConstants_p.h"
#include "QXmppPubSubEvent.h"
#include "QXmppPubSubIq.h"

class QXmppPubSubEventPrivate : public QSharedData
{
public:
    QXmppPubSubIq::QueryType type;
    QString nodeName;
    QList<QXmppPubSubItem> items;
};

///
/// Constructs a PubSub event.
///
QXmppPubSubEvent::QXmppPubSubEvent() : d(new QXmppPubSubEventPrivate) {}

///
/// Constructs a copy of \a other.
///
/// \param other
///
QXmppPubSubEvent::QXmppPubSubEvent(const QXmppPubSubEvent &other) : d(other.d) {}

QXmppPubSubEvent::~QXmppPubSubEvent() {}

///
/// Assigns \a other to this event.
///
/// \param other
///
QXmppPubSubEvent &QXmppPubSubEvent::operator=(const QXmppPubSubEvent &other)
{
    d = other.d;
    return *this;
}

///
/// Returns the type of this event.
///
/// \return the event's type
///
QXmppPubSubIq::QueryType QXmppPubSubEvent::type() const
{
    return d->type;
}

///
/// Sets the type of this event.
///
/// \param type the event's type
///
void QXmppPubSubEvent::setType(const QXmppPubSubIq::QueryType type)
{
    d->type = type;
}

///
/// Returns the name of the event's node.
///
/// \return the node name
///
QString QXmppPubSubEvent::nodeName() const
{
    return d->nodeName;
}

///
/// Sets the name of the event's node.
///
/// \param nodeName
///
void QXmppPubSubEvent::setNodeName(const QString &nodeName)
{
    d->nodeName = nodeName;
}

///
/// Returns the PubSub items of the event.
///
/// \return a list of the PubSub items
///
QList<QXmppPubSubItem> QXmppPubSubEvent::items() const
{
    return d->items;
}

///
/// Sets the PubSub items of the event.
///
/// \param items
///
void QXmppPubSubEvent::setItems(const QList<QXmppPubSubItem> &items)
{
    d->items = items;
}

///
/// Determines whether the event object is initialized.
///
/// \return true if the node name is empty and the event contains no items
///
bool QXmppPubSubEvent::isNull() const
{
    return d->nodeName.isEmpty() && d->items.isEmpty();
}

/// \cond
void QXmppPubSubEvent::parse(const QDomElement &element)
{
    QDomElement nodeElement = element.firstChildElement();

    int type = PUBSUB_QUERIES.indexOf(nodeElement.tagName());
    if (type > -1)
        d->type = QXmppPubSubIq::QueryType(type);

    // parse node name
    d->nodeName = nodeElement.attribute("node");

    if (type != QXmppPubSubIq::ItemsQuery)
        return;

    // parse items
    QDomElement childElement = nodeElement.firstChildElement();

    while (!childElement.isNull())
    {
        QXmppPubSubItem item;
        item.parse(childElement);
        d->items << item;
        childElement = childElement.nextSiblingElement("item");
    }
}

void QXmppPubSubEvent::toXml(QXmlStreamWriter *writer) const
{
    writer->writeStartElement("event");
    writer->writeAttribute("xmlns", ns_pubsub_event);

    // write node element
    writer->writeStartElement(PUBSUB_QUERIES.at(d->type));

    // write node name
    writer->writeAttribute("node", d->nodeName);

    // write items
    for (const QXmppPubSubItem &item : d->items)
        item.toXml(writer);

    writer->writeEndElement();
    writer->writeEndElement();
}
/// \endcond
