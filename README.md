# thermoReg
This repository contains the code and scheme for repeating the thermostat steam project for the climatic chamber, which is used for damp-drying sausages.

Glad to see constructive criticism of the code.
----------------------------------------------------------------------
The thermostat for the climate chamber maintains the set temperature and humidity using hysteresis. The project was created for a Russian customer, therefore "humidity" and "temperature" are written in Russian, this can be changed if desired. The device diagram looks like this:
![Schematic_thermoReg_2021-07-05](https://user-images.githubusercontent.com/66080483/124517396-88148a80-ddec-11eb-9b4c-8cf5a1fa53b4.png)

Components:
1. Arduino Nano v3
2. LCD Display 2004 i2c
3. 3 buttons
4. Temperature and humidity sensor GY-21
5. Single channel electromagnetic relay 2 pieces
6. Wires
7. Box
8. Patience 

![photo_2021-07-06_00-06-56](https://user-images.githubusercontent.com/66080483/124518049-27864d00-ddee-11eb-849c-b20946c32d1a.jpg)
![photo_2021-07-06_00-06-58](https://user-images.githubusercontent.com/66080483/124518052-281ee380-ddee-11eb-88da-7f9a52292e30.jpg)

The sensor is placed at some distance from the control unit so that it can be easily shoved into the climate chamber

In this (v1.0) version the controller can only control humidity with a dehumidifier and humidifier. Because the controller was placed in the refrigerator, where there is an accurate temperature control.

----------------------------------------------------------------------
If these materials were useful to you, please subscribe
