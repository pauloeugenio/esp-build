/**
 * @file ota_config.cpp
 * @brief Arquivo de configuração para atualizações over-the-air (OTA)
 *
 * @author --
 * @date 25 Outubro de 2021
 */

#include "ota_config.h"

void update_spiffs(WiFiClientSecure &client_ota, SensorConstants constants)
{
    char* url_spiffs;

    asprintf(&url_spiffs, "https://raw.githubusercontent.com/3w3rt0n/OTA/main/MEDIDOR_v2/SPIFFS/%s_%s.spiffs.bin", constants.id_ota, constants.nextVersionSPIFFFS);

    Serial.print("URL: ");
    Serial.println(url_spiffs);

    // LED indicador de progresso
    httpUpdate.setLedPin(2, HIGH);

    // Callback - Progresso
    Update.onProgress([](size_t progresso, size_t total)
                      {
                          Serial.print(progresso * 100 / total);
                          Serial.print(" ");
                      });

    // Não reiniciar automaticamente
    httpUpdate.rebootOnUpdate(false);

    // Instrui Cliente a ignorar assinatura do Servidor na conexao segura
    client_ota.setInsecure();

    // Encerra SPIFFS
    SPIFFS.end();

    // Efetua atualização da SPIFFS
    t_httpUpdate_return resultado = httpUpdate.updateSpiffs(client_ota, url_spiffs);

    // Verifica resultado
    switch (resultado)
    {
        case HTTP_UPDATE_FAILED:
        {
            String s = httpUpdate.getLastErrorString();
            Serial.println("\nFalha: " + s);
        }
        break;
        case HTTP_UPDATE_NO_UPDATES:
        {
            Serial.println("\nNenhuma atualização disponível");
        }
        break;
        case HTTP_UPDATE_OK:
        {
            Serial.println("\nSucesso");
            ESP.restart();
        }
        break;
    }

    // Inicializa SPIFFS
    if (SPIFFS.begin())
    {
        Serial.println("SPIFFS Ok");
    }
    else
    {
        Serial.println("SPIFFS Falha");
    }
}

void update_sketch(WiFiClientSecure &client_ota, SensorConstants constants)
{
    char* url_sketch;

    asprintf(&url_sketch, "https://raw.githubusercontent.com/3w3rt0n/OTA/main/MEDIDOR_v2/Sketch/%s_firmware_%s.ino.esp32.bin", constants.id_ota, NEXT_VERSION_SKETCH);

    Serial.print("URL: ");
    Serial.println(url_sketch);

    // LED indicador de progresso
    httpUpdate.setLedPin(2, HIGH);

    // Callback - Progresso
    Update.onProgress([](size_t progresso, size_t total)
                      {
                          Serial.print(progresso * 100 / total);
                          Serial.print(" ");
                      });

    // Não reiniciar automaticamente
    httpUpdate.rebootOnUpdate(false);

    // Instrui Cliente a ignorar assinatura do Servidor na conexao segura
    client_ota.setInsecure();

    // Efetua atualização do Sketch
    t_httpUpdate_return resultado = httpUpdate.update(client_ota, url_sketch);

    // Verifica resultado
    switch (resultado)
    {
    case HTTP_UPDATE_FAILED:
    {
        String s = httpUpdate.getLastErrorString();
        Serial.println("\nFalha: " + s);
    }
    break;
    case HTTP_UPDATE_NO_UPDATES:
        Serial.println("\nNenhuma atualização disponível");
        break;
    case HTTP_UPDATE_OK:
    {
        Serial.println("\nSucesso, reiniciando...");
        ESP.restart();
    }
    break;
    }
}

