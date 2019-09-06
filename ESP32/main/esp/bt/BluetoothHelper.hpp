#pragma once

#include "esp_gap_ble_api.h"
#include "esp_gatts_api.h"
#include <cstdint>

//---------------------------------------------------------------------------
namespace espiot::esp {
//---------------------------------------------------------------------------
class BluetoothHelper {
    public:
    BluetoothHelper() = delete;

    static bool initBLE();
    static bool initGapGatt(uint16_t app_id);
    static const uint8_t* getMac();
    static void printMac();

    private:
    static void gatts_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t* param);
    static void gap_event_handler(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t* param);
};
//---------------------------------------------------------------------------
} // namespace espiot::esp
//---------------------------------------------------------------------------