#include "EspIoT.hpp"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"

#define BLINK_GPIO_RED (gpio_num_t)CONFIG_BLINK_GPIO_RED
#define BLINK_GPIO_BLUE (gpio_num_t)CONFIG_BLINK_GPIO_BLUE
#define BLINK_GPIO_GREEN (gpio_num_t)CONFIG_BLINK_GPIO_GREEN

void app_main(void) {
    gpio_pad_select_gpio(BLINK_GPIO_RED);
    /* Set the GPIO as a push/pull output */
    gpio_set_direction(BLINK_GPIO_RED, GPIO_MODE_OUTPUT);
    while(1) {
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
