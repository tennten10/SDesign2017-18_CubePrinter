//Declare the function of each arduino pin and what it is connected to. Maybe also include colors if we are going to color code the physical ones for ease of assembly. 

//Arduino Uno pins for LCD
#define st1_1 1 // Stepper motor 1 control pin 1
#define example 2 //interrupt pin. Reserve for sensor io. 
#define example 3 //interrupt pin. Reserve for sensor io. 
#define SDCS 4 //microSD chip select pin (for arduino to tell sd it wants to send/receive data)
#define st1_2 5 // Stepper motor 1 control pin 2
#define st2_1 6 // Stepper motor 2 control pin 1
#define st2_2 7 // Stepper motor 2 control pin 2
#define lcdRST 8 //Not actually reset. for touch sensor. Won't use. 
#define lcdDC 9 // TFT data/command select pin (for arduino to tell tft it wants to send data)
#define lcdCS 10 // TFT chip select (for arduino to tell tft to send/receive data from tft)
#define lcdMOSI 11 // SPI master-out-slave-in pin
#define lcdMISO 12 // SPI master-in-slave-out pin
#define SCLK 13 // SPI data clock pin

#define LED A0 // to led driver pwm for brightness
#define example A1
#define example A2
#define example A3
#define example A4
#define Err A5 // error light to flash if error occurs


//source: https://cdn-learn.adafruit.com/downloads/pdf/adafruit-2-8-tft-touch-shield-v2.pdf
