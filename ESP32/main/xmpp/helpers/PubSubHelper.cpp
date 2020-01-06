#include "PubSubHelper.hpp"
#include "../XmppUtils.hpp"
#include <iostream>
#include <vector>

//---------------------------------------------------------------------------
namespace espiot::xmpp::helpers {
//---------------------------------------------------------------------------
const std::string PubSubHelper::XMPP_IOT_SENSOR_TEMP = "xmpp.iot.sensor.temp";
const std::string PubSubHelper::XMPP_IOT_SENSOR_BAR = "xmpp.iot.sensor.bar";
const std::string PubSubHelper::XMPP_IOT_SENSORS = "xmpp.iot.sensors";
const std::string PubSubHelper::XMPP_IOT_ACTUATORS = "xmpp.iot.actuators";
const std::string PubSubHelper::XMPP_IOT_ACTUATOR_LED = "xmpp.iot.actuator.led";
const std::string PubSubHelper::XMPP_IOT_ACTUATOR_SPEAKER = "xmpp.iot.actuator.speaker";
const std::string PubSubHelper::XMPP_IOT_UI = "xmpp.iot.ui";
const std::string PubSubHelper::XMPP_IOT_NAMESPACE = "urn:xmpp:uwpx:iot";

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
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLElement* iqNode = doc.NewElement("iq");
    iqNode->SetAttribute("type", "get");
    iqNode->SetAttribute("from", client->account.jid.getFull().c_str());
    iqNode->SetAttribute("id", randFakeUuid().c_str());

    tinyxml2::XMLElement* queryNode = doc.NewElement("query");
    queryNode->SetAttribute("xmlns", "http://jabber.org/protocol/disco#items");
    iqNode->InsertEndChild(queryNode);

    tinyxml2::XMLPrinter printer;
    iqNode->Accept(&printer);
    return printer.CStr();
}

std::string PubSubHelper::genPublishUiNodeMessage() {
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLElement* itemNode = genPublishItemNode(doc, XMPP_IOT_UI.c_str(), "current");

    tinyxml2::XMLElement* xNode = doc.NewElement("x");
    xNode->SetAttribute("xmlns", "jabber:x:data");
    xNode->SetAttribute("xmlns:xdd", "urn:xmpp:xdata:dynamic");
    xNode->SetAttribute("type", "form");
    itemNode->InsertEndChild(xNode);

    tinyxml2::XMLElement* titleNode = doc.NewElement("title");
    titleNode->SetText("ESP32 XMPP");
    xNode->InsertEndChild(titleNode);

    tinyxml2::XMLElement* ledFieldNode = doc.NewElement("field");
    ledFieldNode->SetAttribute("type", "boolean");
    ledFieldNode->SetAttribute("label", "LED on?");
    ledFieldNode->SetAttribute("var", XMPP_IOT_ACTUATOR_LED.c_str());
    xNode->InsertEndChild(ledFieldNode);

    tinyxml2::XMLElement* speakerFieldNode = doc.NewElement("field");
    speakerFieldNode->SetAttribute("type", "boolean");
    speakerFieldNode->SetAttribute("label", "Speaker on?");
    speakerFieldNode->SetAttribute("var", XMPP_IOT_ACTUATOR_SPEAKER.c_str());
    xNode->InsertEndChild(speakerFieldNode);

    tinyxml2::XMLElement* tempFieldNode = doc.NewElement("field");
    tempFieldNode->SetAttribute("type", "text-single");
    tempFieldNode->SetAttribute("label", "Temperature: ");
    tempFieldNode->SetAttribute("var", XMPP_IOT_SENSOR_TEMP.c_str());
    xNode->InsertEndChild(tempFieldNode);
    tempFieldNode->InsertEndChild(doc.NewElement("xdd:readOnly"));

    tinyxml2::XMLElement* barFieldNode = doc.NewElement("field");
    barFieldNode->SetAttribute("type", "text-single");
    barFieldNode->SetAttribute("label", "Pressure: ");
    barFieldNode->SetAttribute("var", XMPP_IOT_SENSOR_BAR.c_str());
    xNode->InsertEndChild(barFieldNode);
    barFieldNode->InsertEndChild(doc.NewElement("xdd:readOnly"));

    tinyxml2::XMLPrinter printer;
    doc.FirstChild()->Accept(&printer);
    return printer.CStr();
}

