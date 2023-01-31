/**
 * @file main.cpp
 * @brief Função principal que contém as rotinas de setup() e loop() do framework Arduino
 *        e que dão vazão às demais rotinas.
 * 
 *        Compile firmware: pio run -e lolin32
 *        Compile spiffs: pio run -t buildfs -e lolin32
 *        Flash firmware: pio run -t upload
 *        Flash spiffs: pio run -t uploadfs 
 * 
 * @author --
 * @date 02 de Agosto de 2021
 */

#include "main.h"

hw_timer_t *timerWatch;
uint32_t last_time=0, actual_time=0;

void setup()
{
  Serial.begin(115200);

  constants = parse_sensor_id(); // setup sensor constants and id

#if DISPLAY_OLED_LOG
  // Configura o display OLED
  oled_global_config(display_oled);
  attachInterrupt(OLED_BUTTON, ISR_BTN, RISING); // hook isr handler for specific gpio pin
  xReturnedTask = xTaskCreatePinnedToCore(vTaskOLED, "tOLED", 4096+configMINIMAL_STACK_SIZE,   NULL, configMAX_PRIORITIES-3, &xTaskOLEDhandle, APP_CPU_NUM);

  if(xReturnedTask == pdFAIL){ // teste para criacao dos objetos do kernel
    Serial.println("Falha na inicialização dos objetos do kernel para o display OLED.");
    while(1);
  }
#endif
#if SD_CARD_STORAGE
  if(!SD.begin())
  {
    Serial.println("Card Mount Failed");
    return;
  }
  // header
  appendFile(SD, "/data.csv", "VA,VB,VC,IA,IB,IC,P,Q,S,FP,VTHDA,VTHDB,VTHDC,ITHDA,ITHDB,ITHDC,F,A_AB,A_BC,A_AC,EP,EQ,FDV,Timestamp\n");
#endif
#if ENABLE_WATCHDOG_TIMER
  // Configuração do WATCHDOG
  timerWatch = timerBegin(1, 80, true);                        // timerID 1, div 80
  timerAttachInterrupt(timerWatch, &resetModule, true);        // timer, callback, interrupção de borda
  timerAlarmWrite(timerWatch, TIMER_WATCHDOG * 1000000, true); // timer, tempo (us), repetição
  timerAlarmEnable(timerWatch);                                // habilita a interrupção
#endif
#if NVS_STORAGE
  // Carregar ultimo valor salvo da nvs
  NVS.begin();
  ENERGIA_ATIVA = NVS.getFloat("ENERGIA_ATIVA");
  ENERGIA_REATIVA = NVS.getFloat("ENERGIA_REATIVA");
  ENERGIA_APARENTE = NVS.getFloat("ENERGIA_APARENTE");
#endif
#if ENABLE_MQTT_SEND
  create_topics(constants.id_ota);
#endif
#if WIRELESS_CONECTIVITY
  // Conectar na rede wifi
  setup_wifi(constants.SSID, constants.PASSWD);
  ntpAtualizado = setup_ntp(ntpClient1, ntpClient2, rtc);
  reconnect_wifi(constants.SSID, constants.PASSWD);
  
#endif
#if COMPUTE_READINGS
  readings = initialize_readings(); // Inicializa a estrutura responsavel por armazenar as leituras
  adc_global_config(); // Configuracao do ADC 
  gpio_global_config(); // Configuracao do GPIO
  
  last_time = micros();
#endif
}

