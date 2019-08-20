#include "LedHandler.hpp"

//---------------------------------------------------------------------------
namespace espiot::esp {
//---------------------------------------------------------------------------
void LedHandler::init() {
    // Red:
    gpio_pad_select_gpio(GPIO_LED_RED);
    gpio_set_direction(GPIO_LED_RED, GPIO_MODE_OUTPUT);

    // Blue:
    gpio_pad_select_gpio(GPIO_LED_BLUE);
    gpio_set_direction(GPIO_LED_BLUE, GPIO_MODE_OUTPUT);

    // Green:
    gpio_pad_select_gpio(GPIO_LED_GREEN);
    gpio_set_direction(GPIO_LED_GREEN, GPIO_MODE_OUTPUT);

    // Make sure all LEDs are off:
    turnAllOff();
}

void LedHandler::turnAllOff() {
    turnOff(GPIO_LED_RED);
    turnOff(GPIO_LED_BLUE);
    turnOff(GPIO_LED_GREEN);
}

void LedHandler::turnAllOn() {
    turnOn(GPIO_LED_RED);
    turnOn(GPIO_LED_BLUE);
    turnOn(GPIO_LED_GREEN);
}

void LedHandler::blink(gpio_num_t gpioNum) {
}

void LedHandler::turnOn(gpio_num_t gpioNum) {
    gpio_set_level(gpioNum, 0);
}

void LedHandler::turnOff(gpio_num_t gpioNum) {
    gpio_set_level(gpioNum, 0);
}
//---------------------------------------------------------------------------
} // namespace espiot::esp
//---------------------------------------------------------------------------