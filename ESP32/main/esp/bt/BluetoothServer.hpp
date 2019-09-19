#pragma once

#include "BLE2902.h"
#include "BLEAdvertising.h"
#include "BLECharacteristic.h"
#include "BLECharacteristicRWCallback.hpp"
#include "BLEServer.h"
#include "BLEService.h"
#include "BLEServiceHelper.hpp"
#include "esp/RgbLed.hpp"
#include <string>

//---------------------------------------------------------------------------
namespace espiot::esp {
//---------------------------------------------------------------------------
class BluetoothServer : public BLECharacteristicCallbacks, public BLEServerCallbacks {
    private:
    RgbLed& rgbLed;
    bool running;

    BLEServer* server;
    BLEServiceHelper serviceHelper;
    BLEAdvertising* advertising;
    BLEAdvertisementData advertisingData;

    public:
    BluetoothServer(RgbLed& rgbLed);
    ~BluetoothServer() = default;

    bool isRunning();

    void init();
    void start();
    void stop();

    void onRead(BLECharacteristic* characteristic) override;
    void onWrite(BLECharacteristic* characteristic) override;

    void onConnect(BLEServer* pServer) override;
    void onDisconnect(BLEServer* pServer) override;

    static std::string getChipMacString();
};
//---------------------------------------------------------------------------
} // namespace espiot::esp
//---------------------------------------------------------------------------