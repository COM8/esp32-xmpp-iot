#include "WifiTask.hpp"
#include "WiFiCredentials.hpp"
#include "driver/gpio.h"
#include <smooth/core/network/event/ConnectionStatusEvent.h>
#include <smooth/core/task_priorities.h>

//---------------------------------------------------------------------------
namespace espiot::esp {
//---------------------------------------------------------------------------
using namespace smooth::core;
//---------------------------------------------------------------------------
WifiTask::WifiTask(network::Wifi& wifi, RgbLed& rgbLed) : Task("WIFI Task", smooth::core::APPLICATION_BASE_PRIO, 0, std::chrono::seconds(1), 1),
                                                          wifi(wifi),
                                                          rgbLed(rgbLed),
                                                          net_status(NetworkStatusQueue::create("", 2, *this, *this)) {}

void WifiTask::init() {
    printf("Scanning for wifi...\n");
    rgbLed.turnOnOnly(rgbLed.b);
    wifi.set_host_name("Smooth-ESP");
    wifi.set_auto_connect(true);
    wifi.set_ap_credentials(SSID, PASSWORD);
    wifi.connect_to_ap();
}

void WifiTask::event(const network::NetworkStatus& event) {
    switch (event.get_event()) {
        case network::NetworkEvent::GOT_IP:
            rgbLed.turnOnOnly(rgbLed.g);
            printf("WIFI: Got IP!\n");
            break;

        case network::NetworkEvent::DISCONNECTED:
            rgbLed.turnOnOnly(rgbLed.b);
            printf("WIFI: Disconnected!\n");
            break;

        default:
            break;
    }
}
//---------------------------------------------------------------------------
} // namespace espiot::esp
//---------------------------------------------------------------------------
