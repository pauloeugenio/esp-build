/**
 * @file ota_config.cpp
 * @brief Arquivo de configuração para atualizações over-the-air (OTA)
 *
 * @author --
 * @date 20 Setembro de 2021
 */

#ifndef _OTA_CONFIG_
    #define _OTA_CONFIG_

    //Biblioteca OTA
    #include <HTTPUpdate.h>
    #include <SPIFFS.h>
    #include <string.h>
    #include <stdlib.h>
    #include "SmartReadings.h"

    #define CONFIG_FILE "/config.txt"

    #define ACTUAL_VERSION_SKETCH "2.2.1"
    #define NEXT_VERSION_SKETCH "2.2.2"
    #define SKETCH_VERSION 22.01

    /**
     * @brief Atualiza filesystem SPIFFS contendo os valores de constantes essenciais
     * 
     * @param client_ota Instância segura de client Wi-Fi para atualização OTA.
     * @param constants Estrutura contendo as constantes inerentes ao sensor.
     */
    void update_spiffs(WiFiClientSecure &client_ota, SensorConstants constants);

     /**
     * @brief Atualiza Sketch para nova versão do firmware
     * 
     * @param client_ota Instância segura de client Wi-Fi para atualização OTA.
     * @param constants Estrutura contendo as constantes inerentes ao sensor.
     */
    void update_sketch(WiFiClientSecure &client_ota, SensorConstants constants);

     /**
     * @brief Realiza a leitura das constantes contidas no arquivo no filesystem SPIFFS.
     * 
     * @param constants Estrutura contendo as constantes inerentes ao sensor, passada por referência.
     */
    void read_constants(SensorConstants &constants);

    /**
     * @brief Realiza a leitura do arquivo de configuração padrão localizado estaticamente em /config.txt.
     * 
     * @param constants Estrutura contendo as constantes inerentes ao sensor, passada por referência.
     */
    void read_config(SensorConstants &constants);

    /**
     * @brief Invoca as rotinas de leitura dos arquivos no SPIFFS para estabelecer as variáveis internas necessárias para o funcionamento.
     * 
     * @return 
     *  - Estrutura contendo todas as constantes específicas do sensor obtidas dos arquivos no SPIFFS.
     */
    SensorConstants parse_sensor_id();

    /**
     * @brief Realiza a verificação e, se possível, atualização dos binários do firmware.
     * 
     * @param client_ota Instância segura de client Wi-Fi para atualização OTA.
     * @param constants Estrutura contendo as constantes inerentes ao sensor.
     */
    void ota_update_loop(WiFiClientSecure &client_ota, SensorConstants constants);
#endif // _OTA_CONFIG_