#include "XmppConnection.hpp"
#include <iostream>

//---------------------------------------------------------------------------
namespace espiot::xmpp {
//---------------------------------------------------------------------------
XmppConnection::XmppConnection(const XmppAccount* account, smooth::core::Task& task) : account(account),
                                                                                       tcpConnection(account, task, *this, *this),
                                                                                       task(task) {}

void XmppConnection::connect() {
    tcpConnection.connect();
}

void XmppConnection::disconnect() {
    tcpConnection.disconnect();
}

std::string XmppConnection::genInitialStreamHeader() {
    return "<?xml version='1.0'?><stream:stream from='" + account->jid.getBare() + "' to='" + account->jid.domainPart + "' version='1.0' xml:lang='en' xmlns='jabber:client' xmlns:stream='http://etherx.jabber.org/streams'>";
}

void XmppConnection::event(const smooth::core::network::event::ConnectionStatusEvent& event) {
    if (event.is_connected()) {
        std::string msg = genInitialStreamHeader();
        tcpConnection.send(msg);
    } else {
    }
}

void XmppConnection::event(const tcp::XmppPacket& event) {
    std::wcout << "Received: " << event.to_wstring() << "\n";
}
//---------------------------------------------------------------------------
} // namespace espiot::xmpp
//---------------------------------------------------------------------------