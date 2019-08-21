#include "EspIoT.hpp"
#include <cstdio>
#include <smooth/core/task_priorities.h>

//---------------------------------------------------------------------------
namespace espiot {
//---------------------------------------------------------------------------
void app_main(void) {
    EspIoT espIoT;
    espIoT.start();
}

EspIoT::EspIoT() : Application(smooth::core::APPLICATION_BASE_PRIO, std::chrono::seconds(1)),
                   ledHandler(), wifiTask(get_wifi(), ledHandler) {}

void EspIoT::init() {
    // Init the LED handler:
    ledHandler.init();

    // Start the WIFI task:
    wifiTask.start();
}

void EspIoT::tick() {
    printf("Main tick\n");
}
//---------------------------------------------------------------------------
} // namespace espiot
//---------------------------------------------------------------------------
