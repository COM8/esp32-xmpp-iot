#include "BLEServiceHelper.hpp"
#include <iostream>

//---------------------------------------------------------------------------
namespace espiot::esp::bt {
//---------------------------------------------------------------------------
const BLEUUID BLEServiceHelper::UUID_CHARACTERISTIC_LANGUAGE("00002AA2-0000-1000-8000-00805F9B34FB");
const BLEUUID BLEServiceHelper::UUID_CHARACTERISTIC_HARDWARE_REVISION("00002A27-0000-1000-8000-00805F9B34FB");
const BLEUUID BLEServiceHelper::UUID_CHARACTERISTIC_SOFTWARE_REVISION("00002A28-0000-1000-8000-00805F9B34FB");
const BLEUUID BLEServiceHelper::UUID_CHARACTERISTIC_SERIAL_NUMBER("00002A25-0000-1000-8000-00805F9B34FB");
const BLEUUID BLEServiceHelper::UUID_CHARACTERISTIC_MANUFACTURER_NAME("00002A29-0000-1000-8000-00805F9B34FB");
const BLEUUID BLEServiceHelper::UUID_CHARACTERISTIC_WIFI_SSID("00000001-0000-0000-0000-000000000002");
const BLEUUID BLEServiceHelper::UUID_CHARACTERISTIC_WIFI_PASSWORD("00000002-0000-0000-0000-000000000002");
const BLEUUID BLEServiceHelper::UUID_CHARACTERISTIC_JID("00000003-0000-0000-0000-000000000002");
const BLEUUID BLEServiceHelper::UUID_CHARACTERISTIC_JID_PASSWORD("00000004-0000-0000-0000-000000000002");
const BLEUUID BLEServiceHelper::UUID_CHARACTERISTIC_JID_SENDER("00000005-0000-0000-0000-000000000002");
const BLEUUID BLEServiceHelper::UUID_CHARACTERISTIC_SETTINGS_DONE("00000006-0000-0000-0000-000000000002");
const BLEUUID BLEServiceHelper::UUID_CHARACTERISTIC_CHALLENGE_RESPONSE_READ("00000007-0000-0000-0000-000000000002");
const BLEUUID BLEServiceHelper::UUID_CHARACTERISTIC_CHALLENGE_RESPONSE_WRITE("00000008-0000-0000-0000-000000000002");
const BLEUUID BLEServiceHelper::UUID_CHARACTERISTIC_CHALLENGE_RESPONSE_UNLOCKED("00000009-0000-0000-0000-000000000002");

const BLEUUID BLEServiceHelper::UUID_SERVICE_DEVICE_INFORMATION("0000180A-0000-1000-8000-00805F9B34FB");
const BLEUUID BLEServiceHelper::UUID_SERVICE_DEVICE_SETTINGS("00000001-0000-0000-0000-000000000001");
const BLEUUID BLEServiceHelper::UUID_SERVICE_CHALLENGE_RESPONSE("00000002-0000-0000-0000-000000000001");

BLEServiceHelper::BLEServiceHelper(std::string btMac) : btMac(btMac),
                                                        cCCDescriptors(7),
                                                        cUDDescriptors(7) {}

BLEServiceHelper::~BLEServiceHelper() {
    for (BLE2902* desc : cCCDescriptors) {
        delete desc;
    }

    for (BLEDescriptor* desc : cUDDescriptors) {
        delete desc;
    }
}

BLE2902* BLEServiceHelper::getNewCCCDescriptor(bool enableNotify, bool enableIndicate) {
    BLE2902* desc = new BLE2902();
    desc->setNotifications(enableNotify);
    desc->setIndications(enableIndicate);
    cCCDescriptors.push_back(desc);
    return desc;
}

BLEDescriptor* BLEServiceHelper::getNewCUDDescriptor(std::string description) {
    BLEDescriptor* desc = new BLEDescriptor(BLEUUID(static_cast<uint16_t>(0x2901)));
    desc->setValue(description);
    cUDDescriptors.push_back(desc);
    return desc;
}

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
    characteristic->notify();

    characteristic = service->getCharacteristic(UUID_CHARACTERISTIC_HARDWARE_REVISION);
    characteristic->setValue("1.0");
    characteristic->notify();

