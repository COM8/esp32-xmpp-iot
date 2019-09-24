#include "BluetoothServer.hpp"
#include "BLEDevice.h"
#include <array>
#include <cstdio>
#include <string>

//---------------------------------------------------------------------------
namespace espiot::esp::bt {
//---------------------------------------------------------------------------
BluetoothServer::BluetoothServer(RgbLed& rgbLed, Storage& storage) : rgbLed(rgbLed),
                                                                     storage(storage),
                                                                     running(false),
                                                                     server(nullptr),
                                                                     unlockHelper(),
                                                                     serviceHelper(getChipMacString()),
                                                                     advertising(nullptr),
                                                                     advertisingData(),
                                                                     serverCallback(nullptr) {}

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
    if (characteristic->getUUID().equals(BLEServiceHelper::UUID_CHARACTERISTIC_CHALLENGE_RESPONSE_WRITE)) {
        std::string value = characteristic->getValue();
        unlockHelper.onChallengeAnswer(value);

        if (!unlockHelper.isLocked()) {
            serviceHelper.unlock(server);
        }
    } else if (!unlockHelper.isLocked()) {
        std::string value = characteristic->getValue();
        if (characteristic->getUUID().equals(BLEServiceHelper::UUID_CHARACTERISTIC_WIFI_SSID)) {
            storage.writeString(Storage::WIFI_SSID, value);
        } else if (characteristic->getUUID().equals(BLEServiceHelper::UUID_CHARACTERISTIC_WIFI_PASSWORD)) {
            storage.writeString(Storage::WIFI_PASSWORD, value);
        } else if (characteristic->getUUID().equals(BLEServiceHelper::UUID_CHARACTERISTIC_JID)) {
            storage.writeString(Storage::JID, value);
        } else if (characteristic->getUUID().equals(BLEServiceHelper::UUID_CHARACTERISTIC_JID_PASSWORD)) {
            storage.writeString(Storage::JID_PASSWORD, value);
        } else if (characteristic->getUUID().equals(BLEServiceHelper::UUID_CHARACTERISTIC_SETTINGS_DONE)) {
            storage.writeBool(Storage::INITIALIZED, true);
            if (serverCallback) {
                std::string wifiSsid = storage.readString(Storage::WIFI_SSID);
                std::string wifiPassword = storage.readString(Storage::WIFI_PASSWORD);
                std::string jid = storage.readString(Storage::JID);
                std::string jidPassword = storage.readString(Storage::JID_PASSWORD);
                serverCallback->onConfigurationDone(wifiSsid, wifiPassword, jid, jidPassword);
            }
        }
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

void BluetoothServer::registerCallback(BluetoothServerCallback* serverCallback) {
    this->serverCallback = serverCallback;
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