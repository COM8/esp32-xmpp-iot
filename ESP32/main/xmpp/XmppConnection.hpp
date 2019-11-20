#pragma once

#include "XmppAccount.hpp"
#include "tcp/TcpConnection.hpp"
#include "tcp/XmppPacket.hpp"
#include <smooth/core/Task.h>
#include <smooth/core/ipc/IEventListener.h>
#include <smooth/core/network/event/ConnectionStatusEvent.h>
#include <smooth/core/network/event/DataAvailableEvent.h>

//---------------------------------------------------------------------------
namespace espiot::xmpp {
//---------------------------------------------------------------------------
enum XmppConnectionState {
    DISCONNECTED,
    CONNECTING,
    INITIAL_STREAM_HEADER_SEND,
    SASL_PLAIN_AUTH_SEND,
    SASL_SOFT_STREAM_RESET_SEND,
    RESOURCE_BINDING_REQUEST_SEND,
    CONNECTED,
    DISCONNECTING,
    ERROR
};

class XmppConnection : public smooth::core::ipc::IEventListener<smooth::core::network::event::ConnectionStatusEvent>,
                       public smooth::core::ipc::IEventListener<tcp::XmppPacket> {
    private:
    const XmppAccount* account;
    tcp::TcpConnection tcpConnection;
    smooth::core::Task& task;

    XmppConnectionState state;

    using StateChangedListener = smooth::core::ipc::IEventListener<XmppConnectionState>;
    StateChangedListener& stateChangedListener;

    void
    setState(XmppConnectionState state);
    [[nodiscard]] XmppConnectionState getState() const;

    public:
    XmppConnection(const XmppAccount* account, smooth::core::Task& task, StateChangedListener& stateChangedListener);

    void connect();
    void disconnect();

    void event(const smooth::core::network::event::ConnectionStatusEvent& event);
    void event(const tcp::XmppPacket& event);

    private:
    std::string genInitialStreamHeader();
    std::string genPlainAuthMessage();
    std::string genResourceBindMessage();
    std::string genPresenceMessage();
    std::string genMessageMessage(std::string& to, std::string& body);

    void onInitialStreamHeaderReply(const tcp::XmppPacket& packet);
    void onSaslAuthMessageReply(const tcp::XmppPacket& packet);
    void onSaslAuthRestartStreamHeaderReply(const tcp::XmppPacket& packet);
    void onResourceBindingReply(const tcp::XmppPacket& packet);
};
//---------------------------------------------------------------------------
} // namespace espiot::xmpp
//---------------------------------------------------------------------------