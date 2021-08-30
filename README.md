# thermoReg
This repository contains the code and scheme for repeating the thermostat steam project for the climatic chamber, which is used for damp-drying sausages.

Glad to see constructive criticism of the code.
---
The thermostat for the climate chamber maintains the set temperature and humidity using hysteresis. The project was created for a Russian customer, therefore "humidity" and "temperature" are written in Russian, this can be changed if desired (At the beginning of the code there is a setting of the language and type of sensor GY-21 or DHTxx sensors). The device scheme looks like this:
![Schematic_thermoRegulator_2021-08-31](https://user-images.githubusercontent.com/66080483/131405625-526c3f9e-105f-40e0-a3dd-262c95d7d9cb.png)


Components:
1. Arduino Nano v3
2. LCD Display 2004 i2c
3. 3 buttons
4. Temperature and humidity sensor GY-21
5. 4 channel electromagnetic relay
6. Wires/PCB
7. Box (**box isn't adapted to PCB!**)
8. Patience 

![photo_2021-08-30_23-27-26 (2)](https://user-images.githubusercontent.com/66080483/131401637-1efc7748-a6ba-4daa-a6ec-16509663b7be.jpg)

![23](https://user-images.githubusercontent.com/66080483/131401718-eb4f93d9-33dd-42a0-a556-afeff1701743.jpg)

![photo_2021-08-30_23-27-26](https://user-images.githubusercontent.com/66080483/131401775-b594b9d8-a712-4753-addb-428571adad42.jpg)

The sensor is placed at some distance from the control unit so that it can be easily shoved into the climate chamber

I have developed a printed circuit board for this project, the gerber file is in the source code

![Screenshot_2](https://user-images.githubusercontent.com/66080483/131401967-4e46321c-ae81-42c4-93b8-c9d6cbb0f026.png)

![Screenshot_3](https://user-images.githubusercontent.com/66080483/131401972-cabb6f8a-0c40-46be-bcfe-1c32c723b161.png)

----------------------------------------------------------------------
If these materials were useful to you, please subscribe
