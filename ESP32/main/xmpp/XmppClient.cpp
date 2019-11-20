#include "XmppClient.hpp"
#include <iostream>

//---------------------------------------------------------------------------
namespace espiot::xmpp {
//---------------------------------------------------------------------------
XmppClient::XmppClient(const XmppAccount&& account, smooth::core::Task& task) : account(account),
                                                                                connection(&(this->account), task, *this),
                                                                                task(task) {}
void XmppClient::connect() {
    std::cout << "XMPP Client connecting..." << std::endl;
    connection.connect();
}

void XmppClient::disconnect() {
    std::cout << "XMPP Client disconnecting..." << std::endl;
    connection.disconnect();
}

void XmppClient::event(const XmppConnectionState& event) {
    if (event == CONNECTED) {
        std::cout << "XMPP client connected\n";
    } else if (event == DISCONNECTED) {
        std::cout << "XMPP client disconnected\n";
    }
}
//---------------------------------------------------------------------------
} // namespace espiot::xmpp
//---------------------------------------------------------------------------