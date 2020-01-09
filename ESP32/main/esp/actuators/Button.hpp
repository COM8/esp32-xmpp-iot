#pragma once

#include "driver/gpio.h"
#include <smooth/core/io/Input.h>

//---------------------------------------------------------------------------
namespace espiot::esp::actuators {
//---------------------------------------------------------------------------
class Button {
    public:
    smooth::core::io::Input signal;

    Button(gpio_num_t signal);

    bool isPressed();
};
//---------------------------------------------------------------------------
} // namespace espiot::esp::actuators
//---------------------------------------------------------------------------