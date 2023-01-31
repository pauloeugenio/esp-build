/**
 * @file oled_config.cpp
 * @brief Arquivo de definição das funções de uso do display OLED.
 * 
 * @author --
 * @date 05 de Maio de 2021
 */

#include "oled_config.h"

void oled_global_config(SSD1306Wire &display)
{
  // initialize pins
  pinMode(OLED_SWITCH, INPUT); 
  pinMode(OLED_BUTTON, INPUT); 

  display.init();
  display.flipScreenVertically();
  display.setContrast(255);
  display.clear();

  display.setFont(ArialMT_Plain_16);
  display.setTextAlignment(TEXT_ALIGN_CENTER);
}

void display_loop(SSD1306Wire &display, SmartReadings readings, oled_screen_t &screen, oled_mode_t mode)
{
  char *str1, *str2, *str3;
  int pos;
  oled_screen_t this_screen;
  oled_mode_t this_mode;

  // update screen for next run
  this_screen = (oled_screen_t) (screen % OLED_BLANK_SCREEN);
  this_mode = (oled_mode_t) (mode % OLED_BLANK_MODE);
  if(mode==OLED_SEQUENTIAL_MODE) screen = (oled_screen_t) (screen+1);

  switch(this_screen)
  {
    case 0:
      display.setTextAlignment(TEXT_ALIGN_CENTER);
      pos = MIDDLE_OF_SCREEN;
      asprintf(&str1, "");
      asprintf(&str2, "SmartMeter 2.0");
      asprintf(&str3, "");
      break;
    case 1:
      display.setTextAlignment(TEXT_ALIGN_LEFT);
      pos = LEFT_OF_SCREEN;
      asprintf(&str1, "VA: %.2f V", readings.VA);
      asprintf(&str2, "VB: %.2f V", readings.VB);
      asprintf(&str3, "VC: %.2f V", readings.VC);
      break;
    case 2:
      display.setTextAlignment(TEXT_ALIGN_LEFT);
      pos = LEFT_OF_SCREEN;
      asprintf(&str1, "IA: %.2f A", readings.IA);
      asprintf(&str2, "IB: %.2f A", readings.IB);
      asprintf(&str3, "IC: %.2f A", readings.IC);
      break;
    case 3:
      display.setTextAlignment(TEXT_ALIGN_LEFT);
      pos = LEFT_OF_SCREEN;
      asprintf(&str1, "PA: %.2f kW", readings.PA);
      asprintf(&str2, "PB: %.2f kW", readings.PB);
      asprintf(&str3, "PC: %.2f kW", readings.PC);
      break;
    case 4:
      display.setTextAlignment(TEXT_ALIGN_LEFT);
      pos = LEFT_OF_SCREEN;
      asprintf(&str1, "QA: %.2f kvar", readings.QA);
      asprintf(&str2, "QB: %.2f kvar", readings.QB);
      asprintf(&str3, "QC: %.2f kvar", readings.QC);
      break;
    case 5:
      display.setTextAlignment(TEXT_ALIGN_LEFT);
      pos = LEFT_OF_SCREEN;
      asprintf(&str1, "SA: %.2f kVA", readings.SA);
      asprintf(&str2, "SB: %.2f kVA", readings.SB);
      asprintf(&str3, "SC: %.2f kVA", readings.SC);
      break;
    case 6:
      display.setTextAlignment(TEXT_ALIGN_LEFT);
      pos = LEFT_OF_SCREEN;
      asprintf(&str1, "FPA: %.3f", readings.FPA);
      asprintf(&str2, "FPB: %.3f", readings.FPB);
      asprintf(&str3, "FPC: %.3f", readings.FPC);
      break;
    case 7:
      display.setTextAlignment(TEXT_ALIGN_LEFT);
      pos = LEFT_OF_SCREEN;
      asprintf(&str1, "FA: %.2f Hz", readings.FA);
      asprintf(&str2, "FB: %.2f Hz", readings.FB);
      asprintf(&str3, "FC: %.2f Hz", readings.FC);
      break;
    default:
      screen=OLED_MAIN_SCREEN;
      break;
  }
  
  display.clear();
  display.drawString(pos, TOP_ROW,    CHAR_TO_STRING(str1));
  display.drawString(pos, MIDDLE_ROW, CHAR_TO_STRING(str2));
  display.drawString(pos, BOTTOM_ROW, CHAR_TO_STRING(str3)); 
  display.display();   
}


