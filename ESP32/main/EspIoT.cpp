#include "EspIoT.hpp"
#include "esp/WiFiCredentials.hpp"
#include "esp_log.h"
#include "esp_system.h"
#include "xmpp/XmppCredentials.hpp"
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
                   resetButton(GPIO_NUM_4),
                   storage(),
                   wifiTask(get_wifi(), rgbLed),
                   bmp180(GPIO_NUM_32, GPIO_NUM_33),
                   btServer(rgbLed, storage),
                   xmppTask(storage){};

void EspIoT::initWithDummyValues() {
    storage.writeString(esp::Storage::JID, xmpp::JID);
    storage.writeString(esp::Storage::JID_PASSWORD, xmpp::JID_PASSWORD);
    storage.writeString(esp::Storage::JID_SENDER, xmpp::JID_SENDER);
    storage.writeString(esp::Storage::WIFI_SSID, esp::SSID);
    storage.writeString(esp::Storage::WIFI_PASSWORD, esp::PASSWORD);
    storage.writeBool(esp::Storage::INITIALIZED, true);
    storage.writeBool(esp::Storage::SETUP_DONE, true);
    std::cout << "INITIALIZED WITH DUMMY VALUES!" << std::endl;
}

void EspIoT::init() {
    // Set log level to DEBUG:
    esp_log_level_set("*", ESP_LOG_DEBUG);

    storage.init();
    rgbLed.turnOnOnly(rgbLed.r);

    // initWithDummyValues();

    // Check if reset button is pressen:
    if (resetButton.isPressed()) {
        std::cout << "Reset button pressed. Discarding initialization.\n";
        storage.writeBool(esp::Storage::INITIALIZED, false);
    }

    if (storage.readBool(esp::Storage::INITIALIZED)) {
        // Start the WIFI task:
        wifiTask.start();
        xmppTask.start();
    } else {
        btServer.init();
        btServer.registerCallback(this);
        btServer.start();
    }
}

void EspIoT::tick() {
    double tmp = bmp180.readTemp();
    printf("Temp: %lf\n", tmp);

    int32_t pressure = bmp180.readPressure();
    printf("Pressure: %i\n", pressure);

    // Check if reset button is pressen:
    if (resetButton.isPressed()) {
        std::cout << "Reset button pressed. Discarding initialization.\n";
        storage.writeBool(esp::Storage::INITIALIZED, false);
        // Restart
        esp_restart();
    }
}

void EspIoT::onConfigurationDone(std::string& wifiSsid, std::string& wifiPassword, std::string& jid, std::string& jidPassword, std::string& jidSender) {
    std::cout << "Bluetooth configuration done with:\n";
    std::cout << "Wi-Fi SSID: " << wifiSsid << "\n";
    std::cout << "Wi-Fi Password: " << wifiPassword << "\n";
    std::cout << "JID: " << jid << "\n";
    std::cout << "JID Password: " << jidPassword << "\n";
    std::cout << "JID Sender: " << jidSender << std::endl;
    // Restart
    esp_restart();
}
//---------------------------------------------------------------------------
} // namespace espiot
//---------------------------------------------------------------------------
