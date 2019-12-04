#pragma once

#include "../INonConstEventListener.hpp"
#include "../XmppClient.hpp"
#include "../messages/Message.hpp"
#include <tinyxml2.h>

//---------------------------------------------------------------------------
namespace espiot::xmpp::helpers {
//---------------------------------------------------------------------------
enum PubSubHelperState {
    PUB_SUB_HELPER_NOT_STARTED,
    PUB_SUB_HELPER_REQUESTING_NODES,
    PUB_SUB_HELPER_CREATING_NODES,
    PUB_SUB_HELPER_DONE
};

class PubSubHelper : public INonConstEventListener<messages::Message> {
    private:
    std::shared_ptr<xmpp::XmppClient> client;
    PubSubHelperState state;

    void setState(PubSubHelperState state);

    void discoverNodes();
    void createNodes();
    void onDiscoverNodesReply(messages::Message& event);
    void onCreateNodeReply(messages::Message& event);

    std::string genDiscoverNodesMessage();
    std::string genPublishUiNodeMessage();
    std::string genPublishTempNodeItemMessage(double temp);
    std::string genPublishPressureNodeItemMessage(int32_t pressure);
    std::string genPublishLedNodeItemMessage(bool on);
    std::string genPublishSpeakerNodeItemMessage(bool on);
    tinyxml2::XMLElement* genFieldNode(tinyxml2::XMLDocument& doc, const char* var, const char* type, const char* value);
    tinyxml2::XMLElement* genNodePublishConfig(tinyxml2::XMLDocument& doc);
    tinyxml2::XMLElement* genPublishItemNode(tinyxml2::XMLDocument& doc, const char* nodeName, const char* itemId);

    public:
    static const std::string XMPP_IOT_SENSOR_TEMP;
    static const std::string XMPP_IOT_SENSOR_BAR;
    static const std::string XMPP_IOT_SENSORS;
    static const std::string XMPP_IOT_ACTUATORS;
    static const std::string XMPP_IOT_ACTUATOR_LED;
    static const std::string XMPP_IOT_ACTUATOR_SPEAKER;
    static const std::string XMPP_IOT_UI;
    static const std::string XMPP_IOT_NAMESPACE;

    void publishSensorsNode(double temp, int32_t pressure);

    PubSubHelper(std::shared_ptr<xmpp::XmppClient> client);

    PubSubHelperState getState();

    void start();

    void event(messages::Message& event) override;
};
//---------------------------------------------------------------------------
} // namespace espiot::xmpp::helpers
//---------------------------------------------------------------------------