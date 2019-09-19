#include "EspIoT.hpp"
#include "esp_log.h"
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
                   bmp180(GPIO_NUM_32, GPIO_NUM_33),
                   btServer(rgbLed) {}

void EspIoT::init() {
    // Set log level to DEBUG:
    esp_log_level_set("*", ESP_LOG_DEBUG);

    rgbLed.turnOnOnly(rgbLed.r);

    // Start the WIFI task:
    // wifiTask.start();
    btServer.init();
    btServer.start();
    rgbLed.turnOff(rgbLed.r);
    rgbLed.turnOn(rgbLed.g);
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
