#define tRegUPpin 12
#define tRegDOWNpin 9
#define hRegUPpin 11
#define hRegDOWNpin 10

//#define dht.begin(); Wire.begin();  //Uncomment this string if you want use DHT11 or DHT22 instead GY-21
//#define dht.readHumidity() SHT2x.GetHumidity()  //Uncomment this string if you want to use DHT for humidity
//#define dht.readTemperature() SHT2x.GetTemperature()  //Uncomment this string if you want to use DHT for temperature

//#define Eng(); Rus();  //Uncomment this string if you want change language to english

//#include "DHT.h"
#include <Wire.h>
#include <Sodaq_SHT2x.h>
#include <LiquidCrystal_I2C.h>
#include <GyverButton.h>
#include <EEPROM.h>

GButton btn7(7);
GButton btn8(8);
GButton btn3(3);

byte simvol[8] = {
  B11100,
  B10100,
  B11100,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
};  //Celsium simbol
uint8_t bukva_P[8] = { 0x1F, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11 };  // letter П
byte bukva_Y[8] = {
  B10001,
  B10001,
  B10001,
  B01010,
  B00100,
  B01000,
  B10000,
  B00000,
};  // letter У
byte bukva_L[8] = {
  B00011,
  B00111,
  B00101,
  B00101,
  B01101,
  B01001,
  B11001,
  B00000,
};  // letter Л
byte bukva_ZH[8] = {
  B10101,
  B10101,
  B10101,
  B11111,
  B10101,
  B10101,
  B10101,
  B00000,
};  // letter Ж

LiquidCrystal_I2C lcd(0x27, 20, 4);

//if your display doesn't work, check the connection and change the address to: 0x3F

float temp = 10.00;   // initial value of temperature
float hum = 10.00;    // initial value of humidity
float tValue;         // variable for temp from gy-21
float hValue;         // variable for hum from gy-21
int cur = 1;              // variable to track the cursor
float hhyst = 5.00;
float thyst = 5.00;

