
# CubeCell Module - Sending Data to TTN

This document provides a step-by-step guide to configure and program the CubeCell module to send data to The Things Network (TTN) platform.

## Table of Contents
1. [Arduino IDE Setup](#arduino-ide-setup)
2. [Code Overview](#code-overview)
3. [Hardware Setup](#hardware-setup)
4. [TTN Configuration](#ttn-configuration)

---

## Arduino IDE Setup

To program the CubeCell module, use the Arduino IDE. Follow these steps to set up the environment:

1. Open the Arduino IDE.
2. Go to **File > Preferences**.
3. In the **Additional Boards Manager URLs** field, add the following URL:
   ```
   https://github.com/HelTecAutomation/CubeCell-Arduino/releases/download/V1.5.0/package_CubeCell_index.json
   ```
4. Install the CubeCell board through the **Boards Manager**.

## Code Overview

This code generates random data, which is defined in the `loop()` function. The random data is sent to TTN using the `LoRaWAN.send()` function.

- **Data Transmission:** Data is sent when the switch connected to GPIO10 is pressed.
- **Switch Configuration:** The switch is set to active HIGH.

To modify data formats or frequency of data transmission, adjust the code in the `loop()` function as required.

## Hardware Setup

1. **Switch Connection:** Connect a switch to **GPIOI0**. Ensure the switch is configured to be active HIGH.

## TTN Configuration

To receive data from the CubeCell module on the TTN platform, configure your TTN application:

1. Create a new TTN application if you haven’t already.

  <img width="587" alt="image" src="https://github.com/user-attachments/assets/ccb96cf3-f3b8-4581-b114-0e14608eb1bb">

2. Add a device corresponding to the CubeCell module.

   <img width="630" alt="image" src="https://github.com/user-attachments/assets/08ee573c-dc40-4829-9947-7460b3f3512a">

3. Configure the **Data Format** in TTN to match the way data is structured in the CubeCell code (see "Code Overview" below for details on data format).

<img width="1120" alt="image" src="https://github.com/user-attachments/assets/d7108c08-c6c8-4510-ad12-0bf63aca9e63">

![image](https://github.com/user-attachments/assets/68a8e118-ce9c-4a13-bf6c-71492beb8c91)

-------------
