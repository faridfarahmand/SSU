UART to ESP-01 to ThingSpeak Integration on PIC18F46K42
In this project we will be configuring a PIC18F4Kk42 in order to send data to a ThingSpeak server using an ESP-01 module. 
You will need the following before beggining:
- PIC18F46K42
- PICKIT4 Programmer
- ESP-01 Module
- FT232RL Serial Converter (or any USB-to-UART Bridge)
- Various Resistors
- 3.3 V Power Source (power supply or battery is fine)
  

1. The first step we must follow is to configure the UART connection on our PIC microcontoller. Follow this tutorial to configure UART using the MCC function on MPLABX: https://docs.google.com/presentation/d/1JKKtNu3tkO9oB1XLOM3FUndg2SQ4MABi4Z2qnZM9kFc/edit#slide=id. Once configured you should have the following files. 

   
   ![image](https://github.com/avendano1977/EE498---Wifi-UART-integration-on-PIC18/assets/80984104/ff51e7fa-b261-42db-acf9-1fa1296049d6)


Now you can execture the UART-Serial Monitor.c file and communcite serially with the PIC. Make sure the connections are as shown in the schematic below.


![Schematic_EE498 - ESP-01 WIFI integration with PIC18F46k42_2023-11-30 (1)](https://github.com/avendano1977/EE498---Wifi-UART-integration-on-PIC18/assets/80984104/fdca459b-2008-4aa4-be2f-c35fa467eaac)
   
2. The next step is configure the ESP-01 module by itself and send data to ThingSpeak. Follow the tutorial to configure ThingSpeak and the ESP-01 module: https://github.com/avendano1977/EE498---Wifi-UART-integration-on-PIC18/blob/main/ESP-01_Interface.pdf. At this point you should see the data sent from the ESP-01 being plotted on ThingSpeak. ![image](https://github.com/avendano1977/EE498---Wifi-UART-integration-on-PIC18/assets/80984104/debeaae9-37a2-48d4-a63d-48f458892083)
   
3. Now that we have finished configuring the PIC18F46K42, ESP-01, and ThingSpeak we can integrate all three in order to create a very basic IoT system. Follow the schematic shown below. Once the connections are correct execeute the UART-ESP-01.c file and make sure to add in your own WiFi connection details. At this point the project has been completed and the opportunities are endless. Try connecting a sensor and sending data to ThingSpeak to visualize it! While there is only bidirectionality to check whether the initial connection between the ESP-01 and PIC is solid, try to see if you can receive input from ThingSpeak for remote control!
 
![Schematic_EE498 - ESP-01 WIFI integration with PIC18F46k42_2023-11-30 (2)](https://github.com/avendano1977/EE498---Wifi-UART-integration-on-PIC18/assets/80984104/49df0280-64fe-47ba-b4cd-4e6416abf4cd)

