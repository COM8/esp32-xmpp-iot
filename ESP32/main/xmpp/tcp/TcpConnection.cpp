#include "TcpConnection.hpp"

//---------------------------------------------------------------------------
namespace espiot::xmpp::tcp {
//---------------------------------------------------------------------------
TcpConnection::TcpConnection(const XmppAccount* account) : account(account),
                                                           socket(nullptr) {}

void TcpConnection::connect() {
    // socket = smooth::core::network::Socket::create();
}

void TcpConnection::disconnect() {
}
//---------------------------------------------------------------------------
} // namespace espiot::xmpp::tcp
//---------------------------------------------------------------------------