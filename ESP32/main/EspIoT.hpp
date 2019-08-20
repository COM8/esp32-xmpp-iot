#pragma once

#include "esp/LedHandler.hpp"
#include "esp/WifiTask.hpp"
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
    esp::LedHandler ledHandler;
    esp::WifiTask wifiTask;

    public:
    EspIoT();

    void init() override;
    void tick() override;
};
//---------------------------------------------------------------------------
} // namespace espiot
//---------------------------------------------------------------------------
