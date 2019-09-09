#include "BluetoothHelper.hpp"
#include "esp_bt.h"
#include "esp_bt_device.h"
#include "esp_bt_main.h"
#include "esp_gatt_common_api.h"
#include "nvs_flash.h"

//---------------------------------------------------------------------------
namespace espiot::esp {
//---------------------------------------------------------------------------
uint8_t BluetoothHelper::adv_config_done = 0;

esp_ble_adv_params_t BluetoothHelper::adv_params = {
    .adv_int_min = 0x20,
    .adv_int_max = 0x40,
    .adv_type = ADV_TYPE_IND,
    .own_addr_type = BLE_ADDR_TYPE_PUBLIC,
    .channel_map = ADV_CHNL_ALL,
    .adv_filter_policy = ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY,
};

esp_ble_adv_data_t BluetoothHelper::adv_data = {
    .set_scan_rsp = false,
    .include_name = true,
    .include_txpower = true,
    .min_interval = 0x0006, //slave connection min interval, Time = min_interval * 1.25 msec
    .max_interval = 0x0010, //slave connection max interval, Time = max_interval * 1.25 msec
    .appearance = 0x00,
    .manufacturer_len = 0, //TEST_MANUFACTURER_DATA_LEN,
    .p_manufacturer_data = NULL, //test_manufacturer,
    .service_data_len = 0,
    .p_service_data = NULL,
    .service_uuid_len = sizeof(service_uuid),
    .p_service_uuid = service_uuid,
    .flag = (ESP_BLE_ADV_FLAG_GEN_DISC | ESP_BLE_ADV_FLAG_BREDR_NOT_SPT),
};

// scan response data
esp_ble_adv_data_t BluetoothHelper::scan_rsp_data = {
    .set_scan_rsp = true,
    .include_name = true,
    .include_txpower = true,
    .min_interval = 0x0006,
    .max_interval = 0x0010,
    .appearance = 0x00,
    .manufacturer_len = 0, //TEST_MANUFACTURER_DATA_LEN,
    .p_manufacturer_data = NULL, //&test_manufacturer[0],
    .service_data_len = 0,
    .p_service_data = NULL,
    .service_uuid_len = sizeof(service_uuid),
    .p_service_uuid = service_uuid,
    .flag = (ESP_BLE_ADV_FLAG_GEN_DISC | ESP_BLE_ADV_FLAG_BREDR_NOT_SPT),
};

uint8_t BluetoothHelper::service_uuid[16] = {
    /* LSB <--------------------------------------------------------------------------------> MSB */
    //first uuid, 16bit, [12],[13] is the value
    0xfb,
    0x34,
    0x9b,
    0x5f,
    0x80,
    0x00,
    0x00,
    0x80,
    0x00,
    0x10,
    0x00,
    0x00,
    0xFF,
    0x00,
    0x00,
    0x00,
};

/* Full Database Description - Used to add attributes into the database */
const esp_gatts_attr_db_t BluetoothHelper::gatt_db[HRS_IDX_NB] =
    {
        // Service Declaration
        [IDX_SVC] =
            {{ESP_GATT_AUTO_RSP}, {ESP_UUID_LEN_16, (uint8_t*) &primary_service_uuid, ESP_GATT_PERM_READ, sizeof(uint16_t), sizeof(GATTS_SERVICE_UUID_TEST), (uint8_t*) &GATTS_SERVICE_UUID_TEST}},

        /* Characteristic Declaration */
        [IDX_CHAR_A] =
            {{ESP_GATT_AUTO_RSP}, {ESP_UUID_LEN_16, (uint8_t*) &character_declaration_uuid, ESP_GATT_PERM_READ, static_cast<uint16_t>(CHAR_DECLARATION_SIZE), static_cast<uint16_t>(CHAR_DECLARATION_SIZE), (uint8_t*) &char_prop_read_write_notify}},

        /* Characteristic Value */
        [IDX_CHAR_VAL_A] =
            {{ESP_GATT_AUTO_RSP}, {ESP_UUID_LEN_16, (uint8_t*) &GATTS_CHAR_UUID_TEST_A, ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE, GATTS_DEMO_CHAR_VAL_LEN_MAX, sizeof(char_value), (uint8_t*) char_value}},

        /* Client Characteristic Configuration Descriptor */
        [IDX_CHAR_CFG_A] =
            {{ESP_GATT_AUTO_RSP}, {ESP_UUID_LEN_16, (uint8_t*) &character_client_config_uuid, ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE, sizeof(uint16_t), sizeof(heart_measurement_ccc), (uint8_t*) heart_measurement_ccc}},

        /* Characteristic Declaration */
        [IDX_CHAR_B] =
            {{ESP_GATT_AUTO_RSP}, {ESP_UUID_LEN_16, (uint8_t*) &character_declaration_uuid, ESP_GATT_PERM_READ, static_cast<uint16_t>(CHAR_DECLARATION_SIZE), static_cast<uint16_t>(CHAR_DECLARATION_SIZE), (uint8_t*) &char_prop_read}},

        /* Characteristic Value */
        [IDX_CHAR_VAL_B] =
            {{ESP_GATT_AUTO_RSP}, {ESP_UUID_LEN_16, (uint8_t*) &GATTS_CHAR_UUID_TEST_B, ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE, GATTS_DEMO_CHAR_VAL_LEN_MAX, sizeof(char_value), (uint8_t*) char_value}},

        /* Characteristic Declaration */
        [IDX_CHAR_C] =
            {{ESP_GATT_AUTO_RSP}, {ESP_UUID_LEN_16, (uint8_t*) &character_declaration_uuid, ESP_GATT_PERM_READ, static_cast<uint16_t>(CHAR_DECLARATION_SIZE), static_cast<uint16_t>(CHAR_DECLARATION_SIZE), (uint8_t*) &char_prop_write}},

        /* Characteristic Value */
        [IDX_CHAR_VAL_C] =
            {{ESP_GATT_AUTO_RSP}, {ESP_UUID_LEN_16, (uint8_t*) &GATTS_CHAR_UUID_TEST_C, ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE, GATTS_DEMO_CHAR_VAL_LEN_MAX, sizeof(char_value), (uint8_t*) char_value}},

};
const int BluetoothHelper::ADV_CONFIG_FLAG = (1 << 0);
const int BluetoothHelper::SCAN_RSP_CONFIG_FLAG = (1 << 1);

const uint16_t BluetoothHelper::GATTS_SERVICE_UUID_TEST = 0x00FF;
const uint16_t BluetoothHelper::GATTS_CHAR_UUID_TEST_A = 0xFF01;
const uint16_t BluetoothHelper::GATTS_CHAR_UUID_TEST_B = 0xFF02;
const uint16_t BluetoothHelper::GATTS_CHAR_UUID_TEST_C = 0xFF03;

const uint16_t BluetoothHelper::primary_service_uuid = ESP_GATT_UUID_PRI_SERVICE;
const uint16_t BluetoothHelper::character_declaration_uuid = ESP_GATT_UUID_CHAR_DECLARE;
const uint16_t BluetoothHelper::character_client_config_uuid = ESP_GATT_UUID_CHAR_CLIENT_CONFIG;
const uint8_t BluetoothHelper::char_prop_read = ESP_GATT_CHAR_PROP_BIT_READ;
const uint8_t BluetoothHelper::char_prop_write = ESP_GATT_CHAR_PROP_BIT_WRITE;
const uint8_t BluetoothHelper::char_prop_read_write_notify = ESP_GATT_CHAR_PROP_BIT_WRITE | ESP_GATT_CHAR_PROP_BIT_READ | ESP_GATT_CHAR_PROP_BIT_NOTIFY;
const uint8_t BluetoothHelper::heart_measurement_ccc[2] = {0x00, 0x00};
const uint8_t BluetoothHelper::char_value[4] = {0x11, 0x22, 0x33, 0x44};

const uint16_t BluetoothHelper::GATTS_DEMO_CHAR_VAL_LEN_MAX = 500;
const int BluetoothHelper::PREPARE_BUF_MAX_SIZE = 1024;
const int BluetoothHelper::SVC_INST_ID = 0;
const unsigned long BluetoothHelper::CHAR_DECLARATION_SIZE = sizeof(uint8_t);

std::string BluetoothHelper::devName = "";

bool BluetoothHelper::initBLE() {
    printf("Initializing BLE...\n");
    /* Initialize NVS. */
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    ESP_ERROR_CHECK(esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT));

    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    ret = esp_bt_controller_init(&bt_cfg);
    if (ret) {
        printf("%s enable controller failed: %s\n", __func__, esp_err_to_name(ret));
        return false;
    }

