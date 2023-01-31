/**
 * @file adc_config.h
 * @brief Arquivo de inicialização às funcionalidas de leitura contínua de múltiplos canais
 *        e em alta velocidade do ADC SAR do ESP32
 * 
 * @author --
 * @date 06 de Dezembro de 2021
 */

#ifndef _ADC_CONFIG_
    #define _ADC_CONFIG_

    #include <Arduino.h>

    #include "freertos/FreeRTOS.h"
    #include "freertos/task.h"

    #include <driver/i2s.h>
    #include <driver/adc.h>
    
    #include "soc/syscon_reg.h"
    #include "soc/syscon_struct.h"

    #define SAMPLES_ADC            2040    			 // numero de amostras por canal
    #define NUM_ADC_CHAN           6       			 // numero de canais do ADC para varredura
    #define SAMPLING_FREQUENCY_ADC NUM_ADC_CHAN*6120 // frequencia de amsotragem de 6120Hz por canal
    #define I2S_PORT               0           		 // porta i2s de escolha [0,1]

    #define ADC_WIDTH ADC_WIDTH_BIT_12 // resolucao do adc
    #define ADC_ATTEN ADC_ATTEN_2_5db  // nivel de atenuacao

    #define ADC_VA ADC1_CHANNEL_0 // GPIO36
    #define ADC_VB ADC1_CHANNEL_3 // GPIO39
    #define ADC_VC ADC1_CHANNEL_6 // GPIO34
    #define ADC_IA ADC1_CHANNEL_5 // GPIO33
    #define ADC_IB ADC1_CHANNEL_7 // GPIO35
    #define ADC_IC ADC1_CHANNEL_4 // GPIO32

    #define getCh(x) (x & 0xF000) >> 12 // os 4 bits mais significativos e a indicacao do canal
    #define getSmpl(x) (x & 0x0FFF) // os 12 bits menos significativos da amostra significa o valor lidos 

    /**
     * @brief Configurações inicias para os canais do ADC.
     */
    void adc_configuration();

    /**
     * @brief Configurações inicias para o I2S, permitindo que este funcione no modo ADC.
     */
    void i2s_configuration();

    /**
     * @brief Configurações inicias dos registradores para permitir varredura contínua dos canais do ADC.
     */
    void reg_configuration();

    /**
     * @brief Função que reúne as configurações do ADC, I2S e dos registradores para permitir facilidade no uso.
     */
    void adc_global_config();

    /**
     * @brief Realiza leitura do buffer do ADC e trata e distribui os dados conforme as configurações para
     *        permitir facilidade no iso.
     * 
     * @param buffer buffer para os dados lidos (passado por referência para ser modificado)
     * @param samples_read número de amostras lidas (passada por referência para ser modificada)
     */
    void read_data(uint16_t (*buffer)[NUM_ADC_CHAN*SAMPLES_ADC], size_t* samples_read);
#endif // _ADC_CONFIG_