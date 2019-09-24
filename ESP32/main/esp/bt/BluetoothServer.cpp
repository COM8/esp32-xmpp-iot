#include "BluetoothServer.hpp"
#include "BLEDevice.h"
#include <array>
#include <cstdio>
#include <string>

//---------------------------------------------------------------------------
namespace espiot::esp::bt {
//---------------------------------------------------------------------------
BluetoothServer::BluetoothServer(RgbLed& rgbLed) : rgbLed(rgbLed),
                                                   running(false),
                                                   server(nullptr),
                                                   serviceHelper(getChipMacString()),
                                                   advertising(nullptr),
                                                   advertisingData() {}

bool BluetoothServer::isRunning() {
    return running;
}

void BluetoothServer::init() {
    BLEDevice::init("ESP32 meets XMPP");
    server = BLEDevice::createServer();
    server->setCallbacks(this);

    // Generic Computer appearance
    // https://www.bluetooth.com/wp-content/uploads/Sitecore-Media-Library/Gatt/Xml/Characteristics/org.bluetooth.characteristic.gap.appearance.xml
    advertisingData.setAppearance(128);
    advertisingData.setManufacturerData("TUM Garching");
    advertisingData.setName("ESP32 meets XMPP");

    advertising = server->getAdvertising();
    advertising->setAdvertisementData(advertisingData);

    serviceHelper.init(this, server);
}

void BluetoothServer::start() {
    if (running) {
        return;
    }
    serviceHelper.start(server);
    advertising->start();
    rgbLed.turnOn(rgbLed.b);
}

void BluetoothServer::stop() {
    if (!running) {
    }
    advertising->stop();
    serviceHelper.stop(server);
    rgbLed.turnOff(rgbLed.b);
}

void BluetoothServer::onRead(BLECharacteristic* characteristic) {}

void BluetoothServer::onWrite(BLECharacteristic* characteristic) {
    if (characteristic->getValue() == "ready") {
        serviceHelper.unlock(server);
    }
}

void BluetoothServer::onConnect(BLEServer* pServer) {
    // Make sure we lock all information as soon as somebody else connects:
    serviceHelper.lock(server);
}

void BluetoothServer::onDisconnect(BLEServer* pServer) {
    // Make sure we lock all information as soon as somebody disconnects:
    serviceHelper.lock(server);
}

std::string BluetoothServer::getChipMacString() {
    std::array<uint8_t, 6> chipMac;
    esp_efuse_mac_get_default(chipMac.data());
    std::array<char, 18> buff;
    sprintf(buff.data(), "%02X:%02X:%02X:%02X:%02X:%02X", chipMac[0], chipMac[1], chipMac[2], chipMac[3], chipMac[4], chipMac[5]);
    return std::string(buff.data());
}

//---------------------------------------------------------------------------
} // namespace espiot::esp::bt
//---------------------------------------------------------------------------