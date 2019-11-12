#pragma once

#include "../XmppAccount.hpp"

//---------------------------------------------------------------------------
namespace espiot::xmpp::tcp {
//---------------------------------------------------------------------------
class TcpConnection {
    private:
    const XmppAccount* account;

    public:
    TcpConnection(const XmppAccount* account);

    void connect();
    void disconnect();
};
//---------------------------------------------------------------------------
} // namespace espiot::xmpp::tcp
//---------------------------------------------------------------------------