#include "XmppTask.hpp"
#include "../esp/Storage.hpp"
#include "xmpp/Jid.hpp"
#include "xmpp/XmppAccount.hpp"
#include "xmpp/XmppCredentials.hpp"
#include <iostream>
#include <string>
#include <smooth/core/network/event/ConnectionStatusEvent.h>
#include <smooth/core/task_priorities.h>
#include <tinyxml2.h>

//---------------------------------------------------------------------------
namespace espiot::xmpp {
//---------------------------------------------------------------------------
using namespace smooth::core;
//---------------------------------------------------------------------------
const std::string XmppTask::INITIAL_HELLO_MESSAGE = "Hi from the ESP32. Please mirror this message!";

XmppTask::XmppTask(esp::Storage& storage) : Task("XMPP Task", 4096, smooth::core::APPLICATION_BASE_PRIO, std::chrono::seconds(3), 1),
                                            net_status(NetworkStatusQueue::create(2, *this, *this)),
                                            storage(storage),
#ifdef BMP180
                                            bmp180(GPIO_NUM_32, GPIO_NUM_33),
#endif // BMP180
#ifdef MQ2
                                            mq2(),
#endif // MQ2
#ifdef SPEAKER
                                            speaker(GPIO_NUM_13),
#endif // SPEAKER
#ifdef RELAY
                                            relay(GPIO_NUM_32),
#endif // RELAY
#ifdef PHOTORESISTOR
                                            photo(),
#endif
                                            client(nullptr),
                                            pubSubHelper(nullptr) {
}

XmppTask::~XmppTask() {
    if (client) {
        client->unsubscribeFromMessagesListener(this);
    }
}

void XmppTask::init() {
    std::string jidString = storage.readString(esp::Storage::JID);
    xmpp::Jid jid = xmpp::Jid::fromFullJid(jidString);
    std::string password = storage.readString(esp::Storage::JID_PASSWORD);
    xmpp::XmppAccount account(std::move(jid), std::move(password), std::make_shared<smooth::core::network::IPv4>(SERVER_IP, SERVER_PORT));
    client = std::make_unique<xmpp::XmppClient>(std::move(account), *this, *this);
    client->subscribeToMessagesListener(this);

    // Helpers:
    pubSubHelper = std::make_unique<helpers::PubSubHelper>(client);
}

void XmppTask::tick() {
    if (client->isConnected()) {
#ifdef BMP180
        double temp = bmp180.readTemp();
        std::cout << "Temp: " << temp << "\n";
        pubSubHelper->publishTempNode(temp);

        int32_t pressure = bmp180.readPressure();
        std::cout << "Pressure: " << pressure << "\n";
        pubSubHelper->publishPressureNode(pressure);
#endif // BMP180
#ifdef MQ2
        int32_t mq2Val = mq2.read();
        std::cout << "MQ2: " << mq2Val << "\n";
        pubSubHelper->publishMq2Node(mq2Val);
#endif // MQ2
#ifdef RELAY
        bool relayOn = relay.toggle();
        std::cout << "RELAY: " << relayOn << "\n";
        pubSubHelper->publishRelayNode(relayOn);
#endif // RELAY
#ifdef PHOTORESISTOR
        int32_t photoVal = photo.read();
        std::cout << "Photoresistor: " << photoVal << "\n";
        pubSubHelper->publishPhotoresistorNode(photoVal);
#endif // PHOTORESISTOR
    }
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

void XmppTask::onReady() {
    pubSubHelper->start();
}

void XmppTask::handlePresenceMessages(const tinyxml2::XMLElement* elem) {
    const tinyxml2::XMLAttribute* attrib = elem->FindAttribute("type");
    // For now approve all presence subscription requests:
    if (attrib && !strcmp(attrib->Value(), "subscribe")) {
        attrib = elem->FindAttribute("from");
        std::string bareJid = attrib->Value();
        client->approvePresenceSubscription(bareJid);
    }
}

void XmppTask::handleMessageMessages(const tinyxml2::XMLElement* elem) {
    elem = elem->FirstChildElement("body");
    if (elem) {
        if (!storage.readBool(esp::Storage::SETUP_DONE)) {
            if (!strcmp(INITIAL_HELLO_MESSAGE.c_str(), elem->GetText())) {
                std::string to = storage.readString(esp::Storage::JID_SENDER);

                // Add to roster:
                client->addToRoster(to);

                // Send setup done message:
                std::string body = "Setup done!";
                client->sendMessage(to, body);
                storage.writeBool(esp::Storage::SETUP_DONE, true);
                std::cout << "Setup done!\n";
                onReady();
            }
        } else {
            handleIoTMessageMessage(elem->GetText());
        }
    } else if ((elem = elem->FirstChildElement("event"))) {
        if ((elem = elem->FirstChildElement("items"))) {
            handlePubSubEventMessage(elem);
        }
    }
}

void XmppTask::handlePubSubEventMessage(const tinyxml2::XMLElement* elem) {
    const tinyxml2::XMLAttribute* nodeAttrib = elem->FindAttribute("node");
    if (nodeAttrib && (elem = elem->FirstChildElement("item"))) {
        const tinyxml2::XMLAttribute* idAttrib = elem->FindAttribute("id");
        if (idAttrib && (elem = elem->FirstChildElement("val"))) {
            if (!strcmp(nodeAttrib->Value(), pubSubHelper->XMPP_IOT_ACTUATORS.c_str())) {
                if (!strcmp(idAttrib->Value(), pubSubHelper->XMPP_IOT_ACTUATOR_RELAY.c_str())) {
#ifdef RELAY
                    std::string val = elem->GetText();
                    relay.set(val == "1");
                    std::cout << "Relay value updated to: " << val << "\n";
#endif // RELAY
                } else if (!strcmp(idAttrib->Value(), pubSubHelper->XMPP_IOT_ACTUATOR_SPEAKER.c_str())) {
#ifdef SPEAKER
                    std::string val = elem->GetText();
                    speaker.set(val == "1");
                    std::cout << "Speaker value updated to: " << val << "\n";
#endif // SPEAKER
                }
            }
        }
    }
}

void XmppTask::handleIoTMessageMessage(const char* msg) {
#ifdef SPEAKER
    if (!strcmp("set speaker 1", msg)) {
        // speaker.set(1000);
    } else if (!strcmp("set speaker 0", msg)) {
        // speaker.set(0);
    }
#endif // SPEAKER
}

void XmppTask::event(const XmppClientConnectionState& event) {
    if (event == CLIENT_CONNECTED) {
        if (!storage.readBool(esp::Storage::SETUP_DONE)) {
            std::string to = storage.readString(esp::Storage::JID_SENDER);
            client->sendMessage(to, INITIAL_HELLO_MESSAGE);
        } else {
            onReady();
        }
    }
}

void XmppTask::event(messages::Message& event) {
    const tinyxml2::XMLDocument& doc = event.toXmlDoc();
    const tinyxml2::XMLElement* elem = nullptr;
    if ((elem = doc.FirstChildElement("message"))) {
        handleMessageMessages(elem);
    } else if ((elem = doc.FirstChildElement("presence"))) {
        handlePresenceMessages(elem);
    }
}
//---------------------------------------------------------------------------
} // namespace espiot::xmpp
//---------------------------------------------------------------------------