    characteristic = service->getCharacteristic(UUID_CHARACTERISTIC_SOFTWARE_REVISION);
    characteristic->setValue("1.0");
    characteristic->notify();

    characteristic = service->getCharacteristic(UUID_CHARACTERISTIC_SERIAL_NUMBER);
    characteristic->setValue(btMac);
    characteristic->notify();

    characteristic = service->getCharacteristic(UUID_CHARACTERISTIC_MANUFACTURER_NAME);
    characteristic->setValue("TUM Garching");
    characteristic->notify();

    // Challenge response:
    service = server->getServiceByUUID(UUID_SERVICE_CHALLENGE_RESPONSE);
    characteristic = service->getCharacteristic(UUID_CHARACTERISTIC_CHALLENGE_RESPONSE_UNLOCKED);
    characteristic->setValue(1);
    characteristic->notify();

    std::cout << "Bluetooth characteristics unlocked.\n";
}

void BLEServiceHelper::lock(BLEServer* server) {
    // Device information:
    BLEService* service = server->getServiceByUUID(UUID_SERVICE_DEVICE_INFORMATION);
    BLECharacteristic* characteristic = service->getCharacteristic(UUID_CHARACTERISTIC_LANGUAGE);
    characteristic->setValue("");
    characteristic->notify();

    characteristic = service->getCharacteristic(UUID_CHARACTERISTIC_HARDWARE_REVISION);
    characteristic->setValue("");
    characteristic->notify();

    characteristic = service->getCharacteristic(UUID_CHARACTERISTIC_SOFTWARE_REVISION);
    characteristic->setValue("");
    characteristic->notify();

    characteristic = service->getCharacteristic(UUID_CHARACTERISTIC_SERIAL_NUMBER);
    characteristic->setValue("");
    characteristic->notify();

    characteristic = service->getCharacteristic(UUID_CHARACTERISTIC_MANUFACTURER_NAME);
    characteristic->setValue("");
    characteristic->notify();

    // Device settings:
    service = server->getServiceByUUID(UUID_SERVICE_DEVICE_SETTINGS);
    characteristic = service->getCharacteristic(UUID_CHARACTERISTIC_WIFI_SSID);
    characteristic->setValue("");

    characteristic = service->getCharacteristic(UUID_CHARACTERISTIC_WIFI_PASSWORD);
    characteristic->setValue("");

    characteristic = service->getCharacteristic(UUID_CHARACTERISTIC_JID);
    characteristic->setValue("");

    characteristic = service->getCharacteristic(UUID_CHARACTERISTIC_JID_PASSWORD);
    characteristic->setValue("");

    characteristic = service->getCharacteristic(UUID_CHARACTERISTIC_JID_SENDER);
    characteristic->setValue("");

    // Challenge response:
    service = server->getServiceByUUID(UUID_SERVICE_CHALLENGE_RESPONSE);
    characteristic = service->getCharacteristic(UUID_CHARACTERISTIC_CHALLENGE_RESPONSE_READ);
    characteristic->setValue("");
    characteristic->notify();

    characteristic = service->getCharacteristic(UUID_CHARACTERISTIC_CHALLENGE_RESPONSE_WRITE);
    characteristic->setValue("");

    characteristic = service->getCharacteristic(UUID_CHARACTERISTIC_CHALLENGE_RESPONSE_UNLOCKED);
    characteristic->setValue(0);
    characteristic->notify();

    std::cout << "Bluetooth characteristics locked.\n";
}

void BLEServiceHelper::initDeviceInfoService(BLECharacteristicCallbacks* callback, BLEServer* server) {
    BLEService* service = server->createService(UUID_SERVICE_DEVICE_INFORMATION);

    // Language:
    BLECharacteristic* characteristic = service->createCharacteristic(UUID_CHARACTERISTIC_LANGUAGE,
                                                                      BLECharacteristic::PROPERTY_READ |
                                                                          BLECharacteristic::PROPERTY_NOTIFY);
    characteristic->addDescriptor(getNewCCCDescriptor(true, false));

    // Hardware Revision:
    characteristic = service->createCharacteristic(
        UUID_CHARACTERISTIC_HARDWARE_REVISION,
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);
    characteristic->addDescriptor(getNewCCCDescriptor(true, false));

    // Software Revision:
    characteristic = service->createCharacteristic(
        UUID_CHARACTERISTIC_SOFTWARE_REVISION,
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);
    characteristic->addDescriptor(getNewCCCDescriptor(true, false));

    // Serial Number:
    characteristic = service->createCharacteristic(
        UUID_CHARACTERISTIC_SERIAL_NUMBER,
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);
    characteristic->addDescriptor(getNewCCCDescriptor(true, false));

    // Manufacturer Name:
    characteristic = service->createCharacteristic(
        UUID_CHARACTERISTIC_MANUFACTURER_NAME,
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);
    characteristic->addDescriptor(getNewCCCDescriptor(true, false));
}

