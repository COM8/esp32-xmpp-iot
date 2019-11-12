#pragma once

#include "XmppAccount.hpp"
#include "tcp/TcpConnection.hpp"

//---------------------------------------------------------------------------
namespace espiot::xmpp {
//---------------------------------------------------------------------------
class XmppConnection {
    private:
    const XmppAccount* account;
    tcp::TcpConnection tcpConnection;

    public:
    XmppConnection(const XmppAccount* account);

    void connect();
    void disconnect();
};
//---------------------------------------------------------------------------
} // namespace espiot::xmpp
//---------------------------------------------------------------------------