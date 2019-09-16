#pragma once

#include "BLE2902.h"
#include "BLEAdvertising.h"
#include "BLECharacteristic.h"
#include "BLEServer.h"
#include "BLEService.h"
#include "esp/RgbLed.hpp"
#include <string>

//---------------------------------------------------------------------------
namespace espiot::esp {
//---------------------------------------------------------------------------
class BluetoothServer {
    private:
    //-------------------
    // Bluetooth characteristic UUIDs:
    // Source: https://www.bluetooth.com/specifications/gatt/characteristics/
    // Generation:
    // Base: 00000000-0000-1000-8000-00805F9B34FB + Assigned Number (e.g. 0x2A80 for Age)
    // = 00002A80-0000-1000-8000-00805F9B34FB
    //-------------------

    // Format: utf8s (The Language definition is based on ISO639-1.)
    // Source: https://www.bluetooth.com/wp-content/uploads/Sitecore-Media-Library/Gatt/Xml/Characteristics/org.bluetooth.characteristic.language.xml
    static const BLEUUID UUID_CHARACTERISTIC_LANGUAGE;
    // Format: utf8s
    // Source: https://www.bluetooth.com/wp-content/uploads/Sitecore-Media-Library/Gatt/Xml/Characteristics/org.bluetooth.characteristic.hardware_revision_string.xml
    static const BLEUUID UUID_CHARACTERISTIC_HARDWARE_REVISION;
    // Format: utf8s
    // Source: https://www.bluetooth.com/wp-content/uploads/Sitecore-Media-Library/Gatt/Xml/Characteristics/org.bluetooth.characteristic.serial_number_string.xml
    static const BLEUUID UUID_CHARACTERISTIC_SERIAL_NUMBER;

    //-------------------
    // Bluetooth service UUIDs:
    // Source: https://www.bluetooth.com/specifications/gatt/services/
    // Generation:
    // Base: 00000000-0000-1000-8000-00805F9B34FB + Assigned Number (e.g. 0x1800 for Generic Access)
    // = 00001800-0000-1000-8000-00805F9B34FB
    //-------------------
    // Source: https://www.bluetooth.com/wp-content/uploads/Sitecore-Media-Library/Gatt/Xml/Services/org.bluetooth.service.device_information.xml
    static const BLEUUID UUID_SERVICE_DEVICE_INFORMATION;

    RgbLed& rgbLed;
    bool running;

    BLEServer* server;
    BLEService* service;
    BLE2902 descriptor;
    BLEAdvertising* advertising;

    public:
    BluetoothServer(RgbLed& rgbLed);

    bool isRunning();

    void init();
    void start();
    void stop();

    static std::string getChipMacString();
};
//---------------------------------------------------------------------------
} // namespace espiot::esp
//---------------------------------------------------------------------------