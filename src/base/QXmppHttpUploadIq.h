// SPDX-FileCopyrightText: 2019 Linus Jahn <lnj@kaidan.im>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include "QXmppIq.h"

#include <QSharedDataPointer>

class QUrl;
class QMimeType;

class QXmppHttpUploadRequestIqPrivate;
class QXmppHttpUploadSlotIqPrivate;

///
/// \brief Represents an HTTP File Upload IQ for requesting an upload slot as
/// defined by \xep{0363}: HTTP File Upload.
///
/// \since QXmpp 1.1
///
/// \ingroup Stanzas
///
class QXMPP_EXPORT QXmppHttpUploadRequestIq : public QXmppIq
{
public:
    QXmppHttpUploadRequestIq();
    QXmppHttpUploadRequestIq(const QXmppHttpUploadRequestIq &);
    QXmppHttpUploadRequestIq(QXmppHttpUploadRequestIq &&);
    ~QXmppHttpUploadRequestIq() override;

    QXmppHttpUploadRequestIq &operator=(const QXmppHttpUploadRequestIq &);
    QXmppHttpUploadRequestIq &operator=(QXmppHttpUploadRequestIq &&);

    QString fileName() const;
    void setFileName(const QString &filename);

    qint64 size() const;
    void setSize(qint64 size);

    QMimeType contentType() const;
    void setContentType(const QMimeType &type);

    static bool isHttpUploadRequestIq(const QDomElement &element);

protected:
    /// \cond
    void parseElementFromChild(const QDomElement &element) override;
    void toXmlElementFromChild(QXmlStreamWriter *writer) const override;
    /// \endcond

private:
    QSharedDataPointer<QXmppHttpUploadRequestIqPrivate> d;
};

///
/// \brief Represents an HTTP File Upload IQ result for receiving an upload slot as
/// defined by \xep{0363}: HTTP File Upload.
///
/// \since QXmpp 1.1
///
/// \ingroup Stanzas
///
class QXMPP_EXPORT QXmppHttpUploadSlotIq : public QXmppIq
{
public:
    QXmppHttpUploadSlotIq();
    QXmppHttpUploadSlotIq(const QXmppHttpUploadSlotIq &);
    QXmppHttpUploadSlotIq(QXmppHttpUploadSlotIq &&);
    ~QXmppHttpUploadSlotIq() override;

    QXmppHttpUploadSlotIq &operator=(const QXmppHttpUploadSlotIq &);
    QXmppHttpUploadSlotIq &operator=(QXmppHttpUploadSlotIq &&);

    QUrl putUrl() const;
    void setPutUrl(const QUrl &putUrl);

    QUrl getUrl() const;
    void setGetUrl(const QUrl &getUrl);

    QMap<QString, QString> putHeaders() const;
    void setPutHeaders(const QMap<QString, QString> &putHeaders);

    static bool isHttpUploadSlotIq(const QDomElement &element);

protected:
    /// \cond
    void parseElementFromChild(const QDomElement &element) override;
    void toXmlElementFromChild(QXmlStreamWriter *writer) const override;
    /// \endcond

private:
    QSharedDataPointer<QXmppHttpUploadSlotIqPrivate> d;
};
