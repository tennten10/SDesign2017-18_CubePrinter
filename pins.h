//Declare the function of each arduino pin and what it is connected to. Maybe also include colors if we are going to color code the physical ones for ease of assembly. 

//Arduino Uno pins for LCD
#define example 1
#define example 2
#define st1_1 3 // Stepper motor 1 control pin 1
#define SDCS 4 //microSD chip select pin (for arduino to tell sd it wants to send/receive data)
#define st1_2 5 // Stepper motor 1 control pin 2
#define st2_1 6 // Stepper motor 2 control pin 1
#define st2_2 7 // Stepper motor 2 control pin 2
#define example 8 //for touch sensor. Won't use. 
#define DC 9 // TFT data/command select pin (for arduino to tell tft it wants to send data)
#define CS 10 // TFT chip select (for arduino to tell tft to send/receive data from tft)
#define MOSI 11 // SPI master-out-slave-in pin
#define MISO 12 // SPI master-in-slave-out pin
#define SCLK 13 // SPI data clock pin

#define LED A0 // to led driver pwm for brightness
#define example A1
#define example A2
#define example A3
#define example A4
#define example A5


//source: https://cdn-learn.adafruit.com/downloads/pdf/adafruit-2-8-tft-touch-shield-v2.pdf
