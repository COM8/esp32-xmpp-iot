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
                   rgbLed(GPIO_NUM_27, GPIO_NUM_26, GPIO_NUM_25),
                   wifiTask(get_wifi(), rgbLed),
                   bmp180(GPIO_NUM_32, GPIO_NUM_33) {}

void EspIoT::init() {
    // Start the WIFI task:
    wifiTask.start();
}

void EspIoT::tick() {
    double tmp = bmp180.readTemp();
    printf("Temp: %lf\n", tmp);

    int32_t pressure = bmp180.readPressure();
    printf("Pressure: %i\n", pressure);
}
//---------------------------------------------------------------------------
} // namespace espiot
//---------------------------------------------------------------------------
