#pragma once

#include "Jid.hpp"
#include <memory>
#include <string>
#include <smooth/core/network/InetAddress.h>

//---------------------------------------------------------------------------
namespace espiot::xmpp {
//---------------------------------------------------------------------------
class XmppAccount {
    public:
    using InetAddress_sp = std::shared_ptr<smooth::core::network::InetAddress>;

    const Jid jid;
    const std::string password;
    InetAddress_sp server;
    const uint16_t port;

    XmppAccount(const Jid&& jid, const std::string&& password, const InetAddress_sp server, const uint16_t port);
};
//---------------------------------------------------------------------------
} // namespace espiot::xmpp
//---------------------------------------------------------------------------