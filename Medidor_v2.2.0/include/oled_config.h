/**
 * @file oled_config.h
 * @brief Arquivo de inicialização das funções de uso do cartão display OLED.
 * 
 * @author --
 * @date 05 de Maio de 2021
 */

#ifndef _OLED_CONFIG_
    #define _OLED_CONFIG_

    #include <Arduino.h>
    #include <stdlib.h>
    #include <stdio.h>
    #include <string.h>
    
    #include "driver/gpio.h"
    #include "esp_err.h" // error codes and helper functions
    #include "freertos/FreeRTOS.h"
    #include "freertos/task.h"

    #include "SSD1306Wire.h"
    
    #include "SmartReadings.h"
    #include "definitions.h"

    #define OLED_SWITCH GPIO_NUM_16
    #define OLED_BUTTON GPIO_NUM_17
    #define OLED_INPUT_PIN_SEL (1ULL<<OLED_SWITCH) | (1ULL<<OLED_BUTTON)
    #define ESP_INTR_FLAG_DEFAULT 0

    #define CHAR_TO_STRING(x) String(x)

    #define SCREEN_WIDTH 128
    #define SCREEN_HEIGHT 64
    #define CHAR_WIDTH 16
    #define CHAR_HEIGHT 22

    #define LEFT_OF_SCREEN 0
    #define MIDDLE_OF_SCREEN SCREEN_WIDTH/2
    #define TOP_ROW CHAR_HEIGHT*0
    #define MIDDLE_ROW CHAR_HEIGHT*1
    #define BOTTOM_ROW CHAR_HEIGHT*2

    typedef enum {
        OLED_TOGGLE_MODE=0,
        OLED_SEQUENTIAL_MODE=1,
        OLED_BLANK_MODE=2,
    } oled_mode_t;

    typedef enum {
        OLED_MAIN_SCREEN=0,
        OLED_VOLTAGE_SCREEN=1,
        OLED_CURRENT_SCREEN=2,
        OLED_ACTIVE_POWER_SCREEN=3,
        OLED_REACTIVE_POWER_SCREEN=4,
        OLED_APARENT_POWER_SCREEN=5,
        OLED_POWER_FACTOR_SCREEN=6,
        OLED_FREQUENCY_SCREEN=7,
        OLED_BLANK_SCREEN=8,
    } oled_screen_t;

    void oled_global_config(SSD1306Wire &display);

    void display_loop(SSD1306Wire &display, SmartReadings readings, oled_screen_t &screen, oled_mode_t mode);

    
#endif // _OLED_CONFIG_