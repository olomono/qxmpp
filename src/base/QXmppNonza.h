// SPDX-FileCopyrightText: 2021 Linus Jahn <lnj@kaidan.im>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include "QXmppGlobal.h"

class QXmlStreamWriter;
class QDomElement;

class QXmppNonza
{
public:
    QXmppNonza() = default;
    virtual ~QXmppNonza() = default;

    virtual inline bool isXmppStanza() const { return false; }
    virtual void parse(const QDomElement &) = 0;
    virtual void toXml(QXmlStreamWriter *writer) const = 0;
};
