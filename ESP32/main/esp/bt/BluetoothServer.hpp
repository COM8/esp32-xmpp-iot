#pragma once

#include "esp/RgbLed.hpp"

//---------------------------------------------------------------------------
namespace espiot::esp {
//---------------------------------------------------------------------------
class BluetoothServer {
    private:
    RgbLed& rgbLed;
    bool running;

    public:
    BluetoothServer(RgbLed& rgbLed);

    bool isRunning();

    void start();
    void stop();
};
//---------------------------------------------------------------------------
} // namespace espiot::esp
//---------------------------------------------------------------------------