tinyxml2::XMLElement* PubSubHelper::genPublishItemNode(tinyxml2::XMLDocument& doc, const char* nodeName, const char* itemId) {
    tinyxml2::XMLElement* iqNode = doc.NewElement("iq");
    iqNode->SetAttribute("type", "set");
    iqNode->SetAttribute("from", client->account.jid.getFull().c_str());
    iqNode->SetAttribute("id", randFakeUuid().c_str());
    doc.InsertEndChild(iqNode);

    tinyxml2::XMLElement* pubsubNode = doc.NewElement("pubsub");
    pubsubNode->SetAttribute("xmlns", "http://jabber.org/protocol/pubsub");
    iqNode->InsertEndChild(pubsubNode);

    // Publish node:
    tinyxml2::XMLElement* publishNode = doc.NewElement("publish");
    publishNode->SetAttribute("node", nodeName);
    pubsubNode->InsertEndChild(publishNode);

    tinyxml2::XMLElement* itemNode = doc.NewElement("item");
    itemNode->SetAttribute("id", itemId);
    publishNode->InsertEndChild(itemNode);

    // Publish options node:
    pubsubNode->InsertEndChild(genNodePublishConfig(doc));

    return itemNode;
}

std::string PubSubHelper::genPublishTempNodeItemMessage(double temp) {
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLElement* itemNode = genPublishItemNode(doc, XMPP_IOT_SENSORS.c_str(), XMPP_IOT_SENSOR_TEMP.c_str());
    tinyxml2::XMLElement* valNode = doc.NewElement("val");
    valNode->SetAttribute("xmlns", XMPP_IOT_NAMESPACE.c_str());
    valNode->SetAttribute("type", "double");
    valNode->SetAttribute("unit", "celsius");
    valNode->SetText(temp);
    itemNode->InsertEndChild(valNode);

    tinyxml2::XMLPrinter printer;
    doc.FirstChild()->Accept(&printer);
    return printer.CStr();
}

std::string PubSubHelper::genPublishPressureNodeItemMessage(int32_t pressure) {
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLElement* itemNode = genPublishItemNode(doc, XMPP_IOT_SENSORS.c_str(), XMPP_IOT_SENSOR_BAR.c_str());
    tinyxml2::XMLElement* valNode = doc.NewElement("val");
    valNode->SetAttribute("xmlns", XMPP_IOT_NAMESPACE.c_str());
    valNode->SetAttribute("type", "uint");
    valNode->SetAttribute("unit", "bar");
    valNode->SetText(pressure);
    itemNode->InsertEndChild(valNode);

    tinyxml2::XMLPrinter printer;
    doc.FirstChild()->Accept(&printer);
    return printer.CStr();
}

std::string PubSubHelper::genPublishLedNodeItemMessage(bool on) {
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLElement* itemNode = genPublishItemNode(doc, XMPP_IOT_ACTUATORS.c_str(), XMPP_IOT_ACTUATOR_LED.c_str());
    tinyxml2::XMLElement* valNode = doc.NewElement("val");
    valNode->SetAttribute("xmlns", XMPP_IOT_NAMESPACE.c_str());
    valNode->SetAttribute("type", "bool");
    valNode->SetText(on);
    itemNode->InsertEndChild(valNode);

    tinyxml2::XMLPrinter printer;
    doc.FirstChild()->Accept(&printer);
    return printer.CStr();
}

std::string PubSubHelper::genPublishSpeakerNodeItemMessage(bool on) {
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLElement* itemNode = genPublishItemNode(doc, XMPP_IOT_ACTUATORS.c_str(), XMPP_IOT_ACTUATOR_SPEAKER.c_str());
    tinyxml2::XMLElement* valNode = doc.NewElement("val");
    valNode->SetAttribute("xmlns", XMPP_IOT_NAMESPACE.c_str());
    valNode->SetAttribute("type", "bool");
    valNode->SetText(on);
    itemNode->InsertEndChild(valNode);

    tinyxml2::XMLPrinter printer;
    doc.FirstChild()->Accept(&printer);
    return printer.CStr();
}