void setup() {
  hum = EEPROM.read(0);        // Read hum from eeprom
  temp = EEPROM.read(1);       // Read temp from eeprom
  thyst = EEPROM.read(2);      // Read thyst from eeprom
  hhyst = EEPROM.read(3);      // Read hhyst from eeprom
  pinMode(hRegUPpin, OUTPUT);  // PinMode of relay
  pinMode(hRegDOWNpin, OUTPUT);
  digitalWrite(hRegUPpin, 1);  // initial state
  digitalWrite(hRegDOWNpin, 1);
  pinMode(tRegUPpin, OUTPUT);  // PinMode of relay
  pinMode(tRegDOWNpin, OUTPUT);
  digitalWrite(tRegUPpin, 1);  // initial state
  digitalWrite(tRegDOWNpin, 1);
  Wire.begin();
  lcd.init();
  lcd.clear();
  lcd.command(0b101000);
  lcd.createChar(4, bukva_P);  // creating simbols
  lcd.createChar(2, bukva_Y);
  lcd.createChar(3, bukva_L);
  lcd.createChar(5, bukva_ZH);
  lcd.setCursor(0, 1);
  lcd.print("\x3E");  // cursor simbol
  currentValues();
}
void loop() {
  btn3.tick();
  btn7.tick();  // tracking the button state
  btn8.tick();

  if (btn3.isHold()) softReset();  //reset for save the settings

  if (btn3.isClick()) {  //processing of clicks
    lcd.backlight();
    switch (cur) {
      case 1:
        lcd.setCursor(0, 1);
        lcd.print("\x3E");
        lcd.setCursor(10, 3);
        lcd.print(" ");
        cur = 2;
        break;

      case 2:
        lcd.setCursor(10, 1);
        lcd.print("\x3E");
        lcd.setCursor(0, 1);
        lcd.print(" ");
        cur = 3;
        break;

      case 3:
        lcd.setCursor(0, 3);
        lcd.print("\x3E");
        lcd.setCursor(10, 1);
        lcd.print(" ");
        cur = 4;
        break;

      case 4:
        lcd.setCursor(10, 3);
        lcd.print("\x3E");
        lcd.setCursor(0, 3);
        lcd.print(" ");
        cur = 1;
        break;
    }
  }
  if (btn7.isClick() || btn7.isStep()) {
    lcd.display();
    switch (cur) {
      case 2:
        thyst--;
        if (thyst < 1) thyst = 1;
        EEPROM.update(2, thyst);
        break;

      case 3:
        temp--;
        if (temp < 10) {
          lcd.setCursor(19, 1);
          lcd.print(" ");
        }
        EEPROM.update(1, temp);
        break;

      case 4:
        hhyst--;
        if (hhyst < 1) hhyst = 1;
        EEPROM.update(3, hhyst);
        break;

      case 1:
        hum--;
        if (hum < 0) hum = 0;
        if (hum > 100) hum = 100;
        if (hum < 10) {
          lcd.setCursor(18, 3);
          lcd.print(" ");
        }
        EEPROM.update(0, hum);
        break;
    }
  }


  if (btn8.isClick() || btn8.isStep()) {
    lcd.display();
    switch (cur) {
      case 2:
        thyst++;
        EEPROM.update(2, thyst);
        break;

      case 3:
        temp++;
        EEPROM.update(1, temp);
        break;

      case 4:
        hhyst++;
        EEPROM.update(3, hhyst);
        break;

      case 1:
        hum++;
        if (hum > 100) hum = 100;
        EEPROM.update(0, hum);
        break;
    }
  }
  static uint32_t tmr;
  if (millis() - tmr >= 5000) {  // reading values from sensors
    tmr = millis();
    currentValues();
    customValues();
  }
  static uint32_t tmr2;
  if (millis() - tmr2 >= 1000) {  //computing the relay state with hysteresis
    tmr2 = millis();
    if (SHT2x.GetHumidity() < (hum - hhyst)) digitalWrite(hRegUPpin, 0);
    else
      digitalWrite(hRegUPpin, 1);
    if (SHT2x.GetHumidity() > (hum + hhyst)) digitalWrite(hRegDOWNpin, 0);
    else
      digitalWrite(hRegDOWNpin, 1);

    if (SHT2x.GetTemperature() < (temp - thyst)) digitalWrite(tRegUPpin, 0);
    else
      digitalWrite(tRegUPpin, 1);
    if (SHT2x.GetTemperature() > (temp + thyst)) digitalWrite(tRegDOWNpin, 0);
    else
      digitalWrite(tRegDOWNpin, 1);
  }
  static uint32_t tmr3;
  if (millis() - tmr3 == 5000) {
    tmr3 = millis();
    lcd.noDisplay();
  }
}

void currentValues() {
  float tValue = SHT2x.GetTemperature();  //temp value from sensor
  float hValue = SHT2x.GetHumidity();     //hum value from sensor
  Rus();
}

void customValues() {
  lcd.setCursor(12, 1);
  lcd.print(temp);
  lcd.print("\1");
  lcd.print("C");
  lcd.setCursor(12, 3);
  lcd.print(hum);
  lcd.print("\x25");
  lcd.setCursor(3, 3);
  lcd.print(hhyst);
  lcd.setCursor(3, 1);
  lcd.print(thyst);
}

void Rus() {                  //Russian language
  lcd.createChar(1, simvol);  //rendering text and values
  lcd.setCursor(0, 0);
  lcd.print("TEM\4"
            "EPAT\2"
            "PA\x3A");
  lcd.setCursor(13, 0);
  lcd.print(tValue);
  lcd.setCursor(18, 0);
  lcd.print("\1");
  lcd.print("C");
  lcd.setCursor(0, 2);
  lcd.print("B\3A\5HOCTb\x3A");
  lcd.setCursor(13, 2);
  lcd.print(hValue);
  lcd.setCursor(18, 2);
  lcd.print("\x25");
}

void Eng() {                  //English language
  lcd.createChar(1, simvol);  //rendering text and values
  lcd.setCursor(0, 0);
  lcd.print("Temperature\x3A");
  lcd.setCursor(13, 0);
  lcd.print(tValue);
  lcd.setCursor(18, 0);
  lcd.print("\1");
  lcd.print("C");
  lcd.setCursor(0, 2);
  lcd.print("Humidity\x3A");
  lcd.setCursor(13, 2);
  lcd.print(hValue);
  lcd.setCursor(18, 2);
  lcd.print("\x25");
}

void softReset() {asm volatile("jmp 0");}  //soft reset method
