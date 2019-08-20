#pragma once

#include "driver/gpio.h"

//---------------------------------------------------------------------------
namespace espiot::esp {
//---------------------------------------------------------------------------
class LedHandler {
    public:
    const gpio_num_t GPIO_LED_RED = GPIO_NUM_14;
    const gpio_num_t GPIO_LED_BLUE = GPIO_NUM_12;
    const gpio_num_t GPIO_LED_GREEN = GPIO_NUM_27;

    void init();
    void turnAllOff();
    void turnAllOn();
    void blink(gpio_num_t gpioNum);
    void turnOn(gpio_num_t gpioNum);
    void turnOff(gpio_num_t gpioNum);
};

//---------------------------------------------------------------------------
} // namespace espiot::esp
//---------------------------------------------------------------------------