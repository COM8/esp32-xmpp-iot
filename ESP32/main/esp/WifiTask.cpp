#include "WifiTask.hpp"
#include "LedHandler.hpp"
#include <smooth/core/network/event/ConnectionStatusEvent.h>
#include <smooth/core/task_priorities.h>

//---------------------------------------------------------------------------
namespace espiot::esp {
//---------------------------------------------------------------------------
using namespace smooth::core;
//---------------------------------------------------------------------------
WifiTask::WifiTask(network::Wifi& wifi, LedHandler& ledHandler) : Task("WIFI Task", smooth::core::APPLICATION_BASE_PRIO, 0, std::chrono::seconds(1), 1),
                                                                  wifi(wifi),
                                                                  ledHandler(ledHandler) {
    ledHandler.turnAllOff();
    ledHandler.turnOn(ledHandler.GPIO_LED_RED);
}

void WifiTask::init() {
    printf("Scanning for wifi...\n");
    ledHandler.turnAllOff();
    ledHandler.turnOn(ledHandler.GPIO_LED_BLUE);
    wifi.set_host_name("Smooth-ESP");
    wifi.set_auto_connect(true);
    wifi.set_ap_credentials("Terrorbird", "updet11eguewj");
    wifi.connect_to_ap();
}

void WifiTask::event(const network::NetworkStatus& event) {
    switch (event.get_event()) {
        case network::NetworkEvent::GOT_IP:
            ledHandler.turnAllOff();
            ledHandler.turnOn(ledHandler.GPIO_LED_GREEN);
            printf("WIFI: Got IP!\n");
            break;

        case network::NetworkEvent::DISCONNECTED:
            ledHandler.turnAllOff();
            ledHandler.turnOn(ledHandler.GPIO_LED_RED);
            printf("WIFI: Disconnected!\n");
            break;

        default:
            break;
    }
}
//---------------------------------------------------------------------------
} // namespace espiot::esp
//---------------------------------------------------------------------------
