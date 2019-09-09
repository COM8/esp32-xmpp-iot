#pragma once

#include "BLE2902.h"
#include "BLEAdvertising.h"
#include "BLECharacteristic.h"
#include "BLEServer.h"
#include "BLEService.h"
#include "esp/RgbLed.hpp"

//---------------------------------------------------------------------------
namespace espiot::esp {
//---------------------------------------------------------------------------
class BluetoothServer {
    private:
    RgbLed& rgbLed;
    bool running;

    BLEServer* server;
    BLEService* service;
    BLECharacteristic* characteristic;
    BLE2902 descriptor;
    BLEAdvertising* advertising;

    public:
    BluetoothServer(RgbLed& rgbLed);

    bool isRunning();

    void init();
    void start();
    void stop();
};
//---------------------------------------------------------------------------
} // namespace espiot::esp
//---------------------------------------------------------------------------