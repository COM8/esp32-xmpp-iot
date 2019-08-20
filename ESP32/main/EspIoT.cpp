#include "EspIoT.hpp"
#include <cstdio>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"

//---------------------------------------------------------------------------
namespace espiot {
//---------------------------------------------------------------------------

#define BLINK_GPIO_RED (gpio_num_t)CONFIG_BLINK_GPIO_RED
#define BLINK_GPIO_BLUE (gpio_num_t)CONFIG_BLINK_GPIO_BLUE
#define BLINK_GPIO_GREEN (gpio_num_t)CONFIG_BLINK_GPIO_GREEN

void app_main(void) {

    EspIoT espIoT;
    espIoT.run();

    printf("Hello world!\n");

    /* Print chip information */
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);
    printf("This is ESP32 chip with %d CPU cores, WiFi%s%s, ",
            chip_info.cores,
            (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
            (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");

    printf("silicon revision %d, ", chip_info.revision);

    printf("%dMB %s flash\n", spi_flash_get_chip_size() / (1024 * 1024),
            (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");

    for (int i = 10; i >= 0; i--) {
        printf("Restarting in %d seconds...\n", i);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    printf("Restarting now.\n");
    fflush(stdout);
    esp_restart();
}

void EspIoT::run() {
    ledHandler.blink();
}
//---------------------------------------------------------------------------
} // namespace espiot
//---------------------------------------------------------------------------
