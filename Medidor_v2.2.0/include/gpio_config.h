/**
 * @file gpio_config.h
 * @brief Arquivo de inicialização às funcionalidas de leitura contínua de dos GPIOS para cálculo de grandezas de fase
 * 
 * @author --
 * @date 01 de Março de 2022
 */

#ifndef _GPIO_CONFIG_
    #define _GPIO_CONFIG_

    #include <Arduino.h>

    #include "freertos/FreeRTOS.h"
    #include "freertos/task.h"
    #include "freertos/semphr.h"

    #define SAMPLES_GPIO         1024 // numero de amostras por pino
    #define SAMPLING_PERIOD_GPIO 998  // 998+2=1000uS = 1000 Hz

    // TODO: Definição dos pinos dos comparadores
    #define COMP_VA GPIO_NUM_26
    #define COMP_VB GPIO_NUM_27
    #define COMP_VC GPIO_NUM_13
    #define COMP_IA GPIO_NUM_12
    #define COMP_IB GPIO_NUM_25
    #define COMP_IC GPIO_NUM_14
    #define GPIO_INPUT_PIN_SEL   (1ULL<<COMP_VA) | (1ULL<<COMP_VB) | (1ULL<<COMP_VC) | (1ULL<<COMP_IA) | (1ULL<<COMP_IB) | (1ULL<<COMP_IC)

    #define BIT_(shift) (1<<shift)

    /**
     * @brief Função que reúne as configurações do GPIO para permitir facilidade no uso.
     */
    void gpio_global_config();

    /**
     * @brief Realiza leitura dos GPIO e temporização das leituras.
     * 
     * @param buf buffer para os dados lidos (passado por referência para ser modificado)
     * @param time buffer para temporização (passado por referência para ser modificado)
     */
    void read_gpio_data(uint8_t (*buf)[SAMPLES_GPIO], uint32_t (*time)[SAMPLES_GPIO]);

#endif // _GPIO_CONFIG_