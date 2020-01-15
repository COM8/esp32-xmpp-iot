#pragma once

#include "INonConstEventListener.hpp"
#include "esp/Storage.hpp"
#include "esp/actuators/Relay.hpp"
#include "esp/actuators/Speaker.hpp"
#include "esp/sensors/Bmp180.hpp"
#include "esp/sensors/Mq2.hpp"
#include "esp/sensors/Photoresistor.hpp"
#include "helpers/PubSubHelper.hpp"
#include "messages/Message.hpp"
#include "xmpp/XmppClient.hpp"
#include <memory>
#include <smooth/core/Task.h>
#include <smooth/core/ipc/IEventListener.h>
#include <smooth/core/ipc/SubscribingTaskEventQueue.h>
#include <smooth/core/network/IPv4.h>
#include <smooth/core/network/NetworkStatus.h>

//---------------------------------------------------------------------------
namespace espiot::xmpp {
//---------------------------------------------------------------------------
class XmppTask : public smooth::core::Task,
                 public smooth::core::ipc::IEventListener<smooth::core::network::NetworkStatus>,
                 public smooth::core::ipc::IEventListener<XmppClientConnectionState>,
                 public INonConstEventListener<messages::Message> {
    private:
    using NetworkStatusQueue = smooth::core::ipc::SubscribingTaskEventQueue<smooth::core::network::NetworkStatus>;
    std::shared_ptr<NetworkStatusQueue> net_status;

    esp::Storage& storage;

#ifdef BMP180
    esp::sensors::Bmp180 bmp180;
#endif // BMP180
#ifdef MQ2
    esp::sensors::Mq2 mq2;
#endif // MQ2
#ifdef SPEAKER
    esp::actuators::Speaker speaker;
#endif // SPEAKER
#ifdef RELAY
    esp::actuators::Relay relay;
#endif // RELAY
#ifdef PHOTORESISTOR
    esp::sensors::Photoresistor photo;
#endif // PHOTORESISTOR

    std::shared_ptr<xmpp::XmppClient> client;
    std::unique_ptr<helpers::PubSubHelper> pubSubHelper;

    static const std::string INITIAL_HELLO_MESSAGE;

    void onReady();
    void handleIoTMessageMessage(const char* msg);

    public:
    XmppTask(esp::Storage& storage);
    ~XmppTask();

    void init() override;
    void tick() override;
    void handlePresenceMessages(const tinyxml2::XMLElement* elem);
    void handleMessageMessages(const tinyxml2::XMLElement* elem);

    void event(const smooth::core::network::NetworkStatus& event) override;
    void event(const XmppClientConnectionState& event) override;
    void event(messages::Message& event) override;
};
//---------------------------------------------------------------------------
} // namespace espiot::xmpp
//---------------------------------------------------------------------------