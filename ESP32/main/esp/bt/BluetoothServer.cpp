#include "BluetoothServer.hpp"
#include "BluetoothHelper.hpp"

//---------------------------------------------------------------------------
namespace espiot::esp {
//---------------------------------------------------------------------------
BluetoothServer::BluetoothServer(RgbLed& rgbLed) : rgbLed(rgbLed), running(false) {
}

bool BluetoothServer::isRunning() {
    return running;
}

void BluetoothServer::start() {
    if (running) {
        return;
    }
    BluetoothHelper::initBLE();
    BluetoothHelper::initGapGatt(0x55);
    BluetoothHelper::printMac();
}

void BluetoothServer::stop() {
    if (!running) {
    }
}

//---------------------------------------------------------------------------
} // namespace espiot::esp
//---------------------------------------------------------------------------