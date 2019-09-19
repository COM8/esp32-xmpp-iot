#include "BLEServiceHelper.hpp"
#include <iostream>

//---------------------------------------------------------------------------
namespace espiot::esp {
//---------------------------------------------------------------------------
const BLEUUID BLEServiceHelper::UUID_CHARACTERISTIC_LANGUAGE("00002AA2-0000-1000-8000-00805F9B34FB");
const BLEUUID BLEServiceHelper::UUID_CHARACTERISTIC_HARDWARE_REVISION("00002A27-0000-1000-8000-00805F9B34FB");
const BLEUUID BLEServiceHelper::UUID_CHARACTERISTIC_SERIAL_NUMBER("00002A25-0000-1000-8000-00805F9B34FB");
const BLEUUID BLEServiceHelper::UUID_CHARACTERISTIC_MANUFACTURER_NAME("00002A29-0000-1000-8000-00805F9B34FB");
const BLEUUID BLEServiceHelper::UUID_CHARACTERISTIC_WIFI_SSID("00000001-0000-0000-0000-000000000002");
const BLEUUID BLEServiceHelper::UUID_CHARACTERISTIC_WIFI_PASSWORD("00000002-0000-0000-0000-000000000002");
const BLEUUID BLEServiceHelper::UUID_CHARACTERISTIC_JID("00000003-0000-0000-0000-000000000002");
const BLEUUID BLEServiceHelper::UUID_CHARACTERISTIC_JID_PASSWORD("00000004-0000-0000-0000-000000000002");
const BLEUUID BLEServiceHelper::UUID_CHARACTERISTIC_SETTINGS_DONE("00000005-0000-0000-0000-000000000002");
const BLEUUID BLEServiceHelper::UUID_CHARACTERISTIC_CHALLENGE_RESPONSE_READ("00000006-0000-0000-0000-000000000002");
const BLEUUID BLEServiceHelper::UUID_CHARACTERISTIC_CHALLENGE_RESPONSE_WRITE("00000007-0000-0000-0000-000000000002");

const BLEUUID BLEServiceHelper::UUID_SERVICE_DEVICE_INFORMATION("0000180A-0000-1000-8000-00805F9B34FB");
const BLEUUID BLEServiceHelper::UUID_SERVICE_DEVICE_SETTINGS("00000001-0000-0000-0000-000000000001");
const BLEUUID BLEServiceHelper::UUID_SERVICE_CHALLENGE_RESPONSE("00000002-0000-0000-0000-000000000001");

BLEServiceHelper::BLEServiceHelper(std::string btMac) : btMac(btMac) {}

void BLEServiceHelper::init(BLECharacteristicCallbacks* callback, BLEServer* server) {
    initDeviceInfoService(callback, server);
    initDeviceSettingsService(callback, server);
    initChallengeResponseService(callback, server);
}

void BLEServiceHelper::start(BLEServer* server) {
    // Device information:
    BLEService* service = server->getServiceByUUID(UUID_SERVICE_DEVICE_INFORMATION);
    service->start();

    // Device settings:
    service = server->getServiceByUUID(UUID_SERVICE_DEVICE_SETTINGS);
    service->start();

    // Challenge response:
    service = server->getServiceByUUID(UUID_SERVICE_CHALLENGE_RESPONSE);
    service->start();
}

void BLEServiceHelper::stop(BLEServer* server) {
    // Device information:
    BLEService* service = server->getServiceByUUID(UUID_SERVICE_DEVICE_INFORMATION);
    service->stop();

    // Device settings:
    service = server->getServiceByUUID(UUID_SERVICE_DEVICE_SETTINGS);
    service->stop();

    // Challenge response:
    service = server->getServiceByUUID(UUID_SERVICE_CHALLENGE_RESPONSE);
    service->stop();
}

void BLEServiceHelper::unlock(BLEServer* server) {
    // Device information:
    BLEService* service = server->getServiceByUUID(UUID_SERVICE_DEVICE_INFORMATION);
    BLECharacteristic* characteristic = service->getCharacteristic(UUID_CHARACTERISTIC_LANGUAGE);
    characteristic->setValue("en");

    characteristic = service->getCharacteristic(UUID_CHARACTERISTIC_HARDWARE_REVISION);
    characteristic->setValue("1.0");

    characteristic = service->getCharacteristic(UUID_CHARACTERISTIC_SERIAL_NUMBER);
    characteristic->setValue(btMac);

    characteristic = service->getCharacteristic(UUID_CHARACTERISTIC_MANUFACTURER_NAME);
    characteristic->setValue("TUM Garching");
}

void BLEServiceHelper::lock(BLEServer* server) {
    // Device information:
    BLEService* service = server->getServiceByUUID(UUID_SERVICE_DEVICE_INFORMATION);
    BLECharacteristic* characteristic = service->getCharacteristic(UUID_CHARACTERISTIC_LANGUAGE);
    characteristic->setValue("");

    characteristic = service->getCharacteristic(UUID_CHARACTERISTIC_HARDWARE_REVISION);
    characteristic->setValue("");

    characteristic = service->getCharacteristic(UUID_CHARACTERISTIC_SERIAL_NUMBER);
    characteristic->setValue("");

    characteristic = service->getCharacteristic(UUID_CHARACTERISTIC_MANUFACTURER_NAME);
    characteristic->setValue("");
}

void BLEServiceHelper::initDeviceInfoService(BLECharacteristicCallbacks* callback, BLEServer* server) {
    BLEService* service = server->createService(UUID_SERVICE_DEVICE_INFORMATION);

    // Language:
    BLECharacteristic* characteristic = service->createCharacteristic(
        UUID_CHARACTERISTIC_LANGUAGE,
        BLECharacteristic::PROPERTY_READ);

    // Hardware Revision:
    characteristic = service->createCharacteristic(
        UUID_CHARACTERISTIC_HARDWARE_REVISION,
        BLECharacteristic::PROPERTY_READ);

    // Serial Number:
    characteristic = service->createCharacteristic(
        UUID_CHARACTERISTIC_SERIAL_NUMBER,
        BLECharacteristic::PROPERTY_READ);

    // Manufacturer Name:
    characteristic = service->createCharacteristic(
        UUID_CHARACTERISTIC_MANUFACTURER_NAME,
        BLECharacteristic::PROPERTY_READ);
}

void BLEServiceHelper::initDeviceSettingsService(BLECharacteristicCallbacks* callback, BLEServer* server) {
    BLEService* service = server->createService(UUID_SERVICE_DEVICE_SETTINGS);

    // WiFi SSID:
    BLECharacteristic* characteristic = service->createCharacteristic(
        UUID_CHARACTERISTIC_WIFI_SSID,
        BLECharacteristic::PROPERTY_WRITE);
    characteristic->setCallbacks(callback);

    // WiFi Password:
    characteristic = service->createCharacteristic(
        UUID_CHARACTERISTIC_WIFI_PASSWORD,
        BLECharacteristic::PROPERTY_WRITE);
    characteristic->setCallbacks(callback);

    // JID:
    characteristic = service->createCharacteristic(
        UUID_CHARACTERISTIC_JID,
        BLECharacteristic::PROPERTY_WRITE);
    characteristic->setCallbacks(callback);

    // JID Password:
    characteristic = service->createCharacteristic(
        UUID_CHARACTERISTIC_JID_PASSWORD,
        BLECharacteristic::PROPERTY_WRITE);
    characteristic->setCallbacks(callback);

    // Settings done:
    characteristic = service->createCharacteristic(
        UUID_CHARACTERISTIC_SETTINGS_DONE,
        BLECharacteristic::PROPERTY_WRITE);
    characteristic->setCallbacks(callback);
}

void BLEServiceHelper::initChallengeResponseService(BLECharacteristicCallbacks* callback, BLEServer* server) {
    BLEService* service = server->createService(UUID_SERVICE_CHALLENGE_RESPONSE);

    // Read:
    BLECharacteristic* characteristic = service->createCharacteristic(
        UUID_CHARACTERISTIC_CHALLENGE_RESPONSE_READ,
        BLECharacteristic::PROPERTY_READ);
    characteristic->setCallbacks(callback);

    // Write:
    characteristic = service->createCharacteristic(
        UUID_CHARACTERISTIC_CHALLENGE_RESPONSE_WRITE,
        BLECharacteristic::PROPERTY_WRITE);
    characteristic->setCallbacks(callback);
}
//---------------------------------------------------------------------------
} // namespace espiot::esp
//---------------------------------------------------------------------------