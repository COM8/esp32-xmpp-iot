#include "XmppClient.hpp"
#include <iostream>

//---------------------------------------------------------------------------
namespace espiot::xmpp {
//---------------------------------------------------------------------------
XmppClient::XmppClient(const XmppAccount&& account, smooth::core::Task& task) : account(account),
                                                                                connection(&(this->account), task),
                                                                                task(task) {}
void XmppClient::connect() {
    std::cout << "XMPP Client connecting..." << std::endl;
    connection.connect();
}

void XmppClient::disconnect() {
    std::cout << "XMPP Client disconnecting..." << std::endl;
    connection.disconnect();
}
//---------------------------------------------------------------------------
} // namespace espiot::xmpp
//---------------------------------------------------------------------------