    ret = esp_bt_controller_enable(ESP_BT_MODE_BLE);
    if (ret) {
        printf("%s enable controller failed: %s\n", __func__, esp_err_to_name(ret));
        return false;
    }

    ret = esp_bluedroid_init();
    if (ret) {
        printf("%s init bluetooth failed: %s\n", __func__, esp_err_to_name(ret));
        return false;
    }

    ret = esp_bluedroid_enable();
    if (ret) {
        printf("%s enable bluetooth failed: %s\n", __func__, esp_err_to_name(ret));
        return false;
    }
    printf("BLE initialized.\n");
    return true;
}

bool BluetoothHelper::initGapGatt(uint16_t app_id, std::string deviceName) {
    printf("Initializing BLE GAP and GATT...\n");
    devName = deviceName;
    esp_err_t ret;
    ret = esp_ble_gatts_register_callback(gatts_event_handler);
    if (ret) {
        printf("gatts register error, error code = %x\n", ret);
        return false;
    }

    ret = esp_ble_gap_register_callback(gap_event_handler);
    if (ret) {
        printf("gap register error, error code = %x\n", ret);
        return false;
    }

    ret = esp_ble_gatts_app_register(app_id);
    if (ret) {
        printf("gatts app register error, error code = %x\n", ret);
        return false;
    }

    esp_err_t local_mtu_ret = esp_ble_gatt_set_local_mtu(500);
    if (local_mtu_ret) {
        printf("set local  MTU failed, error code = %x\n", local_mtu_ret);
        return false;
    }
    printf("BLE GAP and GATT initialized.\n");
    return true;
}

