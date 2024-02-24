# About PIC 18F4xK42 Family
In this document we provide various related links and documents about the chip and the evaluation modules.

We are currently using PIC18F47K42 CURIOSITY NANO EVALUATION KIT. 
- [Kit Spec](https://ww1.microchip.com/downloads/aemDocuments/documents/MCU08/ProductDocuments/UserGuides/50002899B.pdf)
- [Kit Schematics](https://ww1.microchip.com/downloads/en/DeviceDoc/PIC18F47K42_Curiosity_Nano_Schematics.pdf)
- [PIC18F4xK42 Spec](https://ww1.microchip.com/downloads/aemDocuments/documents/MCU08/ProductDocuments/DataSheets/PIC18%28L%29F26-27-45-46-47-55-56-57K42-Data-Sheet-40001919G.pdf) - everythign you need to know about the chip.
- [List of PIC devices](https://www.northernsoftware.com/dev/pic18f/pic18f47k42.htm) - Great recourse to quickly learn about different PIC devices
- 

## Using MPLAB X 
There are many resources to learn about MPLAB IDE. 
- Installing MPLAB X [See the lab instructions](https://docs.google.com/presentation/d/1CH_h3xdj5d8A5pSpMd1fWH8EFt2mmj-g/edit?usp=sharing&ouid=116265364704461279990&rtpof=true&sd=true )

## Programming PIC 18F4xK42 
We can program the evaluation board several ways: 
- Using the MPLAB X IDE we can compile the code and then place the HEX file on the evaluation module. The module acts as a storage device and we can download the HEX file in there. Once the HEX file is placed in the module it will be stored in the non-volatile programing memory.
- It is possible to use MPLAB [Online XPRESS IDE](https://www.mplab-xpresside.microchip.com/dashboard/) to compile the code and then download the HEX file into the module. Note that the Online XPRESS IDE does NOT support ASSEMBLY instructions.

## Books on PIC 
There used to be alot more books on PIC microcontrollers, before ARDUINO and RPIs become so popular. Here is a short list of some of the good books on PIC microcontrollers: 
- [PIC Programming in Assembly](https://groups.csail.mit.edu/lbr/stack/pic/pic-prog-assembly.pdf)
- [PIC Microcontroller and Embedded Systems: Using Assembly and C for PIC18; by Mazidi](https://www.amazon.com/PIC-Microcontroller-Embedded-Systems-Assembly/dp/099792599X/ref=pd_lpo_1?pd_rd_w=bm37p&content-id=amzn1.sym.116f529c-aa4d-4763-b2b6-4d614ec7dc00&pf_rd_p=116f529c-aa4d-4763-b2b6-4d614ec7dc00&pf_rd_r=02WXRDTRHFZJWT4FD6KQ&pd_rd_wg=4yCHv&pd_rd_r=e152fbf2-7bbf-4816-b970-8d3954e01b8e&pd_rd_i=099792599X&psc=1)

## PIC Projects
There are many many resources available to ideas about cool projects. Here are just a few: 
- [Electronic Wings](https://www.electronicwings.com/pic/pic18f4550-watchdog-timer) - Example codes on his page are primarily for pic18f4550. However, with a little modifications you can change the code to other PIC families. 
  

## Figures
For more informatin please see the [course slides](https://docs.google.com/presentation/d/14XZqJDr-a5fgImAD0o6PCsv6aOMM7zV3/edit?usp=sharing&ouid=111229422470150013614&rtpof=true&sd=true )

This the pinout of a typical 40 pin PIC18F4xK42:

![image](https://user-images.githubusercontent.com/18106204/233185366-c2ea0ea0-5d45-4358-be24-da013fd0f2f7.png)

Evaluation module:

<img width="772" alt="image" src="https://github.com/faridfarahmand/SSU/assets/18106204/e7c22a89-f7ff-46d2-98c2-abe5bd8ac437">

____________
I use this site to edit this document:https://pandao.github.io/editor.md/en.html 
