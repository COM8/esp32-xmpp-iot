#pragma once

#include "BLECharacteristic.h"
#include "BLEServer.h"
#include "BLEUUID.h"
#include <string>

//---------------------------------------------------------------------------
namespace espiot::esp {
//---------------------------------------------------------------------------
class BLEServiceHelper {
    private:
    std::string btMac;

    void initDeviceInfoService(BLECharacteristicCallbacks* callback, BLEServer* server);
    void initDeviceSettingsService(BLECharacteristicCallbacks* callback, BLEServer* server);
    void initChallengeResponseService(BLECharacteristicCallbacks* callback, BLEServer* server);

    public:
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
    // Format: utf8s
    // Source: https://www.bluetooth.com/wp-content/uploads/Sitecore-Media-Library/Gatt/Xml/Characteristics/org.bluetooth.characteristic.manufacturer_name_string.xml
    static const BLEUUID UUID_CHARACTERISTIC_MANUFACTURER_NAME;
    // Custom UUID
    static const BLEUUID UUID_CHARACTERISTIC_WIFI_SSID;
    // Custom UUID
    static const BLEUUID UUID_CHARACTERISTIC_WIFI_PASSWORD;
    // Custom UUID
    static const BLEUUID UUID_CHARACTERISTIC_JID;
    // Custom UUID
    static const BLEUUID UUID_CHARACTERISTIC_JID_PASSWORD;
    // Custom UUID
    static const BLEUUID UUID_CHARACTERISTIC_SETTINGS_DONE;
    // Custom UUID
    static const BLEUUID UUID_CHARACTERISTIC_CHALLENGE_RESPONSE_READ;
    // Custom UUID
    static const BLEUUID UUID_CHARACTERISTIC_CHALLENGE_RESPONSE_WRITE;

    //-------------------
    // Bluetooth service UUIDs:
    // Source: https://www.bluetooth.com/specifications/gatt/services/
    // Generation:
    // Base: 00000000-0000-1000-8000-00805F9B34FB + Assigned Number (e.g. 0x1800 for Generic Access)
    // = 00001800-0000-1000-8000-00805F9B34FB
    //-------------------
    // Source: https://www.bluetooth.com/wp-content/uploads/Sitecore-Media-Library/Gatt/Xml/Services/org.bluetooth.service.device_information.xml
    static const BLEUUID UUID_SERVICE_DEVICE_INFORMATION;
    // Custom UUID
    static const BLEUUID UUID_SERVICE_DEVICE_SETTINGS;
    // Custom UUID
    static const BLEUUID UUID_SERVICE_CHALLENGE_RESPONSE;

    BLEServiceHelper(std::string btMac);

    void init(BLECharacteristicCallbacks* callback, BLEServer* server);
    void start(BLEServer* server);
    void stop(BLEServer* server);
    void unlock(BLEServer* server);
    void lock(BLEServer* server);
};
//---------------------------------------------------------------------------
} // namespace espiot::esp
//---------------------------------------------------------------------------