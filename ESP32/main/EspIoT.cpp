#include "EspIoT.hpp"
#include <cstdio>
#include <smooth/core/task_priorities.h>

//---------------------------------------------------------------------------
namespace espiot {
//---------------------------------------------------------------------------
using namespace smooth::core;
//---------------------------------------------------------------------------
void app_main(void) {
    EspIoT espIoT;
    espIoT.start();
}

EspIoT::EspIoT() : Application(smooth::core::APPLICATION_BASE_PRIO, std::chrono::seconds(1)),
                   rgbLed(GPIO_NUM_14, GPIO_NUM_27, GPIO_NUM_12),
                   wifiTask(get_wifi(), rgbLed) {}

void EspIoT::init() {
    // Start the WIFI task:
    wifiTask.start();
}

void EspIoT::tick() {
    printf("Main tick\n");
}
//---------------------------------------------------------------------------
} // namespace espiot
//---------------------------------------------------------------------------
