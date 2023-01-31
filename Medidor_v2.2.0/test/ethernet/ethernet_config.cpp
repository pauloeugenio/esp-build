/**
 * @file ethernet_config.cpp
 * @brief Arquivo de configuração para estabelecimento da conexão com fio
 *
 * @author --
 * @date 05 de Maio de 2021
 */

#include "ethernet_config.h"
#include "main.h"

EthernetClient ethClient;
PubSubClient pubsub_client;

EthernetUDP udp_ethernet;
ESP32Time rtc;

int timeZone = -3;
int ntpAtualizado = 0;

uint8_t mac[6] = {0x00,0x01,0x02,0x03,0x04,0x05}; // Mudar MAC p/cada medidor
IPAddress IP(192,168,88,111); //IP Static sem DCHP

IPAddress mqttServer1(192,168,88,217);
const int mqttPort1 = 1885;
const char *mqttUser1 = "";
const char *mqttPassword1 = "";


IPAddress mqttServer2(10,7,52,80);
const int mqttPort2 = 1883;
const char *mqttUser2 = "";
const char *mqttPassword2 = "";

IPAddress mqttServer3(10,7,53,213);
const int mqttPort3 = 1883;
const char *mqttUser3 = "";
const char *mqttPassword3 = "";


NTPClient ntpClient1(
  udp_ethernet,             //socket udp
  NTP_SERVER_3,    //URL do server NTP
  timeZone * 3600, //Deslocamento do horário em relacão ao GMT 0
  60000            //Intervalo entre verificações online
);

NTPClient ntpClient2(
    udp_ethernet,             //socket udp
    NTP_UFRN_3,      //URL do servwer NTP
    timeZone * 3600, //Deslocamento do horário em relacão ao GMT 0
    60000            //Intervalo entre verificações online
);

void setupNTP()
{
    //Inicializa o client NTP
    ntpClient1.begin();
    int tentativa = 0;

    //Espera pelo primeiro update online
    Serial.println("\n\n:: Atualizar horario pelo NTP ::");
    while (!ntpClient1.update() && tentativa < 4)
    {
        Serial.print(".");
        ntpClient1.forceUpdate();
        tentativa++;
        delay(500);
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
            delay(500);
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
}

void setupEthernet()
{
    
}