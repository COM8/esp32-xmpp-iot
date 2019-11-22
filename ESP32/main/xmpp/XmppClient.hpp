#pragma once

#include "INonConstEventListener.hpp"
#include "XmppAccount.hpp"
#include "XmppConnection.hpp"
#include "messages/Message.hpp"
#include <string>
#include <vector>
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
                   public INonConstEventListener<messages::Message> {
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
    void sendMessage(const std::string& to, const std::string& body);

    using ConnectionStatusEventListener = smooth::core::ipc::IEventListener<XmppClientConnectionState>;
    ConnectionStatusEventListener& connectionStatusChanged;
    using MessageListener = INonConstEventListener<messages::Message>;
    std::vector<MessageListener*> messageListener;

    XmppClient(const XmppAccount&& account, smooth::core::Task& task, ConnectionStatusEventListener& connectionStatusChanged);

    void connect();
    void disconnect();

    bool isConnected();

    void subscribeToMessagesListener(MessageListener* messageListener);
    void unsubscribeFromMessagesListener(MessageListener* messageListener);

    void event(const XmppConnectionState& event) override;
    void event(messages::Message& event) override;
};
//---------------------------------------------------------------------------
} // namespace espiot::xmpp
//---------------------------------------------------------------------------