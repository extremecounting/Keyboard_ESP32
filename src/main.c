#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "stdbool.h"

#define input_pin_1 GPIO_NUM_36
#define input_pin_2 GPIO_NUM_34
#define input_pin_3 GPIO_NUM_39
#define input_pin_4 GPIO_NUM_35

#define output_pin_1 GPIO_NUM_26
#define output_pin_2 GPIO_NUM_27
#define output_pin_3 GPIO_NUM_14
#define output_pin_4 GPIO_NUM_12
#define output_pin_5 GPIO_NUM_13

volatile unsigned long lastInterruptTime = 0;
volatile bool interrupt_locked;
volatile int input_pin;
const int debouceDelay = 2;
int pinStates[4];
uint8_t outputIndex = 0;

char input_1_chars[5] = {'a', 'b', 'c', 'd', 'e'};
char input_2_chars[5] = {'f', 'g', 'h', 'i', 'j'};
char input_3_chars[5] = {'k', 'l', 'm', 'n', 'o'};
char input_4_chars[5] = {'p', 'q', 'r', 's', 't'};

void app_main() {
    /*gpio_config_t input_pin_config = {};
    input_pin_config.intr_type = GPIO_INTR_POSEDGE;
    input_pin_config.mode = GPIO_MODE_INPUT;
    input_pin_config.pin_bit_mask = GPIO_NUM_*/
    gpio_set_direction()
}