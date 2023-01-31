/**
 * @file mqtt_config.cpp
 * @brief Arquivo de configuração para o envio dos dados obtidos por protocolo MQTT
 *
 * @author --
 * @date 02 de Agosto de 2021
 */

#include "mqtt_config.h"

WiFiClient wifi_client;

void bufferizar(data &mensagem, char texto[800])
{
  for (int i = mensagem.pos; i < (800 + mensagem.pos); i++)
  {
    mensagem.msg[i] = texto[i - mensagem.pos];
  }

  mensagem.pos = strlen(mensagem.msg);
}

#if OLD_MQTT_MESSAGE_FORMAT
void mqtt_msg(data &mensagem, SmartReadings readings, long timestamp, int8_t RSSI, float sketch_version, float spiffs_version)
{
  char buffer[800];

  mensagem.pos = 0;

  // Tensao

  sprintf(buffer, "voltA|%.2lf", (float)readings.VA);
  bufferizar(mensagem, buffer);
  sprintf(buffer, "|voltB|%.2lf", (float)readings.VB);
  bufferizar(mensagem, buffer);
  sprintf(buffer, "|voltC|%.2lf", (float)readings.VC);
  bufferizar(mensagem, buffer);

  // Corrente
  sprintf(buffer, "|correnteA|%.2lf", (float)readings.IA);
  bufferizar(mensagem, buffer);
  sprintf(buffer, "|correnteB|%.2lf", (float)readings.IB);
  bufferizar(mensagem, buffer);
  sprintf(buffer, "|correnteC|%.2lf", (float)readings.IC);
  bufferizar(mensagem, buffer);

  // Potencias
  sprintf(buffer, "|potenciaAtiva|%.2lf", (float)readings.P);
  bufferizar(mensagem, buffer);
  sprintf(buffer, "|potenciaReativa|%.2lf", (float)readings.Q);
  bufferizar(mensagem, buffer);
  sprintf(buffer, "|potenciaAparente|%.2lf", (float)readings.S);
  bufferizar(mensagem, buffer);
  sprintf(buffer, "|fatorDePotencia|%.2lf", (float)readings.FP);
  bufferizar(mensagem, buffer);

  // DHTs
  
  sprintf(buffer, "|voltTHDA|%.2lf", (float)readings.DHT[0]);
  bufferizar(mensagem, buffer);
  sprintf(buffer, "|voltTHDB|%.2lf", (float)readings.DHT[1]);
  bufferizar(mensagem, buffer);
  sprintf(buffer, "|voltTHDC|%.2lf", (float)readings.DHT[2]);
  bufferizar(mensagem, buffer);
  
  
  sprintf(buffer, "|correnteTHDA|%.2lf", (float)readings.DHT[3]);
  bufferizar(mensagem, buffer);
  sprintf(buffer, "|correnteTHDB|%.2lf", (float)readings.DHT[4]);
  bufferizar(mensagem, buffer);
  sprintf(buffer, "|correnteTHDC|%.2lf", (float)readings.DHT[5]);
  bufferizar(mensagem, buffer);

  // Grandezas de fase

  sprintf(buffer, "|frequencia|%.2lf", (float)readings.F); 
  bufferizar(mensagem, buffer);
  sprintf(buffer, "|anguloVoltAB|%.2lf", (float)readings.A_RS); 
  bufferizar(mensagem, buffer);
  sprintf(buffer, "|anguloVoltBC|%.2lf", (float)readings.A_ST);
  bufferizar(mensagem, buffer);
  sprintf(buffer, "|anguloVoltAC|%.2lf", (float)readings.A_TR);
  bufferizar(mensagem, buffer);


  // Energias

  sprintf(buffer, "|energiaAtiva|%.3lf", (float)readings.Ep);
  bufferizar(mensagem, buffer);
  sprintf(buffer, "|energiaReativa|%.3lf", (float)readings.Eq);
  bufferizar(mensagem, buffer);
  sprintf(buffer, "|energiaAparente|%.3lf", (float)readings.FDV); // fator de desequilibrio
  bufferizar(mensagem, buffer);

  // Miscelania
  sprintf(buffer, "|reservado1|%.2lf", (float)timestamp); //Timestamp
  bufferizar(mensagem, buffer);  
  sprintf(buffer, "|reservado2|%.2lf", (float)sketch_version); //NTP ATUALIZADO
  bufferizar(mensagem, buffer);
  sprintf(buffer, "|reservado3|%.2lf", (float)spiffs_version);//(float)WiFi.RSSI()); //WiFi RSSI
  bufferizar(mensagem, buffer);

}
#endif
#if NEW_MQTT_MESSAGE_FORMAT
// Novo formato para a mensagem MQTT
data mqtt_msg(SmartReadings readings, long timestamp)
{
  char buffer[800];

  data mensagem;
  mensagem.pos = 0;

  // Tensao

  sprintf(buffer, "VA|%.2lf", (float)readings.VA);
  bufferizar(&mensagem, buffer);
  sprintf(buffer, "|VB|%.2lf", (float)readings.VB);
  bufferizar(&mensagem, buffer);
  sprintf(buffer, "|VC|%.2lf", (float)readings.VC);
  bufferizar(&mensagem, buffer);

  // Corrente
  sprintf(buffer, "|IA|%.2lf", (float)readings.IA);
  bufferizar(&mensagem, buffer);
  sprintf(buffer, "|IB|%.2lf", (float)readings.IB);
  bufferizar(&mensagem, buffer);
  sprintf(buffer, "|IC|%.2lf", (float)readings.IC);
  bufferizar(&mensagem, buffer);

  // Potencias
  sprintf(buffer, "|P|%.2lf", (float)readings.P);
  bufferizar(&mensagem, buffer);
  sprintf(buffer, "|Q|%.2lf", (float)readings.Q);
  bufferizar(&mensagem, buffer);
  sprintf(buffer, "|S|%.2lf", (float)readings.S);
  bufferizar(&mensagem, buffer);
  sprintf(buffer, "|FP|%.2lf", (float)readings.FP);
  bufferizar(&mensagem, buffer);

  // DHTs
  
  sprintf(buffer, "|VTHDA|%.2lf", (float)readings.DHT[0]);
  bufferizar(&mensagem, buffer);
  sprintf(buffer, "|VTHDB|%.2lf", (float)readings.DHT[1]);
  bufferizar(&mensagem, buffer);
  sprintf(buffer, "|VTHDC|%.2lf", (float)readings.DHT[2]);
  bufferizar(&mensagem, buffer);
  
  
  sprintf(buffer, "|ITHDA|%.2lf", (float)readings.DHT[3]);
  bufferizar(&mensagem, buffer);
  sprintf(buffer, "|ITHDB|%.2lf", (float)readings.DHT[4]);
  bufferizar(&mensagem, buffer);
  sprintf(buffer, "|ITHDC|%.2lf", (float)readings.DHT[5]);
  bufferizar(&mensagem, buffer);

  // Grandezas de fase

  sprintf(buffer, "|HZ|%.2lf", (float)readings.F); 
  bufferizar(&mensagem, buffer);
  sprintf(buffer, "|angVAB|%.2lf", (float)readings.A_RS); 
  bufferizar(&mensagem, buffer);
  sprintf(buffer, "|angVBC|%.2lf", (float)readings.A_ST);
  bufferizar(&mensagem, buffer);
  sprintf(buffer, "|angVAC|%.2lf", (float)readings.A_TR);
  bufferizar(&mensagem, buffer);


  // Energias
  sprintf(buffer, "|PAC|%.3lf", (float)ENERGIA_ATIVA);
  bufferizar(&mensagem, buffer);
  sprintf(buffer, "|QAC|%.3lf", (float)ENERGIA_REATIVA);
  bufferizar(&mensagem, buffer);
  sprintf(buffer, "|PINS|%.3lf", (float)readings.Ep);
  bufferizar(&mensagem, buffer);
  sprintf(buffer, "|QINS|%.3lf", (float)readings.Eq);
  bufferizar(&mensagem, buffer);
  sprintf(buffer, "|SINS|%.3lf", (float)readings.Eq);
  bufferizar(&mensagem, buffer);
  
  
  // Qualidade de energia

  sprintf(buffer, "|VTCD|%.3lf", (float)0); // 
  bufferizar(&mensagem, buffer);
  sprintf(buffer, "|TA|%.3lf", (float)0); // 
  bufferizar(&mensagem, buffer);
  sprintf(buffer, "|DESEQ|%.3lf", (float)readings.FDV); // fator de desequilibrio
  bufferizar(&mensagem, buffer);

  // Miscelania

  sprintf(buffer, "|RSSI|%.2lf", (float)WiFi.RSSI()); // WiFi RSSI
  bufferizar(&mensagem, buffer); 
  sprintf(buffer, "|NTP|%.2lf", (float)ntpAtualizado); //NTP ATUALIZADO
  bufferizar(&mensagem, buffer);
  sprintf(buffer, "|TIMESTAMP|%.2lf", (float)timestamp); //Timestamp
  bufferizar(&mensagem, buffer); 
  sprintf(buffer, "|V_SKETCH|%.2lf", (float)SKETCH_VERSION); // versao sketch
  bufferizar(&mensagem, buffer);
  sprintf(buffer, "|V_SPIFFS|%.2lf", (float)spiffs_version); // versao SPIFFS
  bufferizar(&mensagem, buffer);
  sprintf(buffer, "|TAG|%.2lf", (float)0); 
  bufferizar(&mensagem, buffer);
  sprintf(buffer, "|RES1|%.2lf", (float)0); // 
  bufferizar(&mensagem, buffer);
  sprintf(buffer, "|RES2|%.2lf", (float)0); // 
  bufferizar(&mensagem, buffer);
  sprintf(buffer, "|RES3|%.2lf", (float)0); // 
  bufferizar(&mensagem, buffer);
  sprintf(buffer, "|RES4|%.2lf", (float)0); // 
  bufferizar(&mensagem, buffer);
  sprintf(buffer, "|RES5|%.2lf", (float)0); // 
  bufferizar(&mensagem, buffer);

  return mensagem;
}
#endif

void mqtt_send(const char servidor[], int porta, const char user[], const char password[], const char topico[], const char msg[])
{
  MQTTClient mqtt_client;
  int tentativa = 0;

  mqtt_client.begin(servidor, porta, wifi_client); //Teste

  Serial.print("\nConectando ao Servidor MQTT ");
  Serial.print(servidor);
  Serial.print("...");
  while (!mqtt_client.connect("esp32", user, password, 0) && tentativa < TENTATIVAS_ENVIO)
  {
    Serial.print(".");
    tentativa++;
    vTaskDelay(pdMS_TO_TICKS(1000));
  }

  if (tentativa < TENTATIVAS_ENVIO) //Tenta enviar 4 vezes
  {
    mqtt_client.publish(topico, msg);
    Serial.println("Mensagem enviada!");
    //Serial.println(msg);

    mqtt_client.disconnect();
  }
  else
  {
    Serial.print("\nFalha ao conectar o servidor ");
    Serial.println(servidor);
  }
}


