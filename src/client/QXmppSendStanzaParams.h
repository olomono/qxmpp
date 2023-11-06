// SPDX-FileCopyrightText: 2022 Linus Jahn <lnj@kaidan.im>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include "QXmppGlobal.h"
#include "QXmppTrustLevel.h"

#include <optional>

#include <QSharedDataPointer>

class QXmppSendStanzaParamsPrivate;

class QXMPP_EXPORT QXmppSendStanzaParams
{
public:
    QXmppSendStanzaParams();
    QXmppSendStanzaParams(const QXmppSendStanzaParams &other);
    QXmppSendStanzaParams(QXmppSendStanzaParams &&);
    ~QXmppSendStanzaParams();
    QXmppSendStanzaParams &operator=(const QXmppSendStanzaParams &);
    QXmppSendStanzaParams &operator=(QXmppSendStanzaParams &&);

    QVector<QString> encryptionJids() const;
    void setEncryptionJids(QVector<QString>);

    std::optional<QXmpp::TrustLevels> acceptedTrustLevels() const;
    void setAcceptedTrustLevels(std::optional<QXmpp::TrustLevels> trustLevels);

private:
    QSharedDataPointer<QXmppSendStanzaParamsPrivate> d;
};
