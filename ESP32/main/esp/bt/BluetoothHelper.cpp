#include "BluetoothHelper.hpp"
#include "esp_bt.h"
#include "esp_bt_device.h"
#include "esp_bt_main.h"
#include "esp_gatt_common_api.h"
#include "nvs_flash.h"

//---------------------------------------------------------------------------
namespace espiot::esp {
//---------------------------------------------------------------------------
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

bool BluetoothHelper::initGapGatt(uint16_t app_id) {
    printf("Initializing BLE GAP and GATT...\n");
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
}

void BluetoothHelper::gap_event_handler(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t* param) {
}
//---------------------------------------------------------------------------
} // namespace espiot::esp
//---------------------------------------------------------------------------