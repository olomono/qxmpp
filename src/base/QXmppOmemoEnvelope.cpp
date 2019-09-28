#include "QXmppOmemoEnvelope.h"

#include "QXmppElement.h"
#include "QXmppUtils.h"

#include <QDomElement>

class QXmppOmemoEnvelopePrivate : public QSharedData
{
public:
    quint32 recipientDeviceId = 0;
    bool isUsedForKeyExchange = false;
    QByteArray data;
};

QXmppOmemoEnvelope::QXmppOmemoEnvelope() : d(new QXmppOmemoEnvelopePrivate) {}

/// \brief Constructs a copy of \a other.
///
/// \param other
QXmppOmemoEnvelope::QXmppOmemoEnvelope(const QXmppOmemoEnvelope &other) : d(other.d) {}

/// \brief Assigns \a other to this event
///
/// \param other
QXmppOmemoEnvelope &QXmppOmemoEnvelope::operator=(const QXmppOmemoEnvelope &other)
{
    d = other.d;
    return *this;
}

/// \brief Returns the ID of the recipient's device.
///
/// This is the recipient id (rid) according to XEP-0384.
///
/// \return the recipient's device ID. 0 if unset

quint32 QXmppOmemoEnvelope::recipientDeviceId() const
{
    return d->recipientDeviceId;
}

/// \brief Sets the ID of the recipient's device.
///
/// \param the recipient's device ID

void QXmppOmemoEnvelope::setRecipientDeviceId(const quint32 id)
{
    d->recipientDeviceId = id;
}

/// \brief Returns true if a pre-key was used to prepare this envelope.
///
/// Defaults to false
///
/// \return true if a pre-key was used to prepare this envelope. false otherwise.

bool QXmppOmemoEnvelope::isUsedForKeyExchange() const
{
    return d->isUsedForKeyExchange;
}

/// \brief Sets whether a pre-key was used to prepare this envelope.
///
/// \param used true if a pre-key was used to prepare this envelope. false otherwise.

void QXmppOmemoEnvelope::setIsUsedForKeyExchange(const bool isUsed)
{
    d->isUsedForKeyExchange = isUsed;
}

/// \brief Returns the BLOB containing the data for the underlying double ratchet library
///
/// Should be treated like an obcure BLOB to be passed as is to the ratchet library
/// for further processing
///
/// \return the binary data

QByteArray QXmppOmemoEnvelope::data() const
{
    return d->data;
}

/// \brief Sets the BLOB containing the data from the underlying double ratchet library
///
/// Should be treated like an obcure BLOB produced by the ratchet library
///
/// \param data the binary data

void QXmppOmemoEnvelope::setData(const QByteArray &data)
{
    d->data = data;
}

/// \cond
void QXmppOmemoEnvelope::parse(const QDomElement &element)
{
    d->recipientDeviceId = element.attribute("rid").toUInt();

    // TODO: Is there a method for the following?
    if (element.hasAttribute("kex"))
        d->isUsedForKeyExchange = element.attribute("kex").compare("true") || element.attribute("kex").compare("1");

    d->data = QByteArray::fromBase64(element.text().toLatin1());
}

void QXmppOmemoEnvelope::toXml(QXmlStreamWriter *writer) const
{
    helperToXmlAddTextElement(writer, "key", d->data.toBase64());

    helperToXmlAddAttribute(writer, "rid", QString::number(d->recipientDeviceId));
    if (d->isUsedForKeyExchange)
        helperToXmlAddAttribute(writer, "kex", "true");

    writer->writeEndElement();
}
/// \endcond
