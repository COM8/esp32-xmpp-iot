#pragma once

#include "../XmppAccount.hpp"
#include <smooth/application/network/http/IResponseOperation.h>
#include <smooth/core/ipc/IEventListener.h>
#include <smooth/core/network/Socket.h>
#include <smooth/core/network/event/ConnectionStatusEvent.h>

//---------------------------------------------------------------------------
namespace espiot::xmpp::tcp {
//---------------------------------------------------------------------------
class TcpConnection : public smooth::core::ipc::IEventListener<smooth::core::network::event::TransmitBufferEmptyEvent>,
                      public smooth::core::ipc::IEventListener<smooth::core::network::event::DataAvailableEvent<Proto>>,
                      public smooth::core::ipc::IEventListener<smooth::core::network::event::ConnectionStatusEvent>,
                      public smooth::core::ipc::IEventListener<smooth::core::network::NetworkStatus> {
    private:
    const XmppAccount* account;

    std::shared_ptr<smooth::core::network::Socket> socket;

    public:
    TcpConnection(const XmppAccount* account);

    void event(const smooth::core::network::event::TransmitBufferEmptyEvent&) override;
    void event(const smooth::core::network::event::DataAvailableEvent<Proto>&) override;
    void event(const smooth::core::network::event::ConnectionStatusEvent&) override;
    void event(const smooth::core::network::NetworkStatus& event) override;

    void connect();
    void disconnect();
};
//---------------------------------------------------------------------------
} // namespace espiot::xmpp::tcp
//---------------------------------------------------------------------------