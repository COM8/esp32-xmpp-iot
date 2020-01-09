#pragma once

#include "driver/ledc.h"

//---------------------------------------------------------------------------
namespace espiot::esp::actuators {
//---------------------------------------------------------------------------
class Speaker {
    private:
    ledc_timer_config_t ledc_timer;
    ledc_channel_config_t ledc_channel;

    public:
    Speaker(gpio_num_t signal);

    void set(uint32_t freq_hz);
    uint32_t get();
};
//---------------------------------------------------------------------------
} // namespace espiot::esp::actuators
//---------------------------------------------------------------------------