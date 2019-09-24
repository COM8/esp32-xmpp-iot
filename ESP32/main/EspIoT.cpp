#include "EspIoT.hpp"
#include "esp_log.h"
#include "esp_system.h"
#include <cstdio>
#include <iostream>
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
                   storage(),
                   wifiTask(get_wifi(), rgbLed),
                   bmp180(GPIO_NUM_32, GPIO_NUM_33),
                   btServer(rgbLed, storage){};

void EspIoT::init() {
    // Set log level to DEBUG:
    esp_log_level_set("*", ESP_LOG_DEBUG);

    storage.init();
    rgbLed.turnOnOnly(rgbLed.r);

    if (storage.readBool(esp::Storage::INITIALIZED)) {
        // Start the WIFI task:
        wifiTask.start();
    } else {
        btServer.init();
        btServer.registerCallback(this);
        btServer.start();
    }
    rgbLed.turnOff(rgbLed.r);
    rgbLed.turnOn(rgbLed.g);
}

void EspIoT::tick() {
    double tmp = bmp180.readTemp();
    printf("Temp: %lf\n", tmp);

    int32_t pressure = bmp180.readPressure();
    printf("Pressure: %i\n", pressure);
}

void EspIoT::onConfigurationDone(std::string& wifiSsid, std::string& wifiPassword, std::string& jid, std::string& jidPassword) {
    std::cout << "Bluetooth configuration done with:\n";
    std::cout << "Wi-Fi SSID: " << wifiSsid << "\n";
    std::cout << "Wi-Fi Password: " << wifiPassword << "\n";
    std::cout << "JID: " << jid << "\n";
    std::cout << "JID Password: " << jidPassword << std::endl;
    // Restart
    esp_restart();
}
//---------------------------------------------------------------------------
} // namespace espiot
//---------------------------------------------------------------------------
