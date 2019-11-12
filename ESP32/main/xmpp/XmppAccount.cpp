#include "XmppAccount.hpp"

//---------------------------------------------------------------------------
namespace espiot::xmpp {
//---------------------------------------------------------------------------
XmppAccount::XmppAccount(const Jid&& jid, const std::string&& password, const InetAddress_sp server) : jid(jid),
                                                                                                       password(password),
                                                                                                       server(server) {}
//---------------------------------------------------------------------------
} // namespace espiot::xmpp
//---------------------------------------------------------------------------