const uint8_t* BluetoothHelper::getMac() {
    return esp_bt_dev_get_address();
}

void BluetoothHelper::printMac() {
    const uint8_t* mac = getMac();

    printf("BT MAC: ");
    for (int i = 0; i < 6; i++) {
        printf("%02X", static_cast<int>(mac[i]));
        if (i < 5) {
            printf(":");
        }
    }
    printf("\n");
}

void BluetoothHelper::gatts_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t* param) {
    switch (event) {
        case ESP_GATTS_REG_EVT: {
            esp_err_t set_dev_name_ret = esp_ble_gap_set_device_name(devName.c_str());
            if (set_dev_name_ret) {
                printf("set device name failed, error code = %x", set_dev_name_ret);
            }
            //config adv data
            esp_err_t ret = esp_ble_gap_config_adv_data(&adv_data);
            if (ret) {
                printf("config adv data failed, error code = %x", ret);
            }
            adv_config_done |= ADV_CONFIG_FLAG;
            //config scan response data
            ret = esp_ble_gap_config_adv_data(&scan_rsp_data);
            if (ret) {
                printf("config scan response data failed, error code = %x", ret);
            }
            adv_config_done |= SCAN_RSP_CONFIG_FLAG;
            esp_err_t create_attr_ret = esp_ble_gatts_create_attr_tab(gatt_db, gatts_if, HRS_IDX_NB, SVC_INST_ID);
            if (create_attr_ret) {
                printf("create attr table failed, error code = %x", create_attr_ret);
            }
        } break;
        case ESP_GATTS_READ_EVT:
            printf("ESP_GATTS_READ_EVT");
            break;
        case ESP_GATTS_EXEC_WRITE_EVT:
            // the length of gattc prepare write data must be less than GATTS_DEMO_CHAR_VAL_LEN_MAX.
            printf("ESP_GATTS_EXEC_WRITE_EVT");
            break;
        case ESP_GATTS_MTU_EVT:
            printf("ESP_GATTS_MTU_EVT, MTU %d", param->mtu.mtu);
            break;
        case ESP_GATTS_CONF_EVT:
            printf("ESP_GATTS_CONF_EVT, status = %d, attr_handle %d", param->conf.status, param->conf.handle);
            break;
        case ESP_GATTS_START_EVT:
            printf("SERVICE_START_EVT, status %d, service_handle %d", param->start.status, param->start.service_handle);
            break;
        case ESP_GATTS_CONNECT_EVT:
            printf("ESP_GATTS_CONNECT_EVT, conn_id = %d", param->connect.conn_id);
            break;
        case ESP_GATTS_DISCONNECT_EVT:
            printf("ESP_GATTS_DISCONNECT_EVT, reason = 0x%x", param->disconnect.reason);
            esp_ble_gap_start_advertising(&adv_params);
            break;
        case ESP_GATTS_WRITE_EVT:
        case ESP_GATTS_UNREG_EVT:
        case ESP_GATTS_CREATE_EVT:
        case ESP_GATTS_ADD_INCL_SRVC_EVT:
        case ESP_GATTS_ADD_CHAR_EVT:
        case ESP_GATTS_ADD_CHAR_DESCR_EVT:
        case ESP_GATTS_DELETE_EVT:
        case ESP_GATTS_STOP_EVT:
        case ESP_GATTS_OPEN_EVT:
        case ESP_GATTS_CANCEL_OPEN_EVT:
        case ESP_GATTS_CLOSE_EVT:
        case ESP_GATTS_LISTEN_EVT:
        case ESP_GATTS_CONGEST_EVT:
        case ESP_GATTS_RESPONSE_EVT:
        case ESP_GATTS_CREAT_ATTR_TAB_EVT:
        case ESP_GATTS_SET_ATTR_VAL_EVT:
        case ESP_GATTS_SEND_SERVICE_CHANGE_EVT:
            break;
    }
}

