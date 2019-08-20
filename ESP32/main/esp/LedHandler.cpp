#include "LedHandler.hpp"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sdkconfig.h"

//---------------------------------------------------------------------------
namespace espiot::esp {
//---------------------------------------------------------------------------
#define BLINK_GPIO_RED (gpio_num_t) 14

void LedHandler::blink() {
    gpio_pad_select_gpio(BLINK_GPIO_RED);
    /* Set the GPIO as a push/pull output */
    gpio_set_direction(BLINK_GPIO_RED, GPIO_MODE_OUTPUT);
    while (true) {
        /* Blink off (output low) */
        printf("Turning off the LED\n");
        gpio_set_level(BLINK_GPIO_RED, 0);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        /* Blink on (output high) */
        printf("Turning on the LED\n");
        gpio_set_level(BLINK_GPIO_RED, 1);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
//---------------------------------------------------------------------------
} // namespace espiot::esp
//---------------------------------------------------------------------------