void read_constants(SensorConstants &constants)
{
    File arquivo = SPIFFS.open(constants.ge_fname);

    String linha;
    char buf[40];
    char *str;

    if (arquivo)
    {
        while (arquivo.available())
        {
            linha = arquivo.readStringUntil('\n');
            linha.toCharArray(buf, sizeof(buf));
            char *p = buf;
            while ((str = strtok_r(p, "=", &p)) != NULL)
            {
                //Serial.println(str);
                if (strcmp(str, "ssid") == 0)
                {
                    str = strtok_r(p, ";", &p);
                    strcpy(constants.SSID, str);
                    Serial.print("SSID: ");
                    Serial.println(constants.SSID);
                    // byte lastChar = strlen(SSID)-1;
                    // SSID[lastChar] = '\0';
                    break;
                }

                if (strcmp(str, "password") == 0) //Fazer alteracao
                {
                    str = strtok_r(p, ";", &p);
                    if(strcmp(str, " ") != 0)
                    {
                        strcpy(constants.PASSWD, str);
                        Serial.print("PASSWD: ");
                        Serial.print("~");
                        Serial.print(constants.PASSWD);
                        Serial.println("~");
                    } else
                    {
                        constants.PASSWD[0] = '\0';
                        Serial.println("PASSWD: ~NULL~");
                    }
                    
                    // byte lastChar = strlen(PASSWD);
                    // PASSWD[lastChar] = '\0';
                    break;
                }
                if (strcmp(str, "tc") == 0)
                {
                    str = strtok_r(p, ";", &p);
                    byte lastChar = strlen(str);
                    str[lastChar] = '\0';
                    constants.tc = atof(str);
                    Serial.print("tc: ");
                    Serial.println(constants.tc);
                    break;
                }
                if (strcmp(str, "coefv1_lab_0") == 0)
                {
                    str = strtok_r(p, ";", &p);
                    byte lastChar = strlen(str);
                    str[lastChar] = '\0';
                    constants.coefv1_lab[0] = atof(str);
                    Serial.print("coefv1_lab_0: ");
                    Serial.println(constants.coefv1_lab[0]);
                    break;
                }
                if (strcmp(str, "coefv1_lab_1") == 0)
                {
                    str = strtok_r(p, ";", &p);
                    byte lastChar = strlen(str);
                    str[lastChar] = '\0';
                    constants.coefv1_lab[1] = atof(str);
                    Serial.print("coefv1_lab_1: ");
                    Serial.println(constants.coefv1_lab[1]);
                    break;
                }

                if (strcmp(str, "coefv2_lab_0") == 0)
                {
                    str = strtok_r(p, ";", &p);
                    byte lastChar = strlen(str);
                    str[lastChar] = '\0';
                    constants.coefv2_lab[0] = atof(str);
                    Serial.print("coefv2_lab_0: ");
                    Serial.println(constants.coefv2_lab[0]);
                    break;
                }
                if (strcmp(str, "coefv2_lab_1") == 0)
                {
                    str = strtok_r(p, ";", &p);
                    byte lastChar = strlen(str);
                    str[lastChar] = '\0';
                    constants.coefv2_lab[1] = atof(str);
                    Serial.print("coefv2_lab_1: ");
                    Serial.println(constants.coefv2_lab[1]);
                    break;
                }

                if (strcmp(str, "coefv3_lab_0") == 0)
                {
                    str = strtok_r(p, ";", &p);
                    byte lastChar = strlen(str);
                    str[lastChar] = '\0';
                    constants.coefv3_lab[0] = atof(str);
                    Serial.print("coefv3_lab_0: ");
                    Serial.println(constants.coefv3_lab[0]);
                    break;
                }
                if (strcmp(str, "coefv3_lab_1") == 0)
                {
                    str = strtok_r(p, ";", &p);
                    byte lastChar = strlen(str);
                    str[lastChar] = '\0';
                    constants.coefv3_lab[1] = atof(str);
                    Serial.print("coefv3_lab_1: ");
                    Serial.println(constants.coefv3_lab[1]);
                    break;
                }

                if (strcmp(str, "coefv1_field_0") == 0)
                {
                    str = strtok_r(p, ";", &p);
                    byte lastChar = strlen(str);
                    str[lastChar] = '\0';
                    constants.coefv1_field[0] = atof(str);
                    Serial.print("coefv1_field_0: ");
                    Serial.println(constants.coefv1_field[0]);
                    break;
                }
                if (strcmp(str, "coefv1_field_1") == 0)
                {
                    str = strtok_r(p, ";", &p);
                    byte lastChar = strlen(str);
                    str[lastChar] = '\0';
                    constants.coefv1_field[1] = atof(str);
                    Serial.print("coefv1_field_1: ");
                    Serial.println(constants.coefv1_field[1]);
                    break;
                }

                if (strcmp(str, "coefv2_field_0") == 0)
                {
                    str = strtok_r(p, ";", &p);
                    byte lastChar = strlen(str);
                    str[lastChar] = '\0';
                    constants.coefv2_field[0] = atof(str);
                    Serial.print("coefv2_field_0: ");
                    Serial.println(constants.coefv2_field[0]);
                    break;
                }
                if (strcmp(str, "coefv2_field_1") == 0)
                {
                    str = strtok_r(p, ";", &p);
                    byte lastChar = strlen(str);
                    str[lastChar] = '\0';
                    constants.coefv2_field[1] = atof(str);
                    Serial.print("coefv2_field_1: ");
                    Serial.println(constants.coefv2_field[1]);
                    break;
                }

                if (strcmp(str, "coefv3_field_0") == 0)
                {
                    str = strtok_r(p, ";", &p);
                    byte lastChar = strlen(str);
                    str[lastChar] = '\0';
                    constants.coefv3_field[0] = atof(str);
                    Serial.print("coefv3_field_0: ");
                    Serial.println(constants.coefv3_field[0]);
                    break;
                }
                if (strcmp(str, "coefv3_field_1") == 0)
                {
                    str = strtok_r(p, ";", &p);
                    byte lastChar = strlen(str);
                    str[lastChar] = '\0';
                    constants.coefv3_field[1] = atof(str);
                    Serial.print("coefv3_field_1: ");
                    Serial.println(constants.coefv3_field[1]);
                    break;
                }

                if (strcmp(str, "coefi1_lab_0") == 0)
                {
                    str = strtok_r(p, ";", &p);
                    byte lastChar = strlen(str);
                    str[lastChar] = '\0';
                    constants.coefi1_lab[0] = atof(str);
                    Serial.print("coefi1_lab_0: ");
                    Serial.println(constants.coefi1_lab[0]);
                    break;
                }
                if (strcmp(str, "coefi1_lab_1") == 0)
                {
                    str = strtok_r(p, ";", &p);
                    byte lastChar = strlen(str);
                    str[lastChar] = '\0';
                    constants.coefi1_lab[1] = atof(str);
                    Serial.print("coefi1_lab_1: ");
                    Serial.println(constants.coefi1_lab[1]);
                    break;
                }
                if (strcmp(str, "coefi1_lab_2") == 0)
                {
                    str = strtok_r(p, ";", &p);
                    byte lastChar = strlen(str);
                    str[lastChar] = '\0';
                    constants.coefi1_lab[2] = atof(str);
                    Serial.print("coefi1_lab_2: ");
                    Serial.println(constants.coefi1_lab[2]);
                    break;
                }

                if (strcmp(str, "coefi2_lab_0") == 0)
                {
                    str = strtok_r(p, ";", &p);
                    byte lastChar = strlen(str);
                    str[lastChar] = '\0';
                    constants.coefi2_lab[0] = atof(str);
                    Serial.print("coefi2_lab_0: ");
                    Serial.println(constants.coefi2_lab[0]);
                    break;
                }
                if (strcmp(str, "coefi2_lab_1") == 0)
                {
                    str = strtok_r(p, ";", &p);
                    byte lastChar = strlen(str);
                    str[lastChar] = '\0';
                    constants.coefi2_lab[1] = atof(str);
                    Serial.print("coefi2_lab_1: ");
                    Serial.println(constants.coefi2_lab[1]);
                    break;
                }
                if (strcmp(str, "coefi2_lab_2") == 0)
                {
                    str = strtok_r(p, ";", &p);
                    byte lastChar = strlen(str);
                    str[lastChar] = '\0';
                    constants.coefi2_lab[2] = atof(str);
                    Serial.print("coefi2_lab_2: ");
                    Serial.println(constants.coefi2_lab[2]);
                    break;
                }

                if (strcmp(str, "coefi3_lab_0") == 0)
                {
                    str = strtok_r(p, ";", &p);
                    byte lastChar = strlen(str);
                    str[lastChar] = '\0';
                    constants.coefi3_lab[0] = atof(str);
                    Serial.print("coefi3_lab_0: ");
                    Serial.println(constants.coefi3_lab[0]);
                    break;
                }
                if (strcmp(str, "coefi3_lab_1") == 0)
                {
                    str = strtok_r(p, ";", &p);
                    byte lastChar = strlen(str);
                    str[lastChar] = '\0';
                    constants.coefi3_lab[1] = atof(str);
                    Serial.print("coefi3_lab_1: ");
                    Serial.println(constants.coefi3_lab[1]);
                    break;
                }
                if (strcmp(str, "coefi3_lab_2") == 0)
                {
                    str = strtok_r(p, ";", &p);
                    byte lastChar = strlen(str);
                    str[lastChar] = '\0';
                    constants.coefi3_lab[2] = atof(str);
                    Serial.print("coefi3_lab_2: ");
                    Serial.println(constants.coefi3_lab[2]);
                    break;
                }


                if (strcmp(str, "coefi1_field_0") == 0)
                {
                    str = strtok_r(p, ";", &p);
                    byte lastChar = strlen(str);
                    str[lastChar] = '\0';
                    constants.coefi1_field[0] = atof(str);
                    Serial.print("coefi1_field_0: ");
                    Serial.println(constants.coefi1_field[0]);
                    break;
                }
                if (strcmp(str, "coefi1_field_1") == 0)
                {
                    str = strtok_r(p, ";", &p);
                    byte lastChar = strlen(str);
                    str[lastChar] = '\0';
                    constants.coefi1_field[1] = atof(str);
                    Serial.print("coefi1_field_1: ");
                    Serial.println(constants.coefi1_field[1]);
                    break;
                }

                if (strcmp(str, "coefi2_field_0") == 0)
                {
                    str = strtok_r(p, ";", &p);
                    byte lastChar = strlen(str);
                    str[lastChar] = '\0';
                    constants.coefi2_field[0] = atof(str);
                    Serial.print("coefi2_field_0: ");
                    Serial.println(constants.coefi2_field[0]);
                    break;
                }
                if (strcmp(str, "coefi2_field_1") == 0)
                {
                    str = strtok_r(p, ";", &p);
                    byte lastChar = strlen(str);
                    str[lastChar] = '\0';
                    constants.coefi2_field[1] = atof(str);
                    Serial.print("coefi2_field_1: ");
                    Serial.println(constants.coefi2_field[1]);
                    break;
                }

                if (strcmp(str, "coefi3_field_0") == 0)
                {
                    str = strtok_r(p, ";", &p);
                    byte lastChar = strlen(str);
                    str[lastChar] = '\0';
                    constants.coefi3_field[0] = atof(str);
                    Serial.print("coefi3_field_0: ");
                    Serial.println(constants.coefi3_field[0]);
                    break;
                }
                if (strcmp(str, "coefi3_field_1") == 0)
                {
                    str = strtok_r(p, ";", &p);
                    byte lastChar = strlen(str);
                    str[lastChar] = '\0';
                    constants.coefi3_field[1] = atof(str);
                    Serial.print("coefi3_field_1: ");
                    Serial.println(constants.coefi3_field[1]);
                    break;
                }
            }
        }
    }

    arquivo.close();
}

