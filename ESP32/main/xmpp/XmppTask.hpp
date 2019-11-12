#pragma once

#include "esp/Storage.hpp"
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
                 public smooth::core::ipc::IEventListener<smooth::core::network::NetworkStatus> {
    private:
    using NetworkStatusQueue = smooth::core::ipc::SubscribingTaskEventQueue<smooth::core::network::NetworkStatus>;
    std::shared_ptr<NetworkStatusQueue> net_status;

    using XmppClient_up = std::unique_ptr<xmpp::XmppClient>;
    XmppClient_up client;

    esp::Storage& storage;

    public:
    XmppTask(esp::Storage& storage);

    void init() override;

    void event(const smooth::core::network::NetworkStatus& event) override;
};
//---------------------------------------------------------------------------
} // namespace espiot::xmpp
//---------------------------------------------------------------------------