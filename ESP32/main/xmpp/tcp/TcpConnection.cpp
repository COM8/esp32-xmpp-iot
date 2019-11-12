#include "TcpConnection.hpp"

//---------------------------------------------------------------------------
namespace espiot::xmpp::tcp {
//---------------------------------------------------------------------------
TcpConnection::TcpConnection(const XmppAccount* account) : account(account) {}

void TcpConnection::connect() {
}

void TcpConnection::disconnect() {
}
//---------------------------------------------------------------------------
} // namespace espiot::xmpp::tcp
//---------------------------------------------------------------------------