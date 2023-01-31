/**
 * @file definitions.h
 * @brief Arquivo contendo a inicialização das macros responsáveis por filtrar os modos de uso do medidor.
 *
 * @author --
 * @date 28 de Abril de 2022
 */

// Recursos computacionais
#define LOOP_PERIOD           1000
#define TIMER_PERIOD          LOOP_PERIOD/2
#define COMPUTE_READINGS      (1)
#define ENABLE_WATCHDOG_TIMER (1)

// Conectividade
#define ONLINE  (1)
#define OFFLINE !(ONLINE)

// Tipos de conectividade
#define WIRELESS_CONECTIVITY (1) && ONLINE
#define ETHERNET_CONECTIVITY !(WIRELESS_CONECTIVITY) && ONLINE // status: não implementado
#define ENABLE_OTA_UPDATE    (1) && ONLINE
#define ENABLE_MQTT_SEND     (1) && ONLINE

// Servidores MQTT
#define ENABLE_UFRN_MQTT_SERVERS   (1) && ENABLE_MQTT_SEND
#define ENABLE_EXTERN_MQTT_SERVERS (1) && ENABLE_MQTT_SEND

// Formato da mensagem MQTT
#define OLD_MQTT_MESSAGE_FORMAT (1) && ENABLE_MQTT_SEND
#define NEW_MQTT_MESSAGE_FORMAT !(OLD_MQTT_MESSAGE_FORMAT) && ENABLE_MQTT_SEND


// Armazenamento
#define CLOUD_STORAGE   (1) && ENABLE_MQTT_SEND
#define SPIFFS_STORAGE  (0) && COMPUTE_READINGS
#define SD_CARD_STORAGE (0) && COMPUTE_READINGS
#define NVS_STORAGE     (1)


// Interatividade
#define PRINT_MQTT_LOG   (1) && ENABLE_MQTT_SEND && COMPUTE_READINGS
#define PRINT_OTA_LOG    (1) && ENABLE_OTA_UPDATE
#define PRINT_READINGS   (0) && COMPUTE_READINGS
#define PLOT_WAVEFORMS   (0) && COMPUTE_READINGS
#define DISPLAY_OLED_LOG (0)