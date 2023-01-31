/**
 * @file wifi_config.h
 * @brief Arquivo cabeçalho de configuração para estabelecimento da conexão sem fio
 *
 * @author --
 * @date 02 de Agosto de 2021
 */

#ifndef _WIFI_CONFIG_
    #define _WIFI_CONFIG_ 
    /*
    //#define SSID "UFRN"
    //#define PASSWD "smartcampus2020"
    //#define SSID "LASEM"
    //#define PASSWD "besourosuco"
    //#define SSID "LAR"
    //#define PASSWD "LAR@1480"
    //#define SSID "PROTOLAB"
    //#define PASSWD "protolab_ufrn"
    //#define SSID "ESP"
    //define PASSWD "12345678"
    //#define SSID ""
    //#define PASSWD ""
    //#define SSID "Cabo82"
    //#define PASSWD "mat0154059"
    //#define SSID "Smartenergy"
    //#define PASSWD "password123"
    //#define SSID "Smartenergy"
    //#define PASSWD "SmartEnergy"
    */

    #define NTP_SERVER_0 "0.br.pool.ntp.org"
    #define NTP_SERVER_1 "a.ntp.br"
    #define NTP_SERVER_2 "b.ntp.br"
    #define NTP_SERVER_3 "c.ntp.br"
    #define NTP_UFRN_1 "10.7.10.1"
    #define NTP_UFRN_2 "10.7.10.2"
    #define NTP_UFRN_3 "ntp.ufrn.br"
    
    #include "freertos/FreeRTOS.h"
    #include "freertos/task.h"

    #include <WiFi.h>
    #include <WiFiUdp.h>

    // Biblioteca RTC timer
    #include "ESP32Time.h" // third party library in lib/rtc_esp32/
    #include "NTPClient.h" // third party library in lib/ntp_client/

    /**
     * @brief Re-estabelece conexão Wi-Fi com access point.
     *
     * @param SSID "service set identifier" da rede wi-fi a se reconectar.
     * @param PASSWD "password" da rede wi-fi a se reconectar.
     */
    void reconnect_wifi(char *SSID, char *PASSWD);

    /**
     * @brief Configura os parâmetros da conexão Wi-Fi.
     * 
     * @param SSID "service set identifier" da rede wi-fi a se reconectar.
     * @param PASSWD "password" da rede wi-fi a se reconectar.
     */
    void setup_wifi(char *SSID, char *PASSWD);

    /**
     * @brief Realiza a atualização do RTC do ESP32 por meio do protocolo NTP.
     *
     * @param ntpClient1 Primeiro cliente NTP para tentar conexão.
     * @param ntpClient2 Segundo cliente NTP para tentar conexão.
     * @param rtc Instância do RTC para atualizar horário.
     * 
     * @return
     *  - Código de erro para atualização do NTP.
     */
    int setup_ntp(NTPClient ntpClient1, NTPClient ntpClient2, ESP32Time &rtc);
#endif // _WIFI_CONFIG_