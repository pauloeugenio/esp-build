/**
 * @file spiffs_config.h
 * @brief Arquivo com funcionalidades para gerenciamento do SPIFFS
 *
 * @author --
 * @date 03 de Maio de 2022
 */

#include "spiffs_config.h"


void list_files(bool list_content)
{
  Serial.println(":: List files in root ::");
  File root = SPIFFS.open("/");
  if(!root){
    Serial.println("Failed to open root directory!");
    return;
  }

  File file = root.openNextFile();
  while(file){
    if(file.isDirectory())
    {
      Serial.print("Directory: ");
      Serial.println(file.name());
    } 
    else 
    {
      Serial.print("Filename: ");
      Serial.println(file.name());
      Serial.print("Size: ");
      Serial.println(file.size());
    }
    if(list_content) list_file_content(file);
    file = root.openNextFile();
  }
  Serial.println();
  Serial.println();
  root.close();
}

void list_file_content(File file)
{
  String line;
  while(file.available()){
    line = file.readStringUntil('\n');
    Serial.println(line);
  }
}

File open_file(String filename)
{
  File file = SPIFFS.open(filename, "a"); // a append mode
  if (!file || file.isDirectory()){Serial.println("Error opening the file!");}
  return file;
}

File write_line(File &file, String line)
{
  if(file) file.print(line);
  return file;
}

void format_spiffs()
{
  Serial.println("Formating SPIFFS...");
  SPIFFS.format();
  Serial.println("SPIFFS formated!");
}
