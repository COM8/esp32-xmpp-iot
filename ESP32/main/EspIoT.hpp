#pragma once

#include "esp/Button.hpp"
#include "esp/RgbLed.hpp"
#include "esp/Storage.hpp"
#include "esp/WifiTask.hpp"
#include "esp/bt/BluetoothServer.hpp"
#include "esp/bt/BluetoothServerCallback.hpp"
#include "esp/sensors/Bmp180.hpp"
#include <string>
#include <smooth/core/Application.h>

//---------------------------------------------------------------------------
namespace espiot {
//---------------------------------------------------------------------------
#if defined(__cplusplus)
extern "C" {
#endif

void app_main(void);

#if defined(__cplusplus)
}
#endif
//---------------------------------------------------------------------------
class EspIoT : public smooth::core::Application, public esp::bt::BluetoothServerCallback {
    private:
    esp::RgbLed rgbLed;
    esp::Button resetButton;
    esp::Storage storage;
    esp::WifiTask wifiTask;
    esp::sensors::Bmp180 bmp180;
    esp::bt::BluetoothServer btServer;

    public:
    EspIoT();

    void init() override;
    void tick() override;
    void onConfigurationDone(std::string& wifiSsid, std::string& wifiPassword, std::string& jid, std::string& jidPassword) override;
};
//---------------------------------------------------------------------------
} // namespace espiot
//---------------------------------------------------------------------------
