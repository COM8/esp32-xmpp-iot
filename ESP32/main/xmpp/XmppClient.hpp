#pragma once

#include "XmppAccount.hpp"
#include "XmppConnection.hpp"

//---------------------------------------------------------------------------
namespace espiot::xmpp {
//---------------------------------------------------------------------------
class XmppClient {
    public:
    const XmppAccount account;
    XmppConnection connection;

    XmppClient(const XmppAccount&& account);

    void connect();
    void disconnect();
};
//---------------------------------------------------------------------------
} // namespace espiot::xmpp
//---------------------------------------------------------------------------