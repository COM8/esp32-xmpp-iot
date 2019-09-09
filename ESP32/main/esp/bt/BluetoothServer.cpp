#include "BluetoothServer.hpp"
#include "BLEDevice.h"

//---------------------------------------------------------------------------
namespace espiot::esp {
//---------------------------------------------------------------------------
BluetoothServer::BluetoothServer(RgbLed& rgbLed) : rgbLed(rgbLed),
                                                   running(false),
                                                   server(nullptr),
                                                   service(nullptr),
                                                   characteristic(nullptr),
                                                   descriptor(),
                                                   advertising(nullptr) {}

bool BluetoothServer::isRunning() {
    return running;
}

void BluetoothServer::init() {
    BLEDevice::init("ESP32 meets XMPP");
    server = BLEDevice::createServer();
    service = server->createService("91bad492-b950-4226-aa2b-4ede9fa42f59");

    characteristic = service->createCharacteristic(
        BLEUUID("0d563a58-196a-48ce-ace2-dfec78acc814"),
        BLECharacteristic::PROPERTY_BROADCAST | BLECharacteristic::PROPERTY_READ |
            BLECharacteristic::PROPERTY_NOTIFY | BLECharacteristic::PROPERTY_WRITE |
            BLECharacteristic::PROPERTY_INDICATE);

    characteristic->setValue("Hello World!");

    descriptor.setNotifications(true);
    characteristic->addDescriptor(&descriptor);
}

void BluetoothServer::start() {
    if (running) {
        return;
    }
    service->start();

    advertising = server->getAdvertising();
    advertising->addServiceUUID(service->getUUID());
    advertising->start();
}

void BluetoothServer::stop() {
    if (!running) {
    }
    advertising->stop();
    service->stop();
}

//---------------------------------------------------------------------------
} // namespace espiot::esp
//---------------------------------------------------------------------------