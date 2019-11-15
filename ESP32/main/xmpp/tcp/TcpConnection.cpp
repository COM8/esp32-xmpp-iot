#include "TcpConnection.hpp"
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
    }
}

void TcpConnection::disconnect() {
}

void TcpConnection::event(const smooth::core::network::event::TransmitBufferEmptyEvent&) {
}

void TcpConnection::event(const smooth::core::network::event::DataAvailableEvent<XmppProtocol>&) {
}

void TcpConnection::event(const smooth::core::network::event::ConnectionStatusEvent&) {
}

void TcpConnection::event(const smooth::core::network::NetworkStatus& event) {
}
//---------------------------------------------------------------------------
} // namespace espiot::xmpp::tcp
//---------------------------------------------------------------------------