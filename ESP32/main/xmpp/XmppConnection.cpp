#include "XmppConnection.hpp"
#include "crypto/CryptoUtils.hpp"
#include <iostream>

//---------------------------------------------------------------------------
namespace espiot::xmpp {
//---------------------------------------------------------------------------
XmppConnection::XmppConnection(const XmppAccount* account, smooth::core::Task& task) : account(account),
                                                                                       tcpConnection(account, task, *this, *this),
                                                                                       task(task),
                                                                                       state(DISCONNECTED) {}

void XmppConnection::connect() {
    if (state == DISCONNECTED) {
        setState(CONNECTING);
        tcpConnection.connect();
    }
}

void XmppConnection::disconnect() {
    tcpConnection.disconnect();
}

std::string XmppConnection::genInitialStreamHeader() {
    return "<?xml version='1.0'?><stream:stream from='" + account->jid.getBare() + "' to='" + account->jid.domainPart + "' version='1.0' xml:lang='en' xmlns='jabber:client' xmlns:stream='http://etherx.jabber.org/streams'>";
}

std::string XmppConnection::genPlainAuthMessage() {
    std::string passwordBase64 = crypto::toBase64("\0" + account->jid.userPart + "\0" + account->password);
    return "<auth xmlns='urn:ietf:params:xml:ns:xmpp-sasl' mechanism='PLAIN'>" + passwordBase64 + "</auth>";
}

void XmppConnection::setState(XmppConnectionState state) {
    if (state != this->state) {
        this->state = state;
        // TODO: trigger some kind of event
    }
}

XmppConnectionState XmppConnection::getState() const {
    return state;
}

void XmppConnection::onInitialStreamHeaderReply(const tcp::XmppPacket& packet) {
    std::string s = packet.to_string();
    if (s.find("stream:features") != std::string::npos) {
        std::string msg = genPlainAuthMessage();
        setState(SASL_PLAIN_AUTH_SEND);
        tcpConnection.send(msg);
    }
}

void XmppConnection::event(const smooth::core::network::event::ConnectionStatusEvent& event) {
    if (event.is_connected()) {
        std::string msg = genInitialStreamHeader();
        setState(INITIAL_STREAM_HEADER_SEND);
        tcpConnection.send(msg);
    } else {
    }
}

void XmppConnection::event(const tcp::XmppPacket& event) {
    std::wcout << "Received: " << event.to_wstring() << "\n";

    switch (state) {
        case INITIAL_STREAM_HEADER_SEND:
            onInitialStreamHeaderReply(event);
            break;

        default:
            break;
    }
}
//---------------------------------------------------------------------------
} // namespace espiot::xmpp
//---------------------------------------------------------------------------