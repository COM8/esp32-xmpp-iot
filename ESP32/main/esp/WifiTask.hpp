#pragma once

#include "LedHandler.hpp"
#include <smooth/core/Task.h>
#include <smooth/core/ipc/IEventListener.h>
#include <smooth/core/network/NetworkStatus.h>
#include <smooth/core/network/Wifi.h>

//---------------------------------------------------------------------------
namespace espiot::esp {
//---------------------------------------------------------------------------
class WifiTask : public smooth::core::Task,
                 public smooth::core::ipc::IEventListener<smooth::core::network::NetworkStatus> {
    private:
    smooth::core::network::Wifi& wifi;
    LedHandler& ledHandler;

    public:
    WifiTask(smooth::core::network::Wifi& wifi, LedHandler& ledHandler);

    void init() override;

    void event(const smooth::core::network::NetworkStatus& event) override;
};

//---------------------------------------------------------------------------
} // namespace espiot::esp
//---------------------------------------------------------------------------