void BluetoothHelper::gap_event_handler(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t* param) {
    switch (event) {
        case ESP_GAP_BLE_ADV_DATA_SET_COMPLETE_EVT:
            adv_config_done &= (~ADV_CONFIG_FLAG);
            if (adv_config_done == 0) {
                esp_ble_gap_start_advertising(&adv_params);
            }
            break;
        case ESP_GAP_BLE_SCAN_RSP_DATA_SET_COMPLETE_EVT:
            adv_config_done &= (~SCAN_RSP_CONFIG_FLAG);
            if (adv_config_done == 0) {
                esp_ble_gap_start_advertising(&adv_params);
            }
            break;
        case ESP_GAP_BLE_ADV_START_COMPLETE_EVT:
            /* advertising start complete event to indicate advertising start successfully or failed */
            if (param->adv_start_cmpl.status != ESP_BT_STATUS_SUCCESS) {
                printf("advertising start failed");
            } else {
                printf("advertising start successfully");
            }
            break;
        case ESP_GAP_BLE_ADV_STOP_COMPLETE_EVT:
            if (param->adv_stop_cmpl.status != ESP_BT_STATUS_SUCCESS) {
                printf("Advertising stop failed");
            } else {
                printf("Stop adv successfully\n");
            }
            break;
        case ESP_GAP_BLE_UPDATE_CONN_PARAMS_EVT:
            printf("update connection params status = %d, min_int = %d, max_int = %d,conn_int = %d,latency = %d, timeout = %d",
                   param->update_conn_params.status,
                   param->update_conn_params.min_int,
                   param->update_conn_params.max_int,
                   param->update_conn_params.conn_int,
                   param->update_conn_params.latency,
                   param->update_conn_params.timeout);
            break;
        default:
            break;
    }
}
//---------------------------------------------------------------------------
} // namespace espiot::esp
//---------------------------------------------------------------------------