void read_config(SensorConstants &constants)
{
    SPIFFS.begin();

    File file = SPIFFS.open(CONFIG_FILE);
    String line;
    char buf[40];
    char *str;

    if(file)
    {
        while (file.available())
        {
            
            line = file.readStringUntil('\n');
            line.toCharArray(buf, sizeof(buf));
            char *p = buf;
            while ((str = strtok_r(p, "=", &p)) != NULL)
            {
                if(strcmp(str, "id") == 0)
                {
                    str = strtok_r(p, ";", &p);
                    strcpy(constants.id_ota, str);
                    // constants.id_ota = strdup(str);
                    Serial.print("ID_OTA: ");
                    Serial.println(constants.id_ota);
                    //byte lastChar = strlen(id_ota) - 1;
                    //id_ota[lastChar] = '\0';
                    break;
                }

                if(strcmp(str, "actual_spiffs_version") == 0)
                {
                    str = strtok_r(p, ";", &p);
                    strcpy(constants.actualVersionSPIFFFS, str);
                    // constants.actualVersionSPIFFFS = strdup(str);
                    Serial.print("ACTUAL_V_SPIFFS: ");
                    Serial.println(constants.actualVersionSPIFFFS);
                    // byte lastChar = strlen(actualVersionSPIFFFS)-1;
                    // actualVersionSPIFFFS[lastChar-1] = '\0';
                    break;
                }

                if(strcmp(str, "next_spiffs_version") == 0)
                {
                    str = strtok_r(p, ";", &p);
                    strcpy(constants.nextVersionSPIFFFS, str);
                    // constants.nextVersionSPIFFFS = strdup(str);
                    Serial.print("NEXT_V_SPIFFS: ");
                    Serial.println(constants.nextVersionSPIFFFS);
                    // byte lastChar = strlen(nextVersionSPIFFFS)-1;
                    // nextVersionSPIFFFS[lastChar-1] = '\0';
                    break;
                }

                if(strcmp(str, "spiffs_version") == 0)
                {
                    str = strtok_r(p, ";", &p);
                    byte lastChar = strlen(str);
                    str[lastChar] = '\0';
                    constants.spiffs_version = atof(str);
                    Serial.print("spiffs_version: ");
                    Serial.println(constants.spiffs_version);
                    break;
                }

            }
        }
    }

    sprintf(constants.ge_fname, "/%s.txt", constants.id_ota);
    Serial.print("Arquivo do sensor: ");
    Serial.println(constants.ge_fname);
}

