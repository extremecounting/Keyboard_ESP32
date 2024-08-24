#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "driver/gpio.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"

#define input_pin_1 GPIO_NUM_36
#define input_pin_2 GPIO_NUM_34
#define input_pin_3 GPIO_NUM_39
#define input_pin_4 GPIO_NUM_35

#define output_pin_1 GPIO_NUM_26
#define output_pin_2 GPIO_NUM_27
#define output_pin_3 GPIO_NUM_14
#define output_pin_4 GPIO_NUM_12
#define output_pin_5 GPIO_NUM_13

volatile uint16_t lastInterruptTime = 0;
volatile bool interrupt_locked = false;
volatile uint8_t input_pin;
const uint8_t debouceDelay = 2;
int pinStates[4];
uint8_t outputIndex = 0;

char input_1_chars[5] = {'a', 'b', 'c', 'd', 'e'};
char input_2_chars[5] = {'f', 'g', 'h', 'i', 'j'};
char input_3_chars[5] = {'k', 'l', 'm', 'n', 'o'};
char input_4_chars[5] = {'p', 'q', 'r', 's', 't'};

void set_pins(uint8_t state) {
    gpio_set_level(output_pin_1, 1);
    gpio_set_level(output_pin_2, 1);
    gpio_set_level(output_pin_3, 1);
    gpio_set_level(output_pin_4, 1);
    gpio_set_level(output_pin_5, 1);
    return;
}
static void IRAM_ATTR input1(void* arg) {
    if ((esp_timer_get_time() / 1000) - lastInterruptTime < debouceDelay ||
        interrupt_locked) {
        return;
    }
    lastInterruptTime = esp_timer_get_time() / 1000;
    input_pin = 0;
    interrupt_locked = true;
    return;
}

static void IRAM_ATTR input2(void* arg) {
    if ((esp_timer_get_time() / 1000) - lastInterruptTime < debouceDelay ||
        interrupt_locked) {
        return;
    }
    lastInterruptTime = esp_timer_get_time() / 1000;
    input_pin = 1;
    interrupt_locked = true;
    return;
}

static void IRAM_ATTR input3(void* arg) {
    if ((esp_timer_get_time() / 1000) - lastInterruptTime < debouceDelay ||
        interrupt_locked) {
        return;
    }
    lastInterruptTime = esp_timer_get_time() / 1000;
    input_pin = 2;
    interrupt_locked = true;
    return;
}

static void IRAM_ATTR input4(void* arg) {
    if ((esp_timer_get_time() / 1000) - lastInterruptTime < debouceDelay ||
        interrupt_locked) {
        return;
    }
    lastInterruptTime = esp_timer_get_time() / 1000;
    input_pin = 3;
    interrupt_locked = true;
    return;
}

uint8_t find_output(uint8_t input_pin) {
    set_pins(0);
    gpio_set_level(output_pin_1, 1);
    if (gpio_get_level(input_pin) == 1) {
        gpio_set_level(output_pin_1, 0);
        return 0;
    }
    gpio_set_level(output_pin_1, 0);
    gpio_set_level(output_pin_2, 1);
    if (gpio_get_level(input_pin) == 1) {
        gpio_set_level(output_pin_2, 0);
        return 1;
    }
    gpio_set_level(output_pin_2, 0);
    gpio_set_level(output_pin_3, 1);
    if (gpio_get_level(input_pin) == 1) {
        gpio_set_level(output_pin_3, 0);
        return 2;
    }
    gpio_set_level(output_pin_3, 0);
    gpio_set_level(output_pin_4, 1);
    if (gpio_get_level(input_pin) == 1) {
        gpio_set_level(output_pin_4, 0);
        return 3;
    }
    gpio_set_level(output_pin_4, 0);
    gpio_set_level(output_pin_5, 1);
    if (gpio_get_level(input_pin) == 1) {
        gpio_set_level(output_pin_5, 0);
        return 4;
    }
    gpio_set_level(output_pin_5, 0);
    return 5;
}
void app_main() {
    // Initialize input pins
    gpio_set_direction(input_pin_1, GPIO_MODE_INPUT);
    gpio_set_direction(input_pin_2, GPIO_MODE_INPUT);
    gpio_set_direction(input_pin_3, GPIO_MODE_INPUT);
    gpio_set_direction(input_pin_4, GPIO_MODE_INPUT);

    gpio_set_pull_mode(input_pin_1, GPIO_PULLDOWN_ONLY);
    gpio_set_pull_mode(input_pin_2, GPIO_PULLDOWN_ONLY);
    gpio_set_pull_mode(input_pin_3, GPIO_PULLDOWN_ONLY);
    gpio_set_pull_mode(input_pin_4, GPIO_PULLDOWN_ONLY);

    gpio_install_isr_service(ESP_INTR_FLAG_LEVEL3);

    gpio_set_intr_type(input_pin_1, GPIO_INTR_POSEDGE);
    gpio_intr_enable(input_pin_1);
    gpio_isr_handler_add(input_pin_1, input1, NULL);
    // esp_intr_alloc(ETS_GPIO_INTR_SOURCE, ESP_INTR_FLAG_EDGE, input1, "test",
    // NULL);
    gpio_set_intr_type(input_pin_2, GPIO_INTR_POSEDGE);
    gpio_intr_enable(input_pin_2);
    gpio_isr_handler_add(input_pin_2, input2, NULL);

    // esp_intr_alloc(ETS_GPIO_INTR_SOURCE, 0, input2, "test", NULL);
    gpio_set_intr_type(input_pin_3, GPIO_INTR_POSEDGE);
    gpio_intr_enable(input_pin_3);
    gpio_isr_handler_add(input_pin_3, input3, NULL);
    // esp_intr_alloc(ETS_GPIO_INTR_SOURCE, 0, input3, "test", NULL);
    gpio_set_intr_type(input_pin_4, GPIO_INTR_POSEDGE);
    gpio_intr_enable(input_pin_4);
    gpio_isr_handler_add(input_pin_4, input4, NULL);
    // esp_intr_alloc(ETS_GPIO_INTR_SOURCE, 0, input4, "test", NULL);

    // Set output pins
    gpio_set_direction(output_pin_1, GPIO_MODE_OUTPUT);
    gpio_set_direction(output_pin_2, GPIO_MODE_OUTPUT);
    gpio_set_direction(output_pin_3, GPIO_MODE_OUTPUT);
    gpio_set_direction(output_pin_4, GPIO_MODE_OUTPUT);
    gpio_set_direction(output_pin_5, GPIO_MODE_OUTPUT);

    set_pins(1);

    while (1) {
        if (interrupt_locked) {
            uint8_t output;
            switch (input_pin) {
                case 0:
                    output = find_output(input_pin_1);
                    if (output != 5) {
                        // write letter
                    }
                    break;
                case 1:
                    output = find_output(input_pin_2);
                    if (output != 5) {
                        // write letter
                    }
                    break;
                case 2:
                    output = find_output(input_pin_3);
                    if (output != 5) {
                        // write letter
                    }
                    break;
                case 3:
                    output = find_output(input_pin_4);
                    if (output != 5) {
                        // write letter
                    }
                    break;
                default:
                    break;
            }
            set_pins(1);
            input_pin = 5;
            interrupt_locked = false;
        }
    }
}
