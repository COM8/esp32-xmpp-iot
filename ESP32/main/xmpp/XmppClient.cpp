#include "XmppClient.hpp"
#include "XmppUtils.hpp"
#include <iostream>

//---------------------------------------------------------------------------
namespace espiot::xmpp {
//---------------------------------------------------------------------------
XmppClient::XmppClient(const XmppAccount&& account, smooth::core::Task& task, ConnectionStatusEventListener& connectionStatusChanged, MessageListener& messageListener) : state(CLIENT_DISCONNECTED),
                                                                                                                                                                          account(account),
                                                                                                                                                                          connection(&(this->account), task, *this, *this),
                                                                                                                                                                          task(task),
                                                                                                                                                                          connectionStatusChanged(connectionStatusChanged),
                                                                                                                                                                          messageListener(messageListener) {}
void XmppClient::connect() {
    if (state == CLIENT_DISCONNECTED) {
        std::cout << "XMPP Client connecting..." << std::endl;
        setState(CLIENT_CONNECTING);
        connection.connect();
    }
}

void XmppClient::disconnect() {
    if (state == CLIENT_CONNECTING || state == CLIENT_CONNECTED) {
        std::cout << "XMPP Client disconnecting..." << std::endl;
        setState(CLIENT_DISCONNECTING);
        connection.disconnect();
    }
}

void XmppClient::setState(XmppClientConnectionState state) {
    if (state != this->state) {
        this->state = state;
        connectionStatusChanged.event(state);
    }
}

XmppClientConnectionState XmppClient::getState() {
    return this->state;
}

void XmppClient::send(std::string& msg) {
    connection.send(msg);
}

void XmppClient::send(std::wstring& msg) {
    connection.send(msg);
}

void XmppClient::sendMessage(std::string& to, std::string& body) {
    std::string msg = "<message from='" + account.jid.getFull() + "' id='" + randFakeUuid() + "' to='" + to + "' type='chat' xml:lang='en'><body>" + body + "</body></message>";
    send(msg);
}

void XmppClient::event(const XmppConnectionState& event) {
    if (event == CONNECTED) {
        std::cout << "XMPP client connected\n";
        setState(CLIENT_CONNECTED);
    } else if (event == DISCONNECTED) {
        std::cout << "XMPP client disconnected\n";
        setState(CLIENT_DISCONNECTED);
    }
}

void XmppClient::event(messages::Message& event) {
    messageListener.event(event);
}
//---------------------------------------------------------------------------
} // namespace espiot::xmpp
//---------------------------------------------------------------------------