tinyxml2::XMLElement* PubSubHelper::genFieldNode(tinyxml2::XMLDocument& doc, const char* var, const char* type, const char* value) {
    tinyxml2::XMLElement* fieldNode = doc.NewElement("field");
    fieldNode->SetAttribute("var", var);
    if (type) {
        fieldNode->SetAttribute("type", type);
    }

    tinyxml2::XMLElement* valueNode = doc.NewElement("value");
    valueNode->SetText(value);
    fieldNode->InsertEndChild(valueNode);
    return fieldNode;
}

std::string PubSubHelper::genRequestNodeConfigMessage(const std::string& nodeName) {
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLElement* iqNode = doc.NewElement("iq");
    iqNode->SetAttribute("type", "get");
    iqNode->SetAttribute("from", client->account.jid.getFull().c_str());
    iqNode->SetAttribute("id", randFakeUuid().c_str());

    tinyxml2::XMLElement* pubSubNode = doc.NewElement("pubsub");
    pubSubNode->SetAttribute("xmlns", "http://jabber.org/protocol/pubsub#owner");
    iqNode->InsertEndChild(pubSubNode);

    tinyxml2::XMLElement* configureNode = doc.NewElement("configure");
    configureNode->SetAttribute("node", nodeName.c_str());
    pubSubNode->InsertEndChild(configureNode);

    tinyxml2::XMLPrinter printer;
    iqNode->Accept(&printer);
    return printer.CStr();
}

tinyxml2::XMLElement* PubSubHelper::genNodePublishConfig(tinyxml2::XMLDocument& doc) {
    // https://xmpp.org/extensions/xep-0223.html
    tinyxml2::XMLElement* publishOptionsNode = doc.NewElement("publish-options");

    tinyxml2::XMLElement* xNode = doc.NewElement("x");
    xNode->SetAttribute("xmlns", "jabber:x:data");
    xNode->SetAttribute("type", "submit");
    publishOptionsNode->InsertEndChild(xNode);

    publishOptionsNode->InsertEndChild(genFieldNode(doc, "FORM_TYPE", "hidden", "http://jabber.org/protocol/pubsub#publish-options"));
    publishOptionsNode->InsertEndChild(genFieldNode(doc, "pubsub#persist_items", nullptr, "true"));
    publishOptionsNode->InsertEndChild(genFieldNode(doc, "pubsub#access_model", nullptr, "presence"));
    publishOptionsNode->InsertEndChild(genFieldNode(doc, "pubsub#publish_model", nullptr, "open")); // Perhaps replace with "subscribers"

    return publishOptionsNode;
}

void PubSubHelper::publishSensorsNode(double temp, int32_t pressure) {
    std::string msg = genPublishTempNodeItemMessage(temp);
    client->send(msg);
    msg = genPublishPressureNodeItemMessage(pressure);
    client->send(msg);
}

void PubSubHelper::onDiscoverNodesReply(messages::Message& event) {
    const tinyxml2::XMLDocument& doc = event.toXmlDoc();
    const tinyxml2::XMLElement* elem = doc.FirstChildElement("iq");
    if (!elem) {
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
    // Publish nodes anyway.
    // Don't care if the exist:

    // UI:
    std::string msg = genPublishUiNodeMessage();
    client->send(msg);

    // Sensors:
    msg = genPublishTempNodeItemMessage(0.0);
    client->send(msg);
    msg = genPublishPressureNodeItemMessage(0);
    client->send(msg);

    // Actuators:
    msg = genPublishLedNodeItemMessage(false);
    client->send(msg);
    msg = genPublishSpeakerNodeItemMessage(false);
    client->send(msg);

    /*if (std::find(nodes.begin(), nodes.end(), XMPP_IOT_UI) == nodes.end()) {
        std::string msg = genPublishUiNodeMessage();
        client->send(msg);
    }

    if (std::find(nodes.begin(), nodes.end(), XMPP_IOT_SENSORS) == nodes.end()) {
        std::string msg = genPublishSensorsNodeMessage(22.34, 42);
        client->send(msg);
    }

    if (std::find(nodes.begin(), nodes.end(), XMPP_IOT_ACTUATORS) == nodes.end()) {
        std::string msg = genPublishActuatorsNodeMessage();
        client->send(msg);
    }*/
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