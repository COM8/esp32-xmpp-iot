#include "BluetoothServer.hpp"
#include "BLEDevice.h"
#include <array>
#include <cstdio>
#include <string>

//---------------------------------------------------------------------------
namespace espiot::esp {
//---------------------------------------------------------------------------
const BLEUUID BluetoothServer::UUID_CHARACTERISTIC_LANGUAGE("00002AA2-0000-1000-8000-00805F9B34FB");
const BLEUUID BluetoothServer::UUID_CHARACTERISTIC_HARDWARE_REVISION("00002A27-0000-1000-8000-00805F9B34FB");
const BLEUUID BluetoothServer::UUID_CHARACTERISTIC_SERIAL_NUMBER("00002A25-0000-1000-8000-00805F9B34FB");
const BLEUUID BluetoothServer::UUID_CHARACTERISTIC_MANUFACTURER_NAME("00002A29-0000-1000-8000-00805F9B34FB");

const BLEUUID BluetoothServer::UUID_SERVICE_DEVICE_INFORMATION("0000180A-0000-1000-8000-00805F9B34FB");

BluetoothServer::BluetoothServer(RgbLed& rgbLed) : rgbLed(rgbLed),
                                                   running(false),
                                                   server(nullptr),
                                                   service(nullptr),
                                                   descriptor(),
                                                   advertising(nullptr),
                                                   advertisingData() {}

bool BluetoothServer::isRunning() {
    return running;
}

void BluetoothServer::init() {
    BLEDevice::init("ESP32 meets XMPP");
    server = BLEDevice::createServer();

    service = server->createService(UUID_SERVICE_DEVICE_INFORMATION);

    // Language:
    BLECharacteristic* characteristic = service->createCharacteristic(
        UUID_CHARACTERISTIC_LANGUAGE,
        BLECharacteristic::PROPERTY_BROADCAST |
            BLECharacteristic::PROPERTY_READ);
    characteristic->setValue("en");
    descriptor.setNotifications(true);
    characteristic->addDescriptor(&descriptor);

    // Hardware Revision:
    characteristic = service->createCharacteristic(
        UUID_CHARACTERISTIC_HARDWARE_REVISION,
        BLECharacteristic::PROPERTY_BROADCAST |
            BLECharacteristic::PROPERTY_READ);
    characteristic->setValue("1.0");
    descriptor.setNotifications(true);
    characteristic->addDescriptor(&descriptor);

    // Serial Number:
    characteristic = service->createCharacteristic(
        UUID_CHARACTERISTIC_SERIAL_NUMBER,
        BLECharacteristic::PROPERTY_BROADCAST |
            BLECharacteristic::PROPERTY_READ);
    characteristic->setValue(getChipMacString());
    characteristic->addDescriptor(&descriptor);

    // Manufacturer Name:
    characteristic = service->createCharacteristic(
        UUID_CHARACTERISTIC_MANUFACTURER_NAME,
        BLECharacteristic::PROPERTY_BROADCAST |
            BLECharacteristic::PROPERTY_READ);
    characteristic->setValue("TUM Garching");
    characteristic->addDescriptor(&descriptor);

    // Generic Computer appearance
    // https://www.bluetooth.com/wp-content/uploads/Sitecore-Media-Library/Gatt/Xml/Characteristics/org.bluetooth.characteristic.gap.appearance.xml
    advertisingData.setAppearance(128);
    advertisingData.setManufacturerData("TUM Garching");
    advertisingData.setName("ESP32 meets XMPP");

    advertising = server->getAdvertising();
    advertising->setAdvertisementData(advertisingData);
}

void BluetoothServer::start() {
    if (running) {
        return;
    }
    service->start();
    advertising->addServiceUUID(service->getUUID());
    advertising->start();
    rgbLed.turnOn(rgbLed.b);
}

void BluetoothServer::stop() {
    if (!running) {
    }
    advertising->stop();
    service->stop();
    rgbLed.turnOff(rgbLed.b);
}

std::string BluetoothServer::getChipMacString() {
    std::array<uint8_t, 6> chipMac;
    esp_efuse_mac_get_default(chipMac.data());
    std::array<char, 18> buff;
    sprintf(buff.data(), "%02X:%02X:%02X:%02X:%02X:%02X", chipMac[0], chipMac[1], chipMac[2], chipMac[3], chipMac[4], chipMac[5]);
    return std::string(buff.data());
}

//---------------------------------------------------------------------------
} // namespace espiot::esp
//---------------------------------------------------------------------------