#pragma once

#include "BLE2902.h"
#include "BLEAdvertising.h"
#include "BLECharacteristic.h"
#include "BLEServer.h"
#include "BLEService.h"
#include "BLEServiceHelper.hpp"
#include "BluetoothServerCallback.hpp"
#include "esp/RgbLed.hpp"
#include "esp/Storage.hpp"
#include <string>

//---------------------------------------------------------------------------
namespace espiot::esp::bt {
//---------------------------------------------------------------------------
class BluetoothServer : public BLECharacteristicCallbacks, public BLEServerCallbacks {
    private:
    RgbLed& rgbLed;
    Storage& storage;
    bool running;

    BLEServer* server;
    BLEServiceHelper serviceHelper;
    BLEAdvertising* advertising;
    BLEAdvertisementData advertisingData;
    BluetoothServerCallback* serverCallback;

    public:
    BluetoothServer(RgbLed& rgbLed, Storage& storage);
    ~BluetoothServer() = default;

    bool isRunning();

    void init();
    void start();
    void stop();
    void registerCallback(BluetoothServerCallback* serverCallback);

    void onRead(BLECharacteristic* characteristic) override;
    void onWrite(BLECharacteristic* characteristic) override;

    void onConnect(BLEServer* pServer) override;
    void onDisconnect(BLEServer* pServer) override;

    static std::string getChipMacString();
};
//---------------------------------------------------------------------------
} // namespace espiot::esp::bt
//---------------------------------------------------------------------------