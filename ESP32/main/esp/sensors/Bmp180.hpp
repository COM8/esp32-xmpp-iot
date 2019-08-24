#pragma once

#include <string>
#include <driver/gpio.h>
#include <driver/i2c.h>
//---------------------------------------------------------------------------
namespace espiot::esp::sensors {
//---------------------------------------------------------------------------
// Based on: https://github.com/nkolban/esp32-snippets/blob/master/hardware/temperature%20and%20pressure/bmp180.c
class Bmp180 {
    private:
    const char BMP180_ADDRESS = 0x77;
    i2c_config_t conf;

    enum Register {
        CAL_AC1 = 0xAA, // R   Calibration data (16 bits)
        CAL_AC2 = 0xAC, // R   Calibration data (16 bits)
        CAL_AC3 = 0xAE, // R   Calibration data (16 bits)
        CAL_AC4 = 0xB0, // R   Calibration data (16 bits)
        CAL_AC5 = 0xB2, // R   Calibration data (16 bits)
        CAL_AC6 = 0xB4, // R   Calibration data (16 bits)
        CAL_B1 = 0xB6, // R   Calibration data (16 bits)
        CAL_B2 = 0xB8, // R   Calibration data (16 bits)
        CAL_MB = 0xBA, // R   Calibration data (16 bits)
        CAL_MC = 0xBC, // R   Calibration data (16 bits)
        CAL_MD = 0xBE, // R   Calibration data (16 bits)
        CHIPID = 0xD0,
        VERSION = 0xD1,
        SOFTRESET = 0xE0,
        CONTROL = 0xF4,
        TEMPDATA = 0xF6,
        PRESSUREDATA = 0xF6,
        READTEMPCMD = 0x2E,
        READPRESSURECMD = 0x34 // 0011 0100
    };

    enum Mode {
        ULTRALOWPOWER = 0,
        STANDARD = 1,
        HIGHRES = 2,
        ULTRAHIGHRES = 3
    };

    short readRegister16(Register reg);
    int readRegister24(Register reg);
    uint32_t readUncompensatedTemp();
    uint32_t readUncompensatedPressure(Mode mode);

    public:
    Bmp180(gpio_num_t sda, gpio_num_t scl);
    // Reads the current temperature and returns it in Celsius.
    double readTemp();
    // Reads the current pressur and returns it in Pascals.
    int32_t readPressure();
    double celsiusToFahrenheit(double tempCelsius);
    double pascalsToInHg(int32_t pressurePascals);
};
//---------------------------------------------------------------------------
} // namespace espiot::esp::sensors
//---------------------------------------------------------------------------