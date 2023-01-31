/**
 * @file mqtt_config.h
 * @brief Arquivo cabeçalho de configuração para o envio dos dados obtidos por protocolo MQTT
 *
 * @author --
 * @date 02 de Agosto de 2021
 */

#ifndef _MQTT_CONFIG_
    #define _MQTT_CONFIG_

    #define TENTATIVAS_ENVIO 4

    #include "MQTTClient.h" // third party library in lib/mqtt/
    #include <WiFi.h>

    #include "SmartReadings.h"
    #include "definitions.h"

    extern WiFiClient wifi_client;

    struct data
    {
        char msg[800];
        int pos;
    };

    /**
     * @brief Acrescenta na mensagem atual um texto qualquer.
     * 
     * @param mensagem Estrutura final da mensagem MQTT a ser enviada.
     * @param texto Array de char a ser acrescentado na mensagem.
     *
     */
    void bufferizar(data &mensagem, char texto[800]);

    /**
     * @brief Empacota mensagem MQTT para posteriormente ser publicada no tópico configurado.
     *
     * @param mensagem Variável de mensagem a ser escrita por referência.
     * @param readings Leituras a serem enviadas por MQTT.
     * @param timestamp Timestamp em formato UNIX obtido do módulo RTC.
     * @param RSSI RSSI obtido do módulo Wi-Fi.
     * @param sketch_version Versão atual do Sketch.
     * @param spiffs_version Versão atual do SPIFFS.
     */
    void mqtt_msg(data &mensagem, SmartReadings readings, long timestamp, int8_t RSSI, float sketch_version, float spiffs_version);

    /**
     * @brief Envio de mensagem MQTT para servidor previamente configurado.
     * 
     * @param wifi_client Cliente Wi-Fi passado por referência.
     * @param servidor Servido MQTT cuja mensagem será enviada.
     * @param porta Array de char a ser acrescentado na mensagem.
     * @param user Usuário.
     * @param password Senha para o usuário.
     * @param topico Tópico em que a mensagem será publicada.
     * @param msg Mensagem MQTT previamente configurada e populada que será enviada.
     *
     */
    void mqtt_send(const char servidor[], int porta, const char user[], const char password[], const char topico[], const char msg[]);

    
#endif // _MQTT_CONFIG_