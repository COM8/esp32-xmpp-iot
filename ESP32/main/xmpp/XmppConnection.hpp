#pragma once

#include "XmppAccount.hpp"
#include "tcp/TcpConnection.hpp"
#include <smooth/core/Task.h>

//---------------------------------------------------------------------------
namespace espiot::xmpp {
//---------------------------------------------------------------------------
class XmppConnection {
    private:
    const XmppAccount* account;
    tcp::TcpConnection tcpConnection;
    smooth::core::Task& task;

    public:
    XmppConnection(const XmppAccount* account, smooth::core::Task& task);

    void connect();
    void disconnect();
};
//---------------------------------------------------------------------------
} // namespace espiot::xmpp
//---------------------------------------------------------------------------