#include "PubSubHelper.hpp"
#include "../XmppUtils.hpp"
#include <iostream>
#include <vector>
#include <tinyxml2.h>

//---------------------------------------------------------------------------
namespace espiot::xmpp::helpers {
//---------------------------------------------------------------------------
const std::string PubSubHelper::XMPP_IOT_SENSOR_TEMP = "xmpp.iot.sensor.temp";
const std::string PubSubHelper::XMPP_IOT_SENSOR_BAR = "xmpp.iot.sensor.bar";

PubSubHelper::PubSubHelper(std::shared_ptr<xmpp::XmppClient> client) : client(client),
                                                                       state(PUB_SUB_HELPER_NOT_STARTED) {}

void PubSubHelper::setState(PubSubHelperState state) {
    if (this->state != state) {
        this->state = state;
        // TODO trigger some kind of event
    }
}

PubSubHelperState PubSubHelper::getState() {
    return state;
}

void PubSubHelper::start() {
    client->subscribeToMessagesListener(this);
    discoverNodes();
}

void PubSubHelper::discoverNodes() {
    std::string msg = genDiscoverNodesMessage();
    setState(PUB_SUB_HELPER_REQUESTING_NODES);
    client->send(msg);
}

void PubSubHelper::createNodes() {
}

std::string PubSubHelper::genDiscoverNodesMessage() {
    return "<iq type='get' from='" + client->account.jid.getFull() + "' to='pubsub." + client->account.jid.domainPart + "' id='" + randFakeUuid() + "'><query xmlns='http://jabber.org/protocol/disco#items'/></iq>";
}

std::string PubSubHelper::genCreateNodesMessage(const std::string& nodeName) {
    return "<iq type='set' from='" + client->account.jid.getFull() + "' id='" + randFakeUuid() + "'><pubsub xmlns='http://jabber.org/protocol/pubsub'><publish node='" + nodeName + "'><item id='current'><temp>25.44</temp></item></publish></pubsub></iq>";
}

void PubSubHelper::onDiscoverNodesReply(messages::Message& event) {
    const tinyxml2::XMLDocument& doc = event.toXmlDoc();
    const tinyxml2::XMLElement* elem = doc.FirstChildElement("iq");
    if (!elem) {
        std::cout << "1111" << std::endl;
        return;
    }
    elem = elem->FirstChildElement("query");
    if (!elem) {
        return;
    }
    std::vector<std::string> nodes;
    for (const tinyxml2::XMLElement* e = elem->FirstChildElement("item"); e != nullptr; e = e->NextSiblingElement("item")) {
        const char* val = e->Attribute("node");
        if (val) {
            nodes.push_back(std::string(val));
        }
    }

    if (std::find(nodes.begin(), nodes.end(), XMPP_IOT_SENSOR_TEMP) == nodes.end()) {
        std::string msg = genCreateNodesMessage(XMPP_IOT_SENSOR_TEMP);
        client->send(msg);
    }

    if (std::find(nodes.begin(), nodes.end(), XMPP_IOT_SENSOR_BAR) == nodes.end()) {
        std::string msg = genCreateNodesMessage(XMPP_IOT_SENSOR_BAR);
        client->send(msg);
    }
}

void PubSubHelper::onCreateNodeReply(messages::Message& event) {
}

void PubSubHelper::event(messages::Message& event) {
    switch (state) {
        case PUB_SUB_HELPER_REQUESTING_NODES:
            onDiscoverNodesReply(event);
            break;

        case PUB_SUB_HELPER_CREATING_NODES:
            onCreateNodeReply(event);
            break;

        default:
            break;
    }
}
//---------------------------------------------------------------------------
} // namespace espiot::xmpp::helpers
//---------------------------------------------------------------------------