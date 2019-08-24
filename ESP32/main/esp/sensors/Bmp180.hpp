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

    enum {
        BMP085_REGISTER_CAL_AC1 = 0xAA, // R   Calibration data (16 bits)
        BMP085_REGISTER_CAL_AC2 = 0xAC, // R   Calibration data (16 bits)
        BMP085_REGISTER_CAL_AC3 = 0xAE, // R   Calibration data (16 bits)
        BMP085_REGISTER_CAL_AC4 = 0xB0, // R   Calibration data (16 bits)
        BMP085_REGISTER_CAL_AC5 = 0xB2, // R   Calibration data (16 bits)
        BMP085_REGISTER_CAL_AC6 = 0xB4, // R   Calibration data (16 bits)
        BMP085_REGISTER_CAL_B1 = 0xB6, // R   Calibration data (16 bits)
        BMP085_REGISTER_CAL_B2 = 0xB8, // R   Calibration data (16 bits)
        BMP085_REGISTER_CAL_MB = 0xBA, // R   Calibration data (16 bits)
        BMP085_REGISTER_CAL_MC = 0xBC, // R   Calibration data (16 bits)
        BMP085_REGISTER_CAL_MD = 0xBE, // R   Calibration data (16 bits)
        BMP085_REGISTER_CHIPID = 0xD0,
        BMP085_REGISTER_VERSION = 0xD1,
        BMP085_REGISTER_SOFTRESET = 0xE0,
        BMP085_REGISTER_CONTROL = 0xF4,
        BMP085_REGISTER_TEMPDATA = 0xF6,
        BMP085_REGISTER_PRESSUREDATA = 0xF6,
        BMP085_REGISTER_READTEMPCMD = 0x2E,
        BMP085_REGISTER_READPRESSURECMD = 0x34 // 0011 0100
    };

    typedef enum {
        BMP085_MODE_ULTRALOWPOWER = 0,
        BMP085_MODE_STANDARD = 1,
        BMP085_MODE_HIGHRES = 2,
        BMP085_MODE_ULTRAHIGHRES = 3
    } bmp085_mode_t;

    short readRegister16(uint8_t reg);
    int readRegister24(uint8_t reg);

    uint32_t readUncompensatedTemp();
    uint32_t readUncompensatedPressure(uint32_t mode);

    double centigrade_to_fahrenheit(double centigrade);
    double pascals_to_inHg(double pressure);

    i2c_config_t conf;

    public:
    Bmp180(gpio_num_t sda, gpio_num_t scl);

    double readtemp();
    int32_t readPressure();
};
//---------------------------------------------------------------------------
} // namespace espiot::esp::sensors
//---------------------------------------------------------------------------