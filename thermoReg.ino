#define _LCD_TYPE 1
#define RegUPpin 11
#define RegDOWNpin 10
#include <Wire.h>
#include <Sodaq_SHT2x.h>
#include <LiquidCrystal_I2C.h>
#include "button.h"
#include "GyverRelay.h"
#include <GyverButton.h>
#include <EEPROM.h>

GButton btn7(7);
GButton btn8(8);
GButton btn3(3);

byte simvol[8]   = {B11100,B10100,B11100,B00000,B00000,B00000,B00000,B00000,}; // Символ градуса

uint8_t bukva_P[8] = {0x1F, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11};
byte bukva_Y[8]   = {B10001,B10001,B10001,B01010,B00100,B01000,B10000,B00000,};
byte bukva_L[8]   = {B00011,B00111,B00101,B00101,B01101,B01001,B11001,B00000,};
byte bukva_ZH[8]  = {B10101,B10101,B10101,B11111,B10101,B10101,B10101,B00000,};
 
LiquidCrystal_I2C lcd(0x27, 20, 4);          // Задаем адрес и размер дисплея

float temp = 10.00;
float hum = 10.00;
float tValue;
float hValue;
bool cur;
int i = 0;
static bool relayUp = false;
static bool relayDown = false;
static uint32_t tmr;

void setup()
{
  Serial.begin(9600);
  hum = EEPROM.read(0);
  temp = EEPROM.read(1);    
  pinMode(RegUPpin, OUTPUT);
  pinMode(RegDOWNpin, OUTPUT); 
  digitalWrite(RegUPpin, 1);
  digitalWrite(RegDOWNpin, 1);
  Wire.begin();                                                        
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.command(0b101000);
  lcd.createChar(4, bukva_P);
  lcd.createChar(2, bukva_Y);
  lcd.createChar(3, bukva_L);
  lcd.createChar(5, bukva_ZH);
  cur = 0;
  lcd.setCursor(0,1);
  lcd.print("\x3E");
  currentValues();                       
}
void loop()
{
  btn3.tick();
  btn7.tick();
  btn8.tick();
  
  if (btn3.isHold()){
    softReset();
  }
  if (btn3.isClick()) {
    if (!cur) {
      lcd.setCursor(0,3);
      lcd.print("\x3E");
      lcd.setCursor(0,1);
      lcd.print(" ");
      cur = 1;
    }
    else if (cur) {
      lcd.setCursor(0,1);
      lcd.print("\x3E");
      lcd.setCursor(0,3);
      lcd.print(" ");
      cur = 0; 
    }
  }
  if (btn7.isClick()){
    if (cur) {
      lcd.setCursor(7, 3);
      hum--;
      if (hum < 0) hum = 0;
      if (hum > 100) hum = 100;
      lcd.print(hum);
      if (hum < 10) {
        lcd.setCursor(12, 3);
        lcd.print(" ");
      }
      EEPROM.update(0, hum);
    }
    if (!cur) {
      temp--;
      if (temp < 10) {
        lcd.setCursor(13, 1);
        lcd.print(" ");
      }
      lcd.setCursor(7, 1);
      lcd.print(temp);
      EEPROM.update(1, temp);
    }
  }  

    if (btn7.isStep()) {                                 // если кнопка была удержана (это для инкремента)
      if (cur) {
        lcd.setCursor(7, 3);
        hum = hum - 2;
        if (hum < 0) hum = 0;
        if (hum > 100) hum = 100;
        lcd.print(hum);
        if (hum < 10) {
          lcd.setCursor(12, 3);
          lcd.print(" ");
        }
        EEPROM.update(0, hum);
      }
      if (!cur) {
      temp = temp - 2;
      if (temp < 10) {
        lcd.setCursor(13, 1);
        lcd.print(" ");
      }
      lcd.setCursor(7, 1);
      lcd.print(temp);
      EEPROM.update(1, temp);
      }
    }
    
  if (btn8.isClick()){
    if (cur) {
      lcd.setCursor(7, 3);
      hum++;
      if (hum > 100) hum = 100;
      lcd.print(hum);
      EEPROM.update(0, hum);
    }
    if (!cur) {
      lcd.setCursor(7, 1);
      temp++;
      lcd.print(temp);
      EEPROM.update(1, temp);
    }
  }
    if (btn8.isStep()) {                                 // если кнопка была удержана (это для инкремента)
    if (cur) {
      lcd.setCursor(7, 3);
      hum = hum + 3;
      if (hum > 100) hum = 100;
      lcd.print(hum);
      EEPROM.update(0, hum);
    }
    if (!cur) {
      lcd.setCursor(7, 1);
      temp = temp + 3;
      lcd.print(temp);
      EEPROM.update(1, temp);
    }
  }
  if (i > 500) { 
   currentValues();
   i = 0; 
  }
  i++;
  customValues();

  if (millis() - tmr >= 1000) {
    tmr = millis();
    if (SHT2x.GetHumidity() < (hum - 3)) digitalWrite(RegUPpin, 0); 
    else digitalWrite(RegUPpin, 1);
    if (SHT2x.GetHumidity() > (hum + 3)) digitalWrite(RegDOWNpin, 0); 
    else digitalWrite(RegDOWNpin, 1);
  }
}

void currentValues() {
  float tValue = SHT2x.GetTemperature();
  float hValue = SHT2x.GetHumidity();
  lcd.createChar(1, simvol);
  lcd.setCursor(0,0);
  lcd.print("TEM\4""EPAT\2""PA\x3A");
  lcd.setCursor(13, 0);
  lcd.print(tValue);
  lcd.setCursor(18,0);                         
  lcd.print("\1");
  lcd.print("C");                                             
  lcd.setCursor(0, 2);
  lcd.print("B\3A\5HOCTb\x3A");
  lcd.setCursor(13,2);
  lcd.print(hValue);
  lcd.setCursor(18,2);   
  lcd.print("\x25"); 
}
void customValues() { 
  lcd.setCursor(7,1);
  lcd.print(temp);
  lcd.print("\1");
  lcd.print("C");                                             
  lcd.setCursor(7,3);
  lcd.print(hum);
  lcd.print("\x25");
}
void softReset() {
  asm volatile ("jmp 0");
}