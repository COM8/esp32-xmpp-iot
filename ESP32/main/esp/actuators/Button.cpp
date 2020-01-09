#include "Button.hpp"

//---------------------------------------------------------------------------
namespace espiot::esp::actuators {
//---------------------------------------------------------------------------
Button::Button(gpio_num_t signal) : signal(signal, true, false) {}

bool Button::isPressed() {
    return not signal.read();
}
//---------------------------------------------------------------------------
} // namespace espiot::esp::actuators
//---------------------------------------------------------------------------