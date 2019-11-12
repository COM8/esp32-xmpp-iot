#include "XmppAccount.hpp"

//---------------------------------------------------------------------------
namespace espiot::xmpp {
//---------------------------------------------------------------------------
XmppAccount::XmppAccount(const Jid&& jid, const std::string&& password, const InetAddress_sp server, const uint16_t port) : jid(jid),
                                                                                                                            password(password),
                                                                                                                            server(server),
                                                                                                                            port(port) {}
//---------------------------------------------------------------------------
} // namespace espiot::xmpp
//---------------------------------------------------------------------------