// SPDX-FileCopyrightText: 2010 Jeremy Lainé <jeremy.laine@m4x.org>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include "QXmppIq.h"

class QXMPP_EXPORT QXmppPingIq : public QXmppIq
{
public:
    QXmppPingIq();
    static bool isPingIq(const QDomElement &element);

    /// \cond
    void toXmlElementFromChild(QXmlStreamWriter *writer) const override;
    /// \endcond
};
