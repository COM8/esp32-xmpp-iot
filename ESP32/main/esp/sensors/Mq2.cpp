#include "Mq2.hpp"
#include <iostream>

//---------------------------------------------------------------------------
namespace espiot::esp::sensors {
//---------------------------------------------------------------------------
/**
 * Default to ADC unit 1 and channel 6 (GPIO34) since ADC unit is used for Wi-Fi.
 * Reference: https://docs.espressif.com/projects/esp-idf/en/latest/api-reference/peripherals/adc.html
 **/
Mq2::Mq2() : Mq2(ADC_UNIT_1, ADC1_CHANNEL_6) {}

Mq2::Mq2(adc_unit_t unit, adc1_channel_t channel) : unit(unit),
                                                    channel(channel),
                                                    atten(ADC_ATTEN_DB_0),
                                                    adc_chars() {
    //Check if Two Point or Vref are burned into eFuse
    check_efuse();

    //Configure ADC
    if (unit == ADC_UNIT_1) {
        adc1_config_width(ADC_WIDTH_BIT_12);
        adc1_config_channel_atten(channel, atten);
    } else {
        adc2_config_channel_atten((adc2_channel_t) channel, atten);
    }

    //Characterize ADC
    esp_adc_cal_value_t val_type = esp_adc_cal_characterize(unit, atten, ADC_WIDTH_BIT_12, DEFAULT_VREF, &adc_chars);
    print_char_val_type(val_type);
}

uint32_t Mq2::read() {
    uint32_t adc_reading = 0;
    //Multisampling
    for (int i = 0; i < NO_OF_SAMPLES; i++) {
        if (unit == ADC_UNIT_1) {
            adc_reading += adc1_get_raw((adc1_channel_t) channel);
        } else {
            int raw;
            adc2_get_raw((adc2_channel_t) channel, ADC_WIDTH_BIT_12, &raw);
            adc_reading += raw;
        }
    }
    return adc_reading / NO_OF_SAMPLES;
}

void Mq2::check_efuse() {
    //Check TP is burned into eFuse
    if (esp_adc_cal_check_efuse(ESP_ADC_CAL_VAL_EFUSE_TP) == ESP_OK) {
        std::cout << "eFuse Two Point: Supported.\n";
    } else {
        std::cout << "eFuse Two Point: NOT Supported.\n";
    }

    //Check Vref is burned into eFuse
    if (esp_adc_cal_check_efuse(ESP_ADC_CAL_VAL_EFUSE_VREF) == ESP_OK) {
        std::cout << "eFuse Vref: Supported.\n";
    } else {
        std::cout << "eFuse Vref: NOT Supported.\n";
    }
}

void Mq2::print_char_val_type(esp_adc_cal_value_t val_type) {
    if (val_type == ESP_ADC_CAL_VAL_EFUSE_TP) {
        std::cout << "Characterized using Two Point Value.\n";
    } else if (val_type == ESP_ADC_CAL_VAL_EFUSE_VREF) {
        std::cout << "Characterized using eFuse Vref.\n";
    } else {
        std::cout << "Characterized using default Vref.\n";
    }
}
//---------------------------------------------------------------------------
} // namespace espiot::esp::sensors
//---------------------------------------------------------------------------