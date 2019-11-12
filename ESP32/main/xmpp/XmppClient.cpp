#include "XmppClient.hpp"

//---------------------------------------------------------------------------
namespace espiot::xmpp {
//---------------------------------------------------------------------------
XmppClient::XmppClient(const XmppAccount&& account) : account(account),
                                                      connection(&(this->account)) {}
void XmppClient::connect() {
    connection.connect();
}

void XmppClient::disconnect() {
    connection.disconnect();
}
//---------------------------------------------------------------------------
} // namespace espiot::xmpp
//---------------------------------------------------------------------------