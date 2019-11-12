#include "XmppTask.hpp"
#include "xmpp/Jid.hpp"
#include "xmpp/XmppAccount.hpp"
#include <smooth/core/network/event/ConnectionStatusEvent.h>
#include <smooth/core/task_priorities.h>

//---------------------------------------------------------------------------
namespace espiot::xmpp {
//---------------------------------------------------------------------------
using namespace smooth::core;
//---------------------------------------------------------------------------
XmppTask::XmppTask(Storage& storage) : Task("XMPP Task", smooth::core::APPLICATION_BASE_PRIO, 0, std::chrono::seconds(1), 1),
                                       net_status(NetworkStatusQueue::create(2, *this, *this)),
                                       client(nullptr),
                                       storage(storage) {}

void XmppTask::init() {
    storage.readString(esp::Storage::JID);
    xmpp::Jid jid();
    xmpp::XmppAccount account();
    client = std::make_unique<xmpp::XmppClient>(std::move(account));
}

void XmppTask::event(const network::NetworkStatus& event) {
    switch (event.get_event()) {
        case network::NetworkEvent::GOT_IP:
            break;

        case network::NetworkEvent::DISCONNECTED:
            break;

        default:
            break;
    }
}
//---------------------------------------------------------------------------
} // namespace espiot::xmpp
//---------------------------------------------------------------------------
