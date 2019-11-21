#include "XmppTask.hpp"
#include "../esp/Storage.hpp"
#include "xmpp/Jid.hpp"
#include "xmpp/XmppAccount.hpp"
#include "xmpp/XmppCredentials.hpp"
#include <iostream>
#include <smooth/core/network/event/ConnectionStatusEvent.h>
#include <smooth/core/task_priorities.h>
#include <tinyxml2.h>

//---------------------------------------------------------------------------
namespace espiot::xmpp {
//---------------------------------------------------------------------------
using namespace smooth::core;
//---------------------------------------------------------------------------
const std::string INITIAL_HELLO_MESSAGE = "Hi from the ESP32. Please mirror this message!";

XmppTask::XmppTask(esp::Storage& storage) : Task("XMPP Task", 4096, smooth::core::APPLICATION_BASE_PRIO, std::chrono::seconds(1), 1),
                                            net_status(NetworkStatusQueue::create(2, *this, *this)),
                                            client(nullptr),
                                            storage(storage) {}

void XmppTask::init() {
    std::string jidString = storage.readString(esp::Storage::JID);
    xmpp::Jid jid = xmpp::Jid::fromFullJid(jidString);
    jid.print();
    std::string password = storage.readString(esp::Storage::JID_PASSWORD);
    xmpp::XmppAccount account(std::move(jid), std::move(password), std::make_shared<smooth::core::network::IPv4>(SERVER_IP, SERVER_PORT));
    client = std::make_unique<xmpp::XmppClient>(std::move(account), *this, *this, *this);
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

void XmppTask::event(const XmppClientConnectionState& event) {
    if (event == CLIENT_CONNECTED) {
        if (!storage.readBool(esp::Storage::SETUP_DONE)) {
            std::string to = storage.readString(esp::Storage::JID_SENDER);
            client->sendMessage(to, INITIAL_HELLO_MESSAGE);
        }
    }
}

void XmppTask::event(messages::Message& event) {
    const tinyxml2::XMLDocument& doc = event.toXmlDoc();
    const tinyxml2::XMLElement* elem = doc.FirstChildElement("message");
    if (elem) {
        elem = elem->FirstChildElement("body");
        if (elem) {
            if (!strcmp(INITIAL_HELLO_MESSAGE.c_str(), elem->GetText())) {
                std::string to = storage.readString(esp::Storage::JID_SENDER);
                std::string body = "Setup done!";
                client->sendMessage(to, body);
                storage.writeBool(esp::Storage::SETUP_DONE, true);
                std::cout << "Setup done!\n";
            }
        }
    }
}
//---------------------------------------------------------------------------
} // namespace espiot::xmpp
//---------------------------------------------------------------------------
