#include "TcpConnection.hpp"
#include <iostream>
#include <smooth/core/Task.h>

//---------------------------------------------------------------------------
namespace espiot::xmpp::tcp {
//---------------------------------------------------------------------------
TcpConnection::TcpConnection(const XmppAccount* account, smooth::core::Task& task) : account(account),
                                                                                     task(task),
                                                                                     buffer(nullptr),
                                                                                     socket(nullptr) {}

void TcpConnection::connect() {
    if (!socket) {
        std::unique_ptr<XmppProtocol> protocol = std::make_unique<XmppProtocol>();
        buffer = std::make_shared<smooth::core::network::BufferContainer<XmppProtocol>>(task, *this, *this, *this, std::move(protocol));

        socket = smooth::core::network::Socket<XmppProtocol>::create(buffer);
        socket->start(account->server);

        std::string msg = "<stream:stream from=’test1@xmpp.uwpx.org’ to=’xmpp.uwpx.org’ version=’1.0’ xml:lang=’en’ xmlns=’jabber:client’ xmlns:stream=’http://etherx.jabber.org/streams’>";
        if (send(msg)) {
            std::cout << "Send was successfull!" << std::endl;
        } else {
            std::cout << "Send was unsuccessfull!" << std::endl;
        }
    }
}

bool TcpConnection::send(std::string& msg) {
    return socket->send(static_cast<XmppPacket>(msg));
}

bool TcpConnection::send(std::wstring& msg) {
    return socket->send(static_cast<XmppPacket>(msg));
}

void TcpConnection::disconnect() {}

void TcpConnection::event(const smooth::core::network::event::TransmitBufferEmptyEvent&) {
    std::cout << "EVENT TransmitBufferEmptyEvent" << std::endl;
}

void TcpConnection::event(const smooth::core::network::event::DataAvailableEvent<XmppProtocol>&) {
    std::cout << "EVENT DataAvailableEvent" << std::endl;
}

void TcpConnection::event(const smooth::core::network::event::ConnectionStatusEvent&) {
    std::cout << "EVENT ConnectionStatusEvent" << std::endl;
}

void TcpConnection::event(const smooth::core::network::NetworkStatus& event) {
    std::cout << "EVENT NetworkStatus" << std::endl;
}
//---------------------------------------------------------------------------
} // namespace espiot::xmpp::tcp
//---------------------------------------------------------------------------