SensorConstants parse_sensor_id()
{
    SensorConstants constants = initialize_sensor_constants();

    // Inicializa SPIFFS
    if (SPIFFS.begin()) Serial.println(":: Verificacao da particao SPIFFS: [Ok!] ::\n");
    else Serial.println(":: [ERROR] Atencao falha na verificacao da particao SPIFFS! ::\n");

    // Verifica / exibe arquivo
    if (SPIFFS.exists(CONFIG_FILE))
    {
        File arquivo1 = SPIFFS.open(CONFIG_FILE, "r");
        if (arquivo1)
        {
            Serial.print("Lendo arquivo: ");
            Serial.println(CONFIG_FILE);
            arquivo1.close();

            //Ler constantes de calibração do arquivo de configuracao
            Serial.println("----------INICIO---------");
            read_config(constants);
            Serial.println("------------FIM----------\n\n");
        }
    }
    else
    {
        Serial.println("Arquivo não encontrado");
        Serial.print("\nIniciando, Versao SPPIFFS: ");
        Serial.println(ACTUAL_VERSION_SKETCH);
    }

    // Verifica / exibe arquivo
    if (SPIFFS.exists(constants.ge_fname))
    {
        File arquivo2 = SPIFFS.open(constants.ge_fname, "r");
        if (arquivo2)
        {
            Serial.print("Lendo arquivo: ");
            Serial.println(constants.ge_fname);
            arquivo2.close();

            //Ler constantes de calibração do arquivo de configuracao
            Serial.println("----------INICIO---------");
            read_constants(constants);
            Serial.println("------------FIM----------\n\n");
        }
    }
    else
    {
        Serial.println("Arquivo não encontrado");
        Serial.print("\nIniciando, Versao SPPIFFS: ");
        Serial.println(ACTUAL_VERSION_SKETCH);
    }

    return constants;
}

void ota_update_loop(WiFiClientSecure &client_ota, SensorConstants constants)
{
    Serial.println(":: Atualizar SPIFFS ::");
    // update_spiffs(client_ota, constants);
    Serial.println("<< Atualizar concluido! >>\n");

    Serial.println(":: Atualizar SKETCH ::");
    update_sketch(client_ota, constants);
    Serial.println("<< Atualizar concluido! >>\n");
}