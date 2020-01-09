#include "WifiTask.hpp"
#include "WiFiCredentials.hpp"
#include "driver/gpio.h"
#include <iostream>
#include <smooth/core/network/SocketDispatcher.h>
#include <smooth/core/network/event/ConnectionStatusEvent.h>
#include <smooth/core/task_priorities.h>

//---------------------------------------------------------------------------
namespace espiot::esp {
//---------------------------------------------------------------------------
using namespace smooth::core;
//---------------------------------------------------------------------------
WifiTask::WifiTask(network::Wifi& wifi, actuators::RgbLed& rgbLed, esp::Storage& storage) : Task("WIFI Task", 0, smooth::core::APPLICATION_BASE_PRIO, std::chrono::seconds(1), 1),
                                                                                            wifi(wifi),
                                                                                            rgbLed(rgbLed),
                                                                                            storage(storage),
                                                                                            net_status(NetworkStatusQueue::create(2, *this, *this)) {}

void WifiTask::init() {
    std::cout << "Scanning for wifi...\n";
    rgbLed.turnOnOnly(rgbLed.g);
    rgbLed.turnOn(rgbLed.r);

    rgbLed.turnOnOnly(rgbLed.b);
    wifi.set_host_name("ESP-XMPP");
    wifi.set_auto_connect(true);

    std::string ssid = storage.readString(esp::Storage::WIFI_SSID);
    std::string password = storage.readString(esp::Storage::WIFI_PASSWORD);
    std::cout << "Connecting to Wi-Fi '" << ssid << "' with password: '" << password << "'\n";
    wifi.set_ap_credentials(ssid, password);
    wifi.connect_to_ap();
}

void WifiTask::event(const network::NetworkStatus& event) {
    switch (event.get_event()) {
        case network::NetworkEvent::GOT_IP:
            std::cout << "WIFI: Got IP!\n";
            rgbLed.turnOnOnly(rgbLed.g);
            break;

        case network::NetworkEvent::DISCONNECTED:
            std::cout << "WIFI: Disconnected!\n";
            rgbLed.turnOnOnly(rgbLed.g);
            rgbLed.turnOn(rgbLed.r);
            break;

        default:
            break;
    }

    // Trigger the socket dispatcher event:
    smooth::core::network::SocketDispatcher::instance().event(event);
}
//---------------------------------------------------------------------------
} // namespace espiot::esp
//---------------------------------------------------------------------------
