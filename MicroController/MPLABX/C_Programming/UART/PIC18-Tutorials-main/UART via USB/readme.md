# Tutorial Video
This tutorial video has been created by Mr. Diego King. Many thanks for his efforts: 
https://youtu.be/vfWQTlpFnVw

# Introduction to MPLAB MCC 
When starting a new project using PIC16, PIC24, or PIC32 microcontrollers, the setup of the configuration and all the peripherals can be time-consuming, especially for new projects. The MPLAB® Code Configurator (MCC) is an automatically installed plugin for MPLAB X IDE that simplifies this down to a series of simple selections from the menus within the MCC. The MCC generates driver code using a Graphical User Interface (GUI). The generated drivers control the peripherals on PIC® microcontrollers. The GUI provides an easy means for setting up the configuration of the peripherals.

Additionally, the MCC is used to configure and generate libraries, which allows you to configure and generate code for software libraries and off-chip peripherals. The generated drivers or libraries can be used in any PIC device application program.When starting a new project using PIC16, PIC24, or PIC32 microcontrollers, the setup of the configuration and all the peripherals can be time-consuming, especially for new projects. The MPLAB® Code Configurator (MCC) is an automatically installed plugin for MPLAB X IDE that simplifies this down to a series of simple selections from the menus within the MCC. The MCC generates driver code using a Graphical User Interface (GUI). The generated drivers control the peripherals on PIC® microcontrollers. The GUI provides an easy means for setting up the configuration of the peripherals. 

Additionally, the MCC is used to configure and generate libraries, which allows you to configure and generate code for software libraries and off-chip peripherals. The generated drivers or libraries can be used in any PIC device application program. You can learn more here: https://developerhelp.microchip.com/xwiki/bin/view/software-tools/mcc/ 

In this tutorial we demosntrate how to use MCC to configure the USART module. 

# Step 1. Create a Project w/ MCC
Create a new XC8 project for the Curiosity Nano, ensuring "open MCC when finished" is checked. Allow MCC to open, and click through any notifications.

![Step 1](images/step1.png)

# Step 2. Add the UART Module
Under 'Device Resources' in the 'Resource Management' window there is a UART driver module, click the green plus sign to add it to the builder.

![Step 2](images/step2.png)

# Step 3. Specify the Peripheral Library
A UART window should come up with a 'Dependency Selector' option. Select the UART2 peripheral library to add it to the pin grid.

![Step 3](images/step3.png)

# Step 4. Redirect Printf to UART
In the same UART window, after selecting the peripheral library, there should be some configuration settings. Ensure the 'Redirect Printf to UART' option is enabled. In the figure below note that we are using Baudrate of 9600, Parity = NONE, Data Size = 8. Stop Bit = 1, and Control Mode = NONE. 

![Step 4](images/step4.png)

# Step 5. Setup UART Pins using Pin Grid View
The 'Pin Grid View' window should reflect the new UART2 module which was just added. To pick up UART communications through the debugger, the UART RX and TX connections must be configured as: RX2 (input) on RD1 and TX2 (output) on RD0.

![Step 5](images/step5.png)

# Step 6. Generate
That should be all the configuration needed. Click 'Generate' in the 'Resource Management' window and close MCC when it finishes generating the project files.

![Step 6](images/Step6.png)

After generating the code, a number of new functions and classes are generated. You can see a list of all these functional and classes under the tab called Classes.  

![Step 6b](images/step6b.png)

# Step 7. Initialize the new Module
Navigate to main.c in the newly generated project files and add the line `UART2_Initialize();` within the main function. Now, once programmed, the board will write any printf commands to the COM port of your computer.

![Step 7](images/step7.png)

# Step 8. Monitoring the COM ports
Using PuTTY or another serial console program you can now receive communications from a programmed board. If you don't have PuTTY download Tera Term. [Download Tera Term from here:](https://en.softonic.com/download/tera-term/windows/post-download). It is Also possible to use online serial terminals such as  https://serial.huhn.me/. 

![Step 8](images/fin.png)

# Step 9. Configuring IO Ports
It is very simple to configure a GPIO port as an input or output using MCC. Simply, open the MCC. Click on the GPIO and configure it as input or output. In this case we are configuring RB1 as an output. Note that everytime we make a change to the MCC we need to click on GENERATE button next to the Project Resources to update the MCC code. 

![Step 8](images/step8e.png)

 Note that as you configure different GPIO ports the pins are updated on the chip: 

 ![Step 8b](images/step8b.png) 

# Step 10. Changing the MAIN.C
We can now go back to the main.c program and add a little more code. The figure below shows the modified main.c. Note that we are using RB1 as an output and added a new RANDOM Function generator. 
 
 ![Step 10](images/step10.png) 

 # Step 11. The New Output
Once again, let's check the new output using the Serial Terminal. We can also look at the output of the RB1 using a scope. 

 ![Step 11](images/step11.png) 