void loop()
{
  // contabilizar periodo do loop para passar como argumento para calcular acumulo de energia
  actual_time = micros();
  float loop_period = ((float)(actual_time)-(float)(last_time))/1000000;
  last_time = actual_time;

#if ENABLE_WATCHDOG_TIMER
  timerWrite(timerWatch, (uint64_t) 0); //RESET WATCHDOG
#endif
#if NVS_STORAGE && ONLINE
  // Zera varial de energia
  if ((ntpAtualizado == 1) && ((rtc.getMinute() == 0) || (rtc.getMinute() == 30)))
  {
    NVS.setFloat("ENERGIA_ATIVA", ENERGIA_ATIVA);
    NVS.setFloat("ENERGIA_REATIVA", ENERGIA_REATIVA);
    NVS.setFloat("ENERGIA_APARENTE", ENERGIA_APARENTE);
  }
#endif
#if ENABLE_OTA_UPDATE
  if (rtc.getMinute() == 0 || rtc.getMinute() == 30) ota_update_loop(client_ota, constants); // Atualiza OTA a cada 30min.
#endif
#if COMPUTE_READINGS
  
  read_data(&buffer_adc, &samples_read); // leitura do buffer das leituras do ADC
  read_gpio_data(&buffer_gpio, &buffer_time); // leitura do buffer das dos GPIO
  readings = compute_readings(readings, constants, loop_period, buffer_adc, buffer_gpio, buffer_time, samples_read, (size_t) NUM_ADC_CHAN); // executa o calculo dos parametros

  ENERGIA_ATIVA += readings.Ep;
  ENERGIA_REATIVA += readings.Eq;
  ENERGIA_APARENTE += readings.Es; 
      
#endif
#if PRINT_READINGS
  print_readings(readings);
#endif
#if PLOT_WAVEFORMS
  plot_waveforms(buffer_adc, samples_read, (size_t) NUM_ADC_CHAN, constants.tc);
#endif
#if WIRELESS_CONECTIVITY
  reconnect_wifi(constants.SSID, constants.PASSWD);
#endif
#if ENABLE_MQTT_SEND && COMPUTE_READINGS
  // Preparar mensagem para o envio MQTT
  mqtt_msg(message_mqtt, readings, rtc.getEpoch(), WiFi.RSSI(), SKETCH_VERSION, constants.spiffs_version);
#endif
#if PRINT_MQTT_LOG
  Serial.println("\n\n********************************************");
  Serial.print("Data: ");
  Serial.println(rtc.getTime("%d-%m-%Y %H:%M:%S"));
  Serial.print("WiFi RSSI: ");
  Serial.println(WiFi.RSSI());
  Serial.println(message_mqtt.msg);
  Serial.println("\n");
#endif
#if PRINT_OTA_LOG
  Serial.println("Versao do firmware: " ACTUAL_VERSION_SKETCH);
  Serial.print("Versao SPIFFS: ");
  Serial.println(constants.actualVersionSPIFFFS);
  Serial.print("Dispositivo: ");
  Serial.println(constants.id_ota);
  Serial.println("\n");
#endif
#if CLOUD_STORAGE
  #if ENABLE_UFRN_MQTT_SERVERS
  // mqtt_send(mqttServer1, mqttPort1, mqttUser1, mqttPassword1, TOPIC1, message_mqtt.msg); // ---IMD INFRAESTRUTURA v1 (interno)
  // mqtt_send(mqttServer2, mqttPort2, mqttUser2, mqttPassword2, TOPIC2, message_mqtt.msg); // ---IMD INFRAESTRUTURA v2 (interno)
  mqtt_send(mqttServer3, mqttPort3, mqttUser3, mqttPassword3, TOPIC3, message_mqtt.msg); // ---IMD INFRAESTRUTURA v3 (interno)
  // mqtt_send(mqttServer4, mqttPort4, mqttUser4, mqttPassword4, TOPIC4, message_mqtt.msg); // ---IMD INFRAESTRUTURA v3 (externo)
  #endif
  #if ENABLE_EXTERN_MQTT_SERVERS
  mqtt_send(mqttServer5, mqttPort5, mqttUser5, mqttPassword5, TOPIC5, message_mqtt.msg); // Regina (iterno)
  // mqtt_send(mqttServer6, mqttPort6, mqttUser6, mqttPassword6, TOPIC6, message_mqtt.msg); // Regina (externo)
  mqtt_send(mqttServer7, mqttPort7, mqttUser7, mqttPassword7, TOPIC7, message_mqtt.msg); // UFPA
  #endif
#endif
#if SD_CARD_STORAGE || SPIFFS_STORAGE
  // Preparar mensagem para a escrita no cartão SD
  mensagem = data_msg(readings,  rtc.getEpoch());
  #if SD_CARD_STORAGE
    appendFile(SD, "/data.csv", mensagem.msg);
  #endif
  #if SPIFFS_STORAGE
    File spiffs_file_handler = open_file("/data.csv");
    spiffs_file_handler = write_line(spiffs_file_handler, mensagem.msg);
  #endif
#endif
  vTaskDelay(pdMS_TO_TICKS(LOOP_PERIOD));
}

#if ENABLE_WATCHDOG_TIMER
void IRAM_ATTR resetModule()
{
  ets_printf("\n(watchdog) reiniciar\n\n"); //imprime no log
  //esp_restart_noos(); //reinicia o chip
  esp_restart();
}
#endif

#if ENABLE_MQTT_SEND
void create_topics(char* id)
{
  asprintf(&TOPIC1, "/c92efaa512319d57e818f31c087c632a/%s/attrs", constants.id_ota); //---IMD INFRAESTRUTURA v1 (interno)
  asprintf(&TOPIC2, "/c92efaa512319d57e818f31c087c632a/%s/attrs", constants.id_ota); //---IMD INFRAESTRUTURA v2 (interno)
  asprintf(&TOPIC3, "/c92efaa512319d57e818f31c087c632a/%s/attrs", constants.id_ota); //---IMD INFRAESTRUTURA v3 (interno)
  asprintf(&TOPIC4, "/c92efaa512319d57e818f31c087c632a/%s/attrs", constants.id_ota); //---IMD INFRAESTRUTURA v3 (externo)
  asprintf(&TOPIC5, "/4jggokgpepnvsb2uv4s40d59ov/%s/attrs", "SM003");//constants.id_ota); // ---Regina (interno)
  asprintf(&TOPIC6, "/4jggokgpepnvsb2uv4s40d59ov/%s/attrs", "SM003");//constants.id_ota); // ---Regina (externo)
  asprintf(&TOPIC7, "/4jggokgpepnvsb2uv4s40d59ov/%s/attrs", "SM003");//constants.id_ota); // ---UFPA
}
#endif

#if DISPLAY_OLED_LOG
void vTaskOLED(void * pvParameters)
{
  while(1)
  {
    display_mode = (oled_mode_t) (digitalRead(OLED_SWITCH));
    display_loop(display_oled, readings, display_screen, display_mode);
    if(display_mode==OLED_SEQUENTIAL_MODE) vTaskDelay(pdMS_TO_TICKS(2000));
    else vTaskDelay(pdMS_TO_TICKS(100));
  }

}

static void IRAM_ATTR ISR_BTN()
{
  portENTER_CRITICAL(&spinlock_oled_screen);
  if(display_mode==OLED_TOGGLE_MODE) display_screen = (oled_screen_t) (display_screen+1);
  portEXIT_CRITICAL(&spinlock_oled_screen);
}
#endif