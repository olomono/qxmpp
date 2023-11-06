// SPDX-FileCopyrightText: 2010 Jeremy Lainé <jeremy.laine@m4x.org>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include "QXmppDataForm.h"
#include "QXmppIq.h"

/// \brief The QXmppMucItem class represents a chat room "item".
///
/// It is used to convey information such as permissions.
///
/// \ingroup Stanzas

class QXMPP_EXPORT QXmppMucItem
{
public:
    /// This enum is used to represent long-lived permissions in a room (affiliations).
    enum Affiliation {
        UnspecifiedAffiliation,
        OutcastAffiliation,
        NoAffiliation,
        MemberAffiliation,
        AdminAffiliation,
        OwnerAffiliation
    };

    /// This enum is used to represent short-lived permissions in a room (roles).
    enum Role {
        UnspecifiedRole,
        NoRole,
        VisitorRole,
        ParticipantRole,
        ModeratorRole
    };

    QXmppMucItem();
    bool isNull() const;

    QString actor() const;
    void setActor(const QString &actor);

    Affiliation affiliation() const;
    void setAffiliation(Affiliation affiliation);

    QString jid() const;
    void setJid(const QString &jid);

    QString nick() const;
    void setNick(const QString &nick);

    QString reason() const;
    void setReason(const QString &reason);

    Role role() const;
    void setRole(Role role);

    /// \cond
    void parse(const QDomElement &element);
    void toXml(QXmlStreamWriter *writer) const;

    static Affiliation affiliationFromString(const QString &affiliationStr);
    static QString affiliationToString(Affiliation affiliation);
    static Role roleFromString(const QString &roleStr);
    static QString roleToString(Role role);
    /// \endcond
private:
    QString m_actor;
    Affiliation m_affiliation;
    QString m_jid;
    QString m_nick;
    QString m_reason;
    Role m_role;
};

/// \brief The QXmppMucAdminIq class represents a chat room administration IQ
/// as defined by \xep{0045}: Multi-User Chat.
///
/// It is used to get or modify room memberships.
///
/// \ingroup Stanzas

class QXMPP_EXPORT QXmppMucAdminIq : public QXmppIq
{
public:
    QList<QXmppMucItem> items() const;
    void setItems(const QList<QXmppMucItem> &items);

    /// \cond
    static bool isMucAdminIq(const QDomElement &element);
    /// \endcond

protected:
    /// \cond
    void parseElementFromChild(const QDomElement &element) override;
    void toXmlElementFromChild(QXmlStreamWriter *writer) const override;
    /// \endcond

private:
    QList<QXmppMucItem> m_items;
};

/// \brief The QXmppMucOwnerIq class represents a chat room configuration IQ as
/// defined by \xep{0045}: Multi-User Chat.
///
/// It is used to get or modify room configuration options.
///
/// \sa QXmppDataForm
///

class QXMPP_EXPORT QXmppMucOwnerIq : public QXmppIq
{
public:
    QXmppDataForm form() const;
    void setForm(const QXmppDataForm &form);

    /// \cond
    static bool isMucOwnerIq(const QDomElement &element);
    /// \endcond

protected:
    /// \cond
    void parseElementFromChild(const QDomElement &element) override;
    void toXmlElementFromChild(QXmlStreamWriter *writer) const override;
    /// \endcond

private:
    QXmppDataForm m_form;
};
