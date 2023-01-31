/**
 * @file sd_config.h
 * @brief Arquivo de inicialização das funções de uso do cartão SD.
 * 
 * @author --
 * @date 28 de Abril de 2021
 */

#ifndef _SD_CONFIG_
    #define _SD_CONFIG_

    #include "FS.h"
    #include "SD.h"
    #include "SPI.h"
    #include "mqtt_config.h"

    /*
    * Connect the SD card to the following pins:
    *
    * SD Card | ESP32
    *   3V3     3V3
    *   CS      GPIO5
    *   MOSI    GPIO23
    *   CLK     GPIO18
    *   MISO    GPIO19
    *   GND     GND
    */

    /**
     * @brief Gera mensagem a partir das leituras.
     * 
     * @param readings leituras das variavéis.
     * @return 
     *  - leituras convertidas em formato de mensagem.
     */
    data data_msg(SmartReadings readings);

    /**
     * @brief Concatenar string a um arquivo aberto no cartão SD.
     * 
     * @param fs instância do filesystem do cartão SD.
     * @param path caminho para o arquivo de escrita.
     * @param message mensagem a ser escrita no arquivo.
     */
    
    void appendFile(fs::FS &fs, const char * path, const char * message);

#endif // _SD_CONFIG_