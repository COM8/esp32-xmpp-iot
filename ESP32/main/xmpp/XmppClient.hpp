#pragma once

#include "XmppAccount.hpp"
#include "XmppConnection.hpp"
#include <smooth/core/Task.h>
#include <smooth/core/ipc/IEventListener.h>

//---------------------------------------------------------------------------
namespace espiot::xmpp {
//---------------------------------------------------------------------------
class XmppClient : public smooth::core::ipc::IEventListener<XmppConnectionState> {
    public:
    const XmppAccount account;
    XmppConnection connection;
    smooth::core::Task& task;

    XmppClient(const XmppAccount&& account, smooth::core::Task& task);

    void connect();
    void disconnect();

    void event(const XmppConnectionState& event);
};
//---------------------------------------------------------------------------
} // namespace espiot::xmpp
//---------------------------------------------------------------------------