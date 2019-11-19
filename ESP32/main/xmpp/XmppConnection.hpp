#pragma once

#include "XmppAccount.hpp"
#include "tcp/TcpConnection.hpp"
#include <smooth/core/Task.h>
#include <smooth/core/ipc/IEventListener.h>
#include <smooth/core/network/event/ConnectionStatusEvent.h>
#include <smooth/core/network/event/DataAvailableEvent.h>

//---------------------------------------------------------------------------
namespace espiot::xmpp {
//---------------------------------------------------------------------------
class XmppConnection : public smooth::core::ipc::IEventListener<smooth::core::network::event::ConnectionStatusEvent> {
    private:
    const XmppAccount* account;
    tcp::TcpConnection tcpConnection;
    smooth::core::Task& task;

    public:
    XmppConnection(const XmppAccount* account, smooth::core::Task& task);

    void connect();
    void disconnect();

    void event(const smooth::core::network::event::ConnectionStatusEvent& event);

    private:
    std::string genInitialStreamHeader();
};
//---------------------------------------------------------------------------
} // namespace espiot::xmpp
//---------------------------------------------------------------------------