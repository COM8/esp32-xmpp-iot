#pragma once

#include "BLECharacteristic.h"

//---------------------------------------------------------------------------
namespace espiot::esp::bt {
//---------------------------------------------------------------------------
class BLECharacteristicRWCallback : public BLECharacteristicCallbacks {
    private:
    public:
    BLECharacteristicRWCallback() = default;
    ~BLECharacteristicRWCallback() = default;

    void onRead(BLECharacteristic* characteristic) override;
    void onWrite(BLECharacteristic* characteristic) override;
};
//---------------------------------------------------------------------------
} // namespace espiot::esp::bt
//---------------------------------------------------------------------------