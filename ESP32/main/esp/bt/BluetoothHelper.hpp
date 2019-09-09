#pragma once

#include "esp_gap_ble_api.h"
#include "esp_gatts_api.h"
#include <cstdint>
#include <string>

//---------------------------------------------------------------------------
namespace espiot::esp {
//---------------------------------------------------------------------------
class BluetoothHelper {
    public:
    BluetoothHelper() = delete;

    static bool initBLE();
    static bool initGapGatt(uint16_t app_id, std::string deviceName);
    static const uint8_t* getMac();
    static void printMac();

    private:
    /* Attributes State Machine */
    enum {
        IDX_SVC,
        IDX_CHAR_A,
        IDX_CHAR_VAL_A,
        IDX_CHAR_CFG_A,

        IDX_CHAR_B,
        IDX_CHAR_VAL_B,

        IDX_CHAR_C,
        IDX_CHAR_VAL_C,

        HRS_IDX_NB,
    };
    static uint8_t adv_config_done;
    static esp_ble_adv_params_t adv_params;
    /* The length of adv data must be less than 31 bytes */
    static esp_ble_adv_data_t adv_data;
    static uint8_t service_uuid[16];
    // scan response data
    static esp_ble_adv_data_t scan_rsp_data;
    /* Full Database Description - Used to add attributes into the database */
    static const esp_gatts_attr_db_t gatt_db[HRS_IDX_NB];
    static const int ADV_CONFIG_FLAG;
    static const int SCAN_RSP_CONFIG_FLAG;

    /* Service */
    static const uint16_t GATTS_SERVICE_UUID_TEST;
    static const uint16_t GATTS_CHAR_UUID_TEST_A;
    static const uint16_t GATTS_CHAR_UUID_TEST_B;
    static const uint16_t GATTS_CHAR_UUID_TEST_C;

    static const uint16_t primary_service_uuid;
    static const uint16_t character_declaration_uuid;
    static const uint16_t character_client_config_uuid;
    static const uint8_t char_prop_read;
    static const uint8_t char_prop_write;
    static const uint8_t char_prop_read_write_notify;
    static const uint8_t heart_measurement_ccc[2];
    static const uint8_t char_value[4];

    static const uint16_t GATTS_DEMO_CHAR_VAL_LEN_MAX;
    static const int PREPARE_BUF_MAX_SIZE;
    static const int SVC_INST_ID;
    static const unsigned long CHAR_DECLARATION_SIZE;

    static std::string devName;

    static void gatts_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t* param);
    static void gap_event_handler(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t* param);
};
//---------------------------------------------------------------------------
} // namespace espiot::esp
//---------------------------------------------------------------------------