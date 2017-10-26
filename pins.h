//Declare the function of each arduino pin and what it is connected to. Maybe also include colors if we are going to color code the physical ones for ease of assembly. 

//Arduino Uno pins for LCD
#define example 1
#define example 2
#define example 3
#define SDCS 4 //microSD chip select pin (for arduino to tell sd it wants to send/receive data)
#define example 5
#define example 6
#define example 7
#define example 8 //for touch sensor. Won't use. 
#define DC 9 // TFT data/command select pin (for arduino to tell tft it wants to send data)
#define CS 10 // TFT chip select (for arduino to tell tft to send/receive data from tft)
#define MOSI 11 // SPI master-out-slave-in pin
#define MISO 12 // SPI master-in-slave-out pin
#define SCLK 13 // SPI data clock pin
#define example 14

//source: https://cdn-learn.adafruit.com/downloads/pdf/adafruit-2-8-tft-touch-shield-v2.pdf