void BLEServiceHelper::initDeviceSettingsService(BLECharacteristicCallbacks* callback, BLEServer* server) {
    BLEService* service = server->createService(UUID_SERVICE_DEVICE_SETTINGS);

    // WiFi SSID:
    BLECharacteristic* characteristic = service->createCharacteristic(
        UUID_CHARACTERISTIC_WIFI_SSID,
        BLECharacteristic::PROPERTY_WRITE);
    characteristic->setCallbacks(callback);
    characteristic->addDescriptor(getNewCUDDescriptor("Wi-Fi SSID"));

    // WiFi Password:
    characteristic = service->createCharacteristic(
        UUID_CHARACTERISTIC_WIFI_PASSWORD,
        BLECharacteristic::PROPERTY_WRITE);
    characteristic->setCallbacks(callback);
    characteristic->addDescriptor(getNewCUDDescriptor("Wi-Fi Password"));

    // JID:
    characteristic = service->createCharacteristic(
        UUID_CHARACTERISTIC_JID,
        BLECharacteristic::PROPERTY_WRITE);
    characteristic->setCallbacks(callback);
    characteristic->addDescriptor(getNewCUDDescriptor("JID"));

    // JID Password:
    characteristic = service->createCharacteristic(
        UUID_CHARACTERISTIC_JID_PASSWORD,
        BLECharacteristic::PROPERTY_WRITE);
    characteristic->setCallbacks(callback);
    characteristic->addDescriptor(getNewCUDDescriptor("JID Password"));

    // JID Sender:
    characteristic = service->createCharacteristic(
        UUID_CHARACTERISTIC_JID_SENDER,
        BLECharacteristic::PROPERTY_WRITE);
    characteristic->setCallbacks(callback);
    characteristic->addDescriptor(getNewCUDDescriptor("JID Sender"));

    // Settings done:
    characteristic = service->createCharacteristic(
        UUID_CHARACTERISTIC_SETTINGS_DONE,
        BLECharacteristic::PROPERTY_WRITE);
    characteristic->setCallbacks(callback);
    characteristic->addDescriptor(getNewCUDDescriptor("Settings Done"));
}

void BLEServiceHelper::initChallengeResponseService(BLECharacteristicCallbacks* callback, BLEServer* server) {
    BLEService* service = server->createService(UUID_SERVICE_CHALLENGE_RESPONSE);

    // Read:
    BLECharacteristic* characteristic = service->createCharacteristic(
        UUID_CHARACTERISTIC_CHALLENGE_RESPONSE_READ,
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);
    characteristic->setCallbacks(callback);
    characteristic->addDescriptor(getNewCCCDescriptor(true, false));
    characteristic->addDescriptor(getNewCUDDescriptor("Challenge Response Read"));

    // Write:
    characteristic = service->createCharacteristic(
        UUID_CHARACTERISTIC_CHALLENGE_RESPONSE_WRITE,
        BLECharacteristic::PROPERTY_WRITE);
    characteristic->setCallbacks(callback);
    characteristic->addDescriptor(getNewCUDDescriptor("Challenge Response Write"));

    // Unlocked:
    characteristic = service->createCharacteristic(
        UUID_CHARACTERISTIC_CHALLENGE_RESPONSE_UNLOCKED,
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);
    characteristic->addDescriptor(getNewCCCDescriptor(true, false));
    characteristic->addDescriptor(getNewCUDDescriptor("Challenge Response Unlocked"));
}
//---------------------------------------------------------------------------
} // namespace espiot::esp::bt
//---------------------------------------------------------------------------