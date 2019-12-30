#pragma once

#include <driver/adc.h>
#include <driver/gpio.h>
#include <esp_adc_cal.h>

#define DEFAULT_VREF 1100 //Use adc2_vref_to_gpio() to obtain a better estimate
#define NO_OF_SAMPLES 64 //Multisampling

// Based on: https://github.com/espressif/esp-idf/blob/release/v4.0/examples/peripherals/adc/main/adc1_example_main.c
//---------------------------------------------------------------------------
namespace espiot::esp::sensors {
//---------------------------------------------------------------------------
class Mq2 {
    public:
    Mq2(adc_unit_t unit, adc1_channel_t channel);
    Mq2();

    uint32_t read();

    private:
    adc_unit_t unit;
    adc1_channel_t channel;
    const adc_atten_t atten;
    esp_adc_cal_characteristics_t adc_chars;

    void check_efuse();
    void print_char_val_type(esp_adc_cal_value_t val_type);
};
//---------------------------------------------------------------------------
} // namespace espiot::esp::sensors
//---------------------------------------------------------------------------