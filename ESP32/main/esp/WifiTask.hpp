#pragma once

#include "RgbLed.hpp"
#include <smooth/core/Task.h>
#include <smooth/core/io/Output.h>
#include <smooth/core/ipc/IEventListener.h>
#include <smooth/core/ipc/SubscribingTaskEventQueue.h>
#include <smooth/core/network/NetworkStatus.h>
#include <smooth/core/network/Wifi.h>

//---------------------------------------------------------------------------
namespace espiot::esp {
//---------------------------------------------------------------------------
class WifiTask : public smooth::core::Task,
                 public smooth::core::ipc::IEventListener<smooth::core::network::NetworkStatus> {
    private:
    smooth::core::network::Wifi& wifi;
    RgbLed& rgbLed;

    using NetworkStatusQueue = smooth::core::ipc::SubscribingTaskEventQueue<smooth::core::network::NetworkStatus>;
    std::shared_ptr<NetworkStatusQueue> net_status;

    public:
    WifiTask(smooth::core::network::Wifi& wifi, RgbLed& rgbLed);

    void init() override;

    void event(const smooth::core::network::NetworkStatus& event) override;
};

//---------------------------------------------------------------------------
} // namespace espiot::esp
//---------------------------------------------------------------------------