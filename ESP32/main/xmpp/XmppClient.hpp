#pragma once

#include "XmppAccount.hpp"
#include "XmppConnection.hpp"
#include <smooth/core/Task.h>

//---------------------------------------------------------------------------
namespace espiot::xmpp {
//---------------------------------------------------------------------------
class XmppClient {
    public:
    const XmppAccount account;
    XmppConnection connection;
    smooth::core::Task& task;

    XmppClient(const XmppAccount&& account, smooth::core::Task& task);

    void connect();
    void disconnect();
};
//---------------------------------------------------------------------------
} // namespace espiot::xmpp
//---------------------------------------------------------------------------