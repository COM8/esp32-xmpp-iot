#pragma once

#include "XmppAccount.hpp"
#include "XmppConnection.hpp"
#include "tcp/XmppPacket.hpp"
#include <string>
#include <smooth/core/Task.h>
#include <smooth/core/ipc/IEventListener.h>
#include <smooth/core/network/event/ConnectionStatusEvent.h>

//---------------------------------------------------------------------------
namespace espiot::xmpp {
//---------------------------------------------------------------------------
enum XmppClientConnectionState {
    CLIENT_DISCONNECTED,
    CLIENT_CONNECTING,
    CLIENT_CONNECTED,
    CLIENT_DISCONNECTING
};

class XmppClient : public smooth::core::ipc::IEventListener<XmppConnectionState>,
                   public smooth::core::ipc::IEventListener<tcp::XmppPacket> {
    private:
    XmppClientConnectionState state;

    void setState(XmppClientConnectionState state);
    XmppClientConnectionState getState();

    public:
    const XmppAccount account;
    XmppConnection connection;
    smooth::core::Task& task;

    void send(std::string& msg);
    void send(std::wstring& msg);
    void sendMessage(std::string& to, std::string& body);

    using ConnectionStatusEventListener = smooth::core::ipc::IEventListener<XmppClientConnectionState>;
    ConnectionStatusEventListener& connectionStatusChanged;
    using XmppPacketAvailableListener = smooth::core::ipc::IEventListener<tcp::XmppPacket>;
    XmppPacketAvailableListener& xmppPacketAvailable;

    XmppClient(const XmppAccount&& account, smooth::core::Task& task, ConnectionStatusEventListener& connectionStatusChanged, XmppPacketAvailableListener& xmppPacketAvailable);

    void connect();
    void disconnect();

    bool isConnected();

    void event(const XmppConnectionState& event) override;
    void event(const tcp::XmppPacket& event) override;
};
//---------------------------------------------------------------------------
} // namespace espiot::xmpp
//---------------------------------------------------------------------------