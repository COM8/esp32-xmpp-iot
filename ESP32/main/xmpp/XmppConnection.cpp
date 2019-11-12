#include "XmppConnection.hpp"

//---------------------------------------------------------------------------
namespace espiot::xmpp {
//---------------------------------------------------------------------------
XmppConnection::XmppConnection(const XmppAccount* account) : account(account),
                                                             tcpConnection(account) {}

void XmppConnection::connect() {
    tcpConnection.connect();
}

void XmppConnection::disconnect() {
    tcpConnection.disconnect();
}
//---------------------------------------------------------------------------
} // namespace espiot::xmpp
//---------------------------------------------------------------------------