/**
 * @file ethernet_config.h
 * @brief Arquivo cabeçalho de configuração para estabelecimento da conexão com fio
 *
 * @author --
 * @date 05 de Maio de 2021
 */

#ifndef _ETHERNET_CONFIG_
    #define _ETHERNET_CONFIG_ 

	/*
    * Connect the Ethernet module to the following pins:
    *
    * ENC28J60 | ESP32
    *   3V3            3V3
    *   CS              GPIO5
    *   MOSI         GPIO23
    *   CLK            GPIO18
    *   MISO         GPIO19
    *   GND          GND
    */


    #define NTP_SERVER_0 "0.br.pool.ntp.org"
    #define NTP_SERVER_1 "a.ntp.br"
    #define NTP_SERVER_2 "b.ntp.br"
    #define NTP_SERVER_3 "c.ntp.br"
    #define NTP_UFRN_1 "10.7.10.1"
    #define NTP_UFRN_2 "10.7.10.2"
    #define NTP_UFRN_3 "ntp.ufrn.br"

    #define PUBLISH_DELAY   5000

    #include "UIPEthernet/UIPEthernet.h"
    #include "PubSubClient/PubSubClient.h"

    #include "rtc_esp32/ESP32Time.h"
    #include "ntp_client_generic/NTPClient_Generic.h" //Biblioteca diferente da versao com WIFI

    extern EthernetClient ethClient;
    extern PubSubClient pubsub_client;

    extern EthernetUDP udp_ethernet;
    extern ESP32Time rtc;

    extern int timeZone;
    extern int ntpAtualizado;

    extern uint8_t mac[6]; // Mudar MAC p/cada medidor
    extern IPAddress IP; //IP Static sem DCHP

    extern IPAddress mqttServer1;
    extern const int mqttPort1;
    extern const char *mqttUser1;
    extern const char *mqttPassword1;
    #define TOPIC1 "/4jggokgpepnvsb2uv4s40d59ov/SM003/attrs"


    extern IPAddress mqttServer2;
    extern const int mqttPort2;
    extern const char *mqttUser2;
    extern const char *mqttPassword2;
    #define TOPIC2 "/4jggokgpepnvsb2uv4s40d59ov/SM003/attrs"

    extern IPAddress mqttServer3;
    extern const int mqttPort3;
    extern const char *mqttUser3;
    extern const char *mqttPassword3;
    #define TOPIC3 "/c92efaa512319d57e818f31c087c632a/" ID_OTA "/attrs" // ---IMD INFRAESTRUTURA v3

#endif // _ETHERNET_CONFIG_