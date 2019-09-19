#include "BLECharacteristicRWCallback.hpp"
#include <iostream>

//---------------------------------------------------------------------------
namespace espiot::esp {
//---------------------------------------------------------------------------
void BLECharacteristicRWCallback::onRead(BLECharacteristic* characteristic) {
    std::cout << "Characteristic READ: " << characteristic->getUUID().toString();
    std::cout << " " << characteristic->getValue() << "\n";
}

void BLECharacteristicRWCallback::onWrite(BLECharacteristic* characteristic) {
    std::cout << "Characteristic WRITE: " << characteristic->getUUID().toString();
    std::cout << " " << characteristic->getValue() << "\n";
}
//---------------------------------------------------------------------------
} // namespace espiot::esp
//---------------------------------------------------------------------------