#include "XmppConnection.hpp"

//---------------------------------------------------------------------------
namespace espiot::xmpp {
//---------------------------------------------------------------------------
XmppConnection::XmppConnection(const XmppAccount* account, smooth::core::Task& task) : account(account),
                                                                                       tcpConnection(account, task),
                                                                                       task(task) {}

void XmppConnection::connect() {
    tcpConnection.connect();
}

void XmppConnection::disconnect() {
    tcpConnection.disconnect();
}
//---------------------------------------------------------------------------
} // namespace espiot::xmpp
//---------------------------------------------------------------------------