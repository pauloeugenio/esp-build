/**
 * @file main.h
 * @brief Arquivo de inicialização à função principal, contendo definição de macros,
 *        inicialização de variáveis, inclusão de bilbiotecas, protótipo de funções
 * 
 * @author --
 * @date 02 de Agosto de 2021
 */

#ifndef _MAIN_
    #define _MAIN_

    // --------
    // includes
    // --------

    #include <Arduino.h>
    #include <iostream>
    #include <string>
    #include <cmath>  /* isnan */
    #include <math.h> /* isinf, sqrt, pow */

    #include "ArduinoNvs.h" // third party library in lib/arduino_nvs/ 
    
    #include "wifi_config.h"
    #include "mqtt_config.h"
    #include "ota_config.h"
    #include "gpio_config.h"
    #include "adc_config.h"
    #include "spiffs_config.h"
    #include "sd_config.h"
    #include "oled_config.h"
    #include "SmartReadings.h"
    #include "definitions.h"
    
    #ifndef FREERTOS_LIB_INCLUDED
        #define FREERTOS_LIB_INCLUDED
        #include "freertos/FreeRTOS.h"
        #include "freertos/task.h"
    #endif //FREERTOS_LIB_INCLUDED

    #define TIMER_WATCHDOG 220 // tempo em segundos

    // -----------------
    // variáveis globais
    // -----------------

    SmartReadings readings;                        // variaveis calculadas a partr das leituras
    SensorConstants constants;                     // constantes inerentes ao sensor obtidas via SPIFFS
    size_t bytes_read;                             // numero de bytes lidos
    size_t samples_read;                           // numero de amostras lida
    uint16_t buffer_adc[NUM_ADC_CHAN*SAMPLES_ADC]; // buffer de leitura do i2s
    uint32_t buffer_time[SAMPLES_GPIO];            // vetor de tempo instantaneo
    uint8_t buffer_gpio[SAMPLES_GPIO];             // vetor de forma de onda retangular dos comparadores

    float ENERGIA_ATIVA=0;
    float ENERGIA_REATIVA=0;
    float ENERGIA_APARENTE=0;   

    //-------------------------------------//
    //             Watchdog                //
    //-------------------------------------//
    
    // hw_timer_t *timerWatch;

    //-------------------------------------//
    //                Wi-Fi                //
    //-------------------------------------//

    // Wi-Fi
    WiFiUDP udp_wifi;

    //-------------------------------------//
    //                 NTP & RTC           //
    //-------------------------------------//

    // RTC e NTP
    ESP32Time rtc;

    int timeZone = -3;
    int ntpAtualizado = 0;

    NTPClient ntpClient1(
        udp_wifi,        // Socket udp
        NTP_SERVER_1,
        //NTP_UFRN_3,      // URL do server NTP
        timeZone * 3600, // Deslocamento do horário em relacão ao GMT 0
        60000            // Intervalo entre verificações online
    );

    NTPClient ntpClient2(
        udp_wifi,        // Socket udp
        NTP_SERVER_1,    // URL do server NTP
        timeZone * 3600, // Deslocamento do horário em relacão ao GMT 0
        60000            // Intervalo entre verificações online
    );

    //-------------------------------------//
    //                 MQTT                //
    //-------------------------------------//

    data message_mqtt;

    const char *mqttServer1 = "10.7.52.80"; // ---IMD INFRAESTRUTURA v1 (interno)
    const int mqttPort1 = 1883;
    const char *mqttUser1 = "";
    const char *mqttPassword1 = "";
    char* TOPIC1;

    const char *mqttServer2 = "10.7.53.213"; // ---IMD INFRAESTRUTURA v2 (interno)
    const int mqttPort2 = 1883;
    const char *mqttUser2 = "";
    const char *mqttPassword2 = "";
    char* TOPIC2;

    const char *mqttServer3 = "10.7.53.29"; // ---IMD INFRAESTRUTURA v3 (interno)
    const int mqttPort3 = 1883;
    const char *mqttUser3 = "";
    const char *mqttPassword3 = "";
    char* TOPIC3;

    const char *mqttServer4 = "177.20.150.21"; // ---IMD INFRAESTRUTURA v3 (externo)
    const int mqttPort4 = 1883;
    const char *mqttUser4 = "";
    const char *mqttPassword4 = "";
    char* TOPIC4;

    const char *mqttServer5 = "10.7.229.35"; // ---Regina (interno)
    const int mqttPort5 = 1883;
    const char *mqttUser5 = "";
    const char *mqttPassword5 = "";
    char* TOPIC5;

    const char *mqttServer6 = "177.20.148.103"; // ---Regina (externo)
    const int mqttPort6 = 1883;
    const char *mqttUser6 = "";
    const char *mqttPassword6 = "";
    char* TOPIC6;

    const char *mqttServer7 = "200.239.72.248"; // ---UFPA
    const int mqttPort7 = 1883;
    const char *mqttUser7 = "";
    const char *mqttPassword7 = "";
    char* TOPIC7;


    //-------------------------------------//
    //                OTA                  //
    //-------------------------------------//

    WiFiClientSecure client_ota; // instancia de cliente seguro

    //-------------------------------------//
    //                OLED                 //
    //-------------------------------------//

    SSD1306Wire display_oled(0x3C, 21, 22);
    oled_screen_t display_screen=OLED_MAIN_SCREEN;
    oled_mode_t display_mode=OLED_SEQUENTIAL_MODE;

    //-------------------------------------//
    //             freeRTOS                //
    //-------------------------------------//

    TaskHandle_t xTaskOLEDhandle; 

    portMUX_TYPE spinlock = portMUX_INITIALIZER_UNLOCKED;
    portMUX_TYPE spinlock_oled_mode = portMUX_INITIALIZER_UNLOCKED;
    portMUX_TYPE spinlock_oled_screen = portMUX_INITIALIZER_UNLOCKED;

    /**
     * @brief Callback do timer do watchdog para resetar o ESP32 
     *
     */
    void IRAM_ATTR resetModule();

    /**
     * @brief Realiza a atualização do display OLED.
     */
    void vTaskOLED(void * pvParameters);

    /**
     * @brief Inicializa as strings referentes a cada tópico MQTT.
     * 
     * @param id id OTA do dispositivo.
     */
    void create_topics(char* id);

    /**
     * @brief Interrupt service routine (ISR) para botão que altera a tela a ser mostrada no display OLED.
     * 
     * @param id id OTA do dispositivo.
     */
    static void IRAM_ATTR ISR_BTN();
    

#endif // _MAIN_
