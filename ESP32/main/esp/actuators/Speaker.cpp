#include "Speaker.hpp"

//---------------------------------------------------------------------------
namespace espiot::esp::actuators {
//---------------------------------------------------------------------------
Speaker::Speaker(gpio_num_t signal) {
    /*
     * Prepare and set configuration of timers
     * that will be used by LED Controller
     */
    ledc_timer = (ledc_timer_config_t){
        .speed_mode = LEDC_HIGH_SPEED_MODE, // timer mode
        .duty_resolution = LEDC_TIMER_13_BIT, // resolution of PWM duty
        .timer_num = LEDC_TIMER_0, // timer index
        .freq_hz = 0, // frequency of PWM signal
        .clk_cfg = LEDC_AUTO_CLK // Auto select the source clock
    };
    // Set configuration of timer0 for high speed channels
    ledc_timer_config(&ledc_timer);

    ledc_channel = (ledc_channel_config_t){
        .gpio_num = signal,
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .channel = LEDC_CHANNEL_0,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = LEDC_TIMER_0,
        .duty = 100,
        .hpoint = 0,
    };

    ledc_channel_config(&ledc_channel);
    // ledc_fade_func_install(0);
    ledc_set_duty(ledc_channel.speed_mode, ledc_channel.channel, 1);
    ledc_update_duty(ledc_channel.speed_mode, ledc_channel.channel);
}

void Speaker::set(uint32_t freq_hz) {
    ledc_timer.freq_hz = freq_hz;
    ledc_timer_config(&ledc_timer);
}

uint32_t Speaker::get() {
    return ledc_timer.freq_hz;
}

//---------------------------------------------------------------------------
} // namespace espiot::esp::actuators
//---------------------------------------------------------------------------