//Declare the function of each arduino pin and what it is connected to. Maybe also include colors if we are going to color code the physical ones for ease of assembly. 

//Arduino Uno pins for LCD. Translate to Nano
#define example   1 //
#define SEQ_CNTRL 2 //interrupt pin. Sequence control. Listens for input from other Arduino. On Rise, increment image.   
#define example   3 //interrupt pin. Reserve for sensor io. 
#define SDCS      4 //microSD chip select pin (for arduino to tell sd it wants to send/receive data)
#define example   5 // 
#define SEQ_FDBK  6 // Confirms data with other Arduino. goes low when it is ready for LED to go
#define SEQ_LAYER 7 // Confirms that next layer exists. HIGH as long as it does. When low, it signals beginning of shut down routine. 
#define lcdRST    8 // Not actually reset. for touch sensor. Won't use. 
#define lcdDC     9 // TFT data/command select pin (for arduino to tell tft it wants to send data)
#define lcdCS    10 // TFT chip select (for arduino to tell tft to send/receive data from tft)
#define lcdMOSI  11 // SPI master-out-slave-in pin
#define lcdMISO  12 // SPI master-in-slave-out pin
#define SCLK     13 // SPI data clock pin

#define example  A0 // 
#define example  A1 //
#define example  A2 //
#define example  A3 //
#define example  A4 //
#define Err      A5 //error light to flash if error occurs


//source: https://cdn-learn.adafruit.com/downloads/pdf/adafruit-2-8-tft-touch-shield-v2.pdf
