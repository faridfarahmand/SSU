# Arduino_Camera_Web_Server

This repository contains an example application for an ESP32-based camera server. The program configures the ESP32 to capture images using a camera module, and streams them over WiFi to a web browser UI. This builds off of the example web server project in the Arduino IDE.

This application uses the ESP32's integrated WiFi capabilities and PSRAM. When a user connects to the ESP32's IP address, they will be able to see the images captured by the camera and can determine various settings and formatting. 

This application only supports 1 active connection to the UI.


# Requirements
Freenove ESP32 WROVER Board
Camera module compatible with the ESP32 board
Arduino IDE version 1.8.16

# Dependencies
esp_camera.h: This library is used to interact with the camera module.
WiFi.h: This library is used to establish a WiFi connection.
camera_pins.h: This file should define the pins used by the camera module.

# Setup
The camera model used must be defined at the start of the script. 

# MAC Address Spoofing (Optional)
This script includes commented out code that can be used to spoof the ESP32's MAC address. If you wish to use this functionality, uncomment the relevant code and replace CustomMACaddress with the desired MAC address.

# WiFi Credentials
Replace the ssid and password variables with your WiFi credentials.

# Usage
Upload the sketch to your ESP32 board using the Arduino IDE. Once the board is powered on, it will start the camera server and connect to the WiFi network. You can then connect to the ESP32's IP address to view the images captured by the camera.

The loop function in this sketch does not contain any functionality as everything is handled in separate tasks by the web server.

# Know Issues
If the camera fails to initialize, the error code will be printed to the Serial Monitor. You may have to hit the RST button a few times on the ESP32 board. Please refer to the ESP32 Camera library documentation for more information on what these codes mean.

The SSID & Password variables only support text and numerical characters, it doesn't not recognize spaces or special characters. 

# Contributing
If you'd like to contribute, please fork the repository and make changes as you'd like. Pull requests are warmly welcome.

# License
MIT license applies.

