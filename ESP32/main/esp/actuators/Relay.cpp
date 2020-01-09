#include "Relay.hpp"

//---------------------------------------------------------------------------
namespace espiot::esp::actuators {
//---------------------------------------------------------------------------
Relay::Relay(gpio_num_t signal) : active(false),
                                  signal(signal, true, false, false) {}

void Relay::set(bool active) {
    signal.set(active);
    this->active = active;
}

bool Relay::toggle() {
    set(!active);
    return active;
}
//---------------------------------------------------------------------------
} // namespace espiot::esp::actuators
//---------------------------------------------------------------------------