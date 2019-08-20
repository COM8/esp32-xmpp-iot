#pragma once

#include "esp/LedHandler.hpp"

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
class EspIoT {
    private:
    esp::LedHandler ledHandler;

    public:
    void run();
};

//---------------------------------------------------------------------------
} // namespace espiot
//---------------------------------------------------------------------------
