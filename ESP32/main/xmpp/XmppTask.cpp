#include "XmppTask.hpp"
#include "xmpp/Jid.hpp"
#include "xmpp/XmppAccount.hpp"
#include "xmpp/XmppCredentials.hpp"
#include <smooth/core/network/event/ConnectionStatusEvent.h>
#include <smooth/core/task_priorities.h>

//---------------------------------------------------------------------------
namespace espiot::xmpp {
//---------------------------------------------------------------------------
using namespace smooth::core;
//---------------------------------------------------------------------------
XmppTask::XmppTask(esp::Storage& storage) : Task("XMPP Task", smooth::core::APPLICATION_BASE_PRIO, 0, std::chrono::seconds(1), 1),
                                            net_status(NetworkStatusQueue::create(2, *this, *this)),
                                            client(nullptr),
                                            storage(storage) {}

void XmppTask::init() {
    std::string jidString = storage.readString(esp::Storage::JID);
    xmpp::Jid jid = xmpp::Jid::fromBareJid(jidString);
    jid.print();
    std::string password = storage.readString(esp::Storage::JID_PASSWORD);
    xmpp::XmppAccount account(std::move(jid), std::move(password), std::make_shared<smooth::core::network::IPv4>(SERVER_IP, SERVER_PORT));
    client = std::make_unique<xmpp::XmppClient>(std::move(account));
}

void XmppTask::event(const network::NetworkStatus& event) {
    switch (event.get_event()) {
        case network::NetworkEvent::GOT_IP:
            if (client) {
                client->connect();
            }
            break;

        case network::NetworkEvent::DISCONNECTED:
            if (client) {
                client->disconnect();
            }
            break;

        default:
            break;
    }
}
//---------------------------------------------------------------------------
} // namespace espiot::xmpp
//---------------------------------------------------------------------------
