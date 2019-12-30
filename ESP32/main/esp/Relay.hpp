#pragma once

#include "driver/gpio.h"
#include <smooth/core/io/Output.h>

//---------------------------------------------------------------------------
namespace espiot::esp {
//---------------------------------------------------------------------------
class Relay {
    private:
    bool active;

    public:
    smooth::core::io::Output signal;

    Relay(gpio_num_t signal);

    void set(bool active);
    bool toggle();
};
//---------------------------------------------------------------------------
} // namespace espiot::esp
//---------------------------------------------------------------------------