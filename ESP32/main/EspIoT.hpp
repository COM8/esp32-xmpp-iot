#pragma once

#include "esp/RgbLed.hpp"
#include "esp/WifiTask.hpp"
#include "esp/sensors/Bmp180.hpp"
#include <smooth/core/Application.h>

//---------------------------------------------------------------------------
namespace espiot {
//---------------------------------------------------------------------------
#if defined(__cplusplus)
extern "C" {
#endif

void app_main(void);

#if defined(__cplusplus)
}
#endif
//---------------------------------------------------------------------------
class EspIoT : public smooth::core::Application {
    private:
    esp::RgbLed rgbLed;
    esp::WifiTask wifiTask;
    esp::sensors::Bmp180 bmp180;

    public:
    EspIoT();

    void init() override;
    void tick() override;
};
//---------------------------------------------------------------------------
} // namespace espiot
//---------------------------------------------------------------------------
