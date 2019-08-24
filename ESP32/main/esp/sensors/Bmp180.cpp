#include "Bmp180.hpp"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

//---------------------------------------------------------------------------
namespace espiot::esp::sensors {
//---------------------------------------------------------------------------
short Bmp180::readRegister16(Register reg) {
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (BMP180_ADDRESS << 1) | I2C_MASTER_WRITE, 1 /* expect ack */);
    i2c_master_write_byte(cmd, reg, 1);
    i2c_master_stop(cmd);
    i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);

    uint8_t msb;
    uint8_t lsb;
    cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (BMP180_ADDRESS << 1) | I2C_MASTER_READ, 1 /* expect ack */);
    i2c_master_read_byte(cmd, &msb, static_cast<i2c_ack_type_t>(1));
    i2c_master_read_byte(cmd, &lsb, static_cast<i2c_ack_type_t>(0));
    i2c_master_stop(cmd);
    i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);
    return static_cast<short>((msb << 8) | lsb);
}

int Bmp180::readRegister24(Register reg) {
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (BMP180_ADDRESS << 1) | I2C_MASTER_WRITE, 1 /* expect ack */);
    i2c_master_write_byte(cmd, reg, 1);
    i2c_master_stop(cmd);
    i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);

    uint8_t msb;
    uint8_t lsb;
    uint8_t xlsb;
    cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (BMP180_ADDRESS << 1) | I2C_MASTER_READ, 1 /* expect ack */);
    i2c_master_read_byte(cmd, &msb, static_cast<i2c_ack_type_t>(1));
    i2c_master_read_byte(cmd, &lsb, static_cast<i2c_ack_type_t>(1));
    i2c_master_read_byte(cmd, &xlsb, static_cast<i2c_ack_type_t>(0));
    i2c_master_stop(cmd);
    i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);
    return static_cast<int>((msb << 16) | (lsb << 8) | xlsb);
}

uint32_t Bmp180::readUncompensatedTemp() {
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (BMP180_ADDRESS << 1) | I2C_MASTER_WRITE, 1 /* expect ack */);
    i2c_master_write_byte(cmd, Register::CONTROL, 1);
    i2c_master_write_byte(cmd, Register::READTEMPCMD, 1);
    i2c_master_stop(cmd);
    i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);
    vTaskDelay(5 / portTICK_PERIOD_MS);
    return static_cast<unsigned short>(readRegister16(Register::TEMPDATA));
}

uint32_t Bmp180::readUncompensatedPressure(Mode mode) {
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (BMP180_ADDRESS << 1) | I2C_MASTER_WRITE, 1 /* expect ack */);
    i2c_master_write_byte(cmd, Register::CONTROL, 1);
    i2c_master_write_byte(cmd, Register::READPRESSURECMD + (mode << 6), 1);
    i2c_master_stop(cmd);
    i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);
    switch (mode) {
        case Mode::ULTRALOWPOWER:
            vTaskDelay(5 / portTICK_PERIOD_MS);
            break;
        case Mode::STANDARD:
            vTaskDelay(8 / portTICK_PERIOD_MS);
            break;
        case Mode::HIGHRES:
            vTaskDelay(14 / portTICK_PERIOD_MS);
            break;
        case Mode::ULTRAHIGHRES:
        default:
            vTaskDelay(26 / portTICK_PERIOD_MS);
            break;
    }
    long ret;
    if (mode != Mode::ULTRAHIGHRES) {
        ret = readRegister24(Register::PRESSUREDATA);
    } else {
        ret = readRegister24(Register::PRESSUREDATA);
    }
    return ret >> (8 - mode);
}

Bmp180::Bmp180(gpio_num_t sda, gpio_num_t scl) {
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = sda;
    conf.scl_io_num = scl;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = 100000;
    i2c_param_config(I2C_NUM_0, &conf);

    i2c_driver_install(I2C_NUM_0, I2C_MODE_MASTER, 0, 0, 0);
}

// Reads the current temperature and returns it in Celsius.
double Bmp180::readTemp() {
    unsigned short AC5 = readRegister16(Register::CAL_AC5);
    unsigned short AC6 = readRegister16(Register::CAL_AC6);
    short MC = readRegister16(Register::CAL_MC);
    short MD = readRegister16(Register::CAL_MD);

    int32_t UT = readUncompensatedTemp();

    int32_t X1 = (UT - static_cast<int32_t>(AC6)) * static_cast<int32_t>(AC5) >> 15;
    int32_t X2 = (static_cast<int32_t>(MC) << 11) / (X1 + static_cast<int32_t>(MD));
    int32_t B5 = X1 + X2;
    return ((B5 + 8) >> 4) / 10.0;
}

// Reads the current pressur and returns it in Pascals.
int32_t Bmp180::readPressure() {
    short AC1 = readRegister16(Register::CAL_AC1);
    short AC2 = readRegister16(Register::CAL_AC2);
    short AC3 = readRegister16(Register::CAL_AC3);
    unsigned short AC4 = readRegister16(Register::CAL_AC4);
    unsigned short AC5 = readRegister16(Register::CAL_AC5);
    unsigned short AC6 = readRegister16(Register::CAL_AC6);
    short B1 = readRegister16(Register::CAL_B1);
    short B2 = readRegister16(Register::CAL_B2);
    short MC = readRegister16(Register::CAL_MC);
    short MD = readRegister16(Register::CAL_MD);

    Mode mode = Mode::STANDARD;
    int32_t UT = readUncompensatedTemp();
    int32_t UP = readUncompensatedPressure(mode);

    int32_t X1 = (UT - static_cast<int32_t>(AC6)) * static_cast<int32_t>(AC5) >> 15;
    int32_t X2 = (static_cast<int32_t>(MC) << 11) / (X1 + static_cast<int32_t>(MD));
    int32_t B5 = X1 + X2;

    int32_t B6 = B5 - 4000;
    X1 = (B2 * (B6 * B6) >> 12) >> 11;
    X2 = (AC2 * B6) >> 11;
    int32_t X3 = X1 + X2;
    int32_t B3 = (((static_cast<int32_t>(AC1) * 4 + X3) << mode) + 2) >> 2;
    X1 = (AC3 * B6) >> 13;
    X2 = (B1 * ((B6 * B6) >> 12)) >> 16;
    X3 = ((X1 + X2) + 2) >> 2;
    uint32_t B4 = (static_cast<int32_t>(AC4) * static_cast<int32_t>(X3 + 32768)) >> 15;
    uint32_t B7 = ((static_cast<int32_t>(UP) - B3) * (50000 >> mode));
    int32_t P;
    if (B7 < 0x80000000) {
        P = (B7 << 1) / B4;
    } else {
        P = (B7 / B4) << 1;
    }
    X1 = (P >> 8) * (P >> 8);
    X1 = (X1 * 3038) >> 16;
    X2 = (-7357 * P) >> 16;
    return P + ((X1 + X2 + 3791) >> 4);
}

double Bmp180::celsiusToFahrenheit(double tempCelsius) {
    return tempCelsius * 9 / 5 + 32.0;
}
double Bmp180::pascalsToInHg(int32_t pressurePascals) {
    return pressurePascals * 0.00029530;
}
//---------------------------------------------------------------------------
} // namespace espiot::esp::sensors
//---------------------------------------------------------------------------