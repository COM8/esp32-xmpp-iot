#pragma once

#include "esp/Storage.hpp"
#include "esp/WifiTask.hpp"
#include "esp/actuators/Button.hpp"
#include "esp/actuators/RgbLed.hpp"
#include "esp/bt/BluetoothServer.hpp"
#include "esp/bt/BluetoothServerCallback.hpp"
#include "esp/sensors/Bmp180.hpp"
#include "xmpp/XmppTask.hpp"
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
    esp::actuators::RgbLed rgbLed;
    esp::actuators::Button resetButton;
    esp::Storage storage;
    esp::WifiTask wifiTask;
    esp::bt::BluetoothServer btServer;
    xmpp::XmppTask xmppTask;

    void initWithDummyValues();

    public:
    EspIoT();

    void init() override;
    void tick() override;
    void onConfigurationDone(std::string& wifiSsid, std::string& wifiPassword, std::string& jid, std::string& jidPassword, std::string& jidSender) override;
};
//---------------------------------------------------------------------------
} // namespace espiot
//---------------------------------------------------------------------------
