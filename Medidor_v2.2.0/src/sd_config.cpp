/**
 * @file sd_config.cpp
 * @brief Arquivo de definição das funções de uso do cartão SD.
 * 
 * @author --
 * @date 28 de Abril de 2021
 */

#include "sd_config.h"

data data_msg(SmartReadings readings, long timestamp)
{
  char buffer[800];

  data mensagem;
  mensagem.pos = 0;

  // Tensao

  sprintf(buffer, "%.2lf,", (float)readings.VA);
  bufferizar(mensagem, buffer);
  sprintf(buffer, "%.2lf,", (float)readings.VB);
  bufferizar(mensagem, buffer);
  sprintf(buffer, "%.2lf,", (float)readings.VC);
  bufferizar(mensagem, buffer);

  // Corrente
  sprintf(buffer, "%.2lf,", (float)readings.IA);
  bufferizar(mensagem, buffer);
  sprintf(buffer, "%.2lf,", (float)readings.IB);
  bufferizar(mensagem, buffer);
  sprintf(buffer, "%.2lf,", (float)readings.IC);
  bufferizar(mensagem, buffer);

  // Potencias
  sprintf(buffer, "%.2lf,", (float)readings.P);
  bufferizar(mensagem, buffer);
  sprintf(buffer, "%.2lf,", (float)readings.Q);
  bufferizar(mensagem, buffer);
  sprintf(buffer, "%.2lf,", (float)readings.S);
  bufferizar(mensagem, buffer);
  sprintf(buffer, "%.2lf,", (float)readings.FP);
  bufferizar(mensagem, buffer);

  // DHTs
  
  sprintf(buffer, "%.2lf,", (float)readings.DHT[0]);
  bufferizar(mensagem, buffer);
  sprintf(buffer, "%.2lf,", (float)readings.DHT[1]);
  bufferizar(mensagem, buffer);
  sprintf(buffer, "%.2lf,", (float)readings.DHT[2]);
  bufferizar(mensagem, buffer);
  
  
  sprintf(buffer, "%.2lf,", (float)readings.DHT[3]);
  bufferizar(mensagem, buffer);
  sprintf(buffer, "%.2lf,", (float)readings.DHT[4]);
  bufferizar(mensagem, buffer);
  sprintf(buffer, "%.2lf,", (float)readings.DHT[5]);
  bufferizar(mensagem, buffer);

  // Grandezas de fase

  sprintf(buffer, "%.2lf,", (float)readings.F); 
  bufferizar(mensagem, buffer);
  sprintf(buffer, "%.2lf,", (float)readings.A_RS); 
  bufferizar(mensagem, buffer);
  sprintf(buffer, "%.2lf,", (float)readings.A_ST);
  bufferizar(mensagem, buffer);
  sprintf(buffer, "%.2lf,", (float)readings.A_TR);
  bufferizar(mensagem, buffer);


  // Energias

  sprintf(buffer, "%.3lf,", (float)readings.Ep);
  bufferizar(mensagem, buffer);
  sprintf(buffer, "%.3lf,", (float)readings.Eq);
  bufferizar(mensagem, buffer);
  sprintf(buffer, "%.3lf,", (float)readings.FDV); // fator de desequilibrio
  bufferizar(mensagem, buffer);



  // Miscelania

  sprintf(buffer, "%ld\n", timestamp); //Timestamp
  bufferizar(mensagem, buffer);  
  // sprintf(buffer, "|reservado2|%.2lf", (float)ntpAtualizado); //NTP ATUALIZADO
  // bufferizar(mensagem, buffer);
  // sprintf(buffer, "|reservado3|%.2lf", OTA_VERSION);//(float)WiFi.RSSI()); //WiFi RSSI
  // bufferizar(mensagem, buffer);

  return mensagem;
}

void appendFile(fs::FS &fs, const char * path, const char * message)
{
  Serial.printf("Appending to file: %s\n", path);

  File file = fs.open(path, FILE_APPEND);
  if(!file){
    Serial.println("Failed to open file for appending");
    return;
  }
  if(file.print(message)){
    Serial.println("Message appended");
  } else {
    Serial.println("Append failed");
  }
  file.close();
}

