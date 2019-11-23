#pragma once

#include "../INonConstEventListener.hpp"
#include "../XmppClient.hpp"
#include "../messages/Message.hpp"

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
    std::string genCreateNodesMessage(const std::string& nodeName);

    public:
    static const std::string XMPP_IOT_SENSOR_TEMP;
    static const std::string XMPP_IOT_SENSOR_BAR;

    // void publishNode(std::string& nodeName, std::string& val);

    PubSubHelper(std::shared_ptr<xmpp::XmppClient> client);

    PubSubHelperState getState();

    void start();

    void event(messages::Message& event) override;
};
//---------------------------------------------------------------------------
} // namespace espiot::xmpp::helpers
//---------------------------------------------------------------------------