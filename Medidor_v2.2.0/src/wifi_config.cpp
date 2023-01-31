/**
 * @file wifi_config.cpp
 * @brief Arquivo de configuração para estabelecimento da conexão sem fio
 *
 * @author --
 * @date 02 de Agosto de 2021
 */

#include "wifi_config.h"

void reconnect_wifi(char *SSID, char *PASSWD)
{
  Serial.print("Verificando WIFI...");
  while (WiFi.status() != WL_CONNECTED)
  {
    //WiFi.begin(SSID); // Se não tiver senha
    WiFi.begin(SSID, PASSWD);
    Serial.print(".");
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
  Serial.println(" Ok!");
}

void setup_wifi(char *SSID, char *PASSWD)
{
  vTaskDelay(pdMS_TO_TICKS(100));
  Serial.print("Conectando a rede ");
  Serial.print(SSID);
  Serial.print(" - ");
  Serial.println(WiFi.macAddress());

  //WiFi.begin(SSID); // Se não tiver senha
  WiFi.begin(SSID, PASSWD);

  Serial.print("Conectando");
  while (WiFi.status() != WL_CONNECTED)
  {
    vTaskDelay(pdMS_TO_TICKS(100));
    Serial.print(".");
  }

  Serial.println("\nConectado a rede WiFi!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

int setup_ntp(NTPClient ntpClient1, NTPClient ntpClient2, ESP32Time &rtc)
{
  int tentativa = 0;
  int ntpAtualizado = 0;
    
  //Inicializa o client NTP
  ntpClient1.begin();

  //Espera pelo primeiro update online
  Serial.println("\n\n:: Atualizar horario pelo NTP ::");
  while (!ntpClient1.update() && tentativa < 4)
  {
    Serial.print(".");
    ntpClient1.forceUpdate();
    tentativa++;
    vTaskDelay(pdMS_TO_TICKS(500));
  }

  if (tentativa < 4)
  {
    ntpAtualizado = 1;
    Serial.println();
    Serial.println(":: NTP atualizado 1 ::");

    //Atualizar RTC
    rtc.setTime(ntpClient1.getEpochTime());

    Serial.print("Hora NTP: ");
    Serial.println(ntpClient1.getEpochTime());

    Serial.print("Hora RTC: ");
    Serial.println(rtc.getEpoch());
  }
  else // Tentar atualiza o NTP pelo segundo servidor
  {
    ntpAtualizado = 0;
    Serial.println();
    Serial.println(":: Falhar na atualizado do NTP 1 ::");

    ntpClient2.begin();
    tentativa = 0;

    //Espera pelo primeiro update online
    Serial.println(":: Atualizar horario pelo NTP ::");
    while (ntpAtualizado == 0 && !ntpClient2.update() && tentativa < 4)
    {
      Serial.print(".");
      ntpClient2.forceUpdate();
      tentativa++;
      vTaskDelay(pdMS_TO_TICKS(500));
    }

    if (tentativa < 4)
    {
      ntpAtualizado = 1;
      Serial.println();
      Serial.println(":: NTP atualizado 2 ::");

      //Atualizar RTC
      rtc.setTime(ntpClient2.getEpochTime());

      Serial.print("Hora NTP: ");
      Serial.println(ntpClient2.getEpochTime());

      Serial.print("Hora RTC: ");
      Serial.println(rtc.getEpoch());
    }
    else
    {
      ntpAtualizado = 0;
      Serial.println();
      Serial.println(":: Falhar na atualizado do NTP 2 ::");
    }
  }

  return ntpAtualizado;
}
