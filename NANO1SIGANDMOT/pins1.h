//Declare the function of each arduino pin and what it is connected to. Maybe also include colors if we are going to color code the physical ones for ease of assembly. 

//Arduino Nano pins for Motors and Sensors
#define example 1  // 
#define PGOOD32V 2  //  if signal low, power is bad. Attach interupt to handle this
#define ALTSIG  3  // Altimeter signal. Fires 9V. Will want a resistor to drop to 5V and diode to have "check valve" for two input times?
#define ONOFF32V 4  // turns 32V boost on/off. Set low to turn on.  
#define SENS1 5  // 
#define SENS2 6  // 
#define SENS3 7  // 
#define SENS4 8  //  
#define SENS5 9  // 
#define SENS6 10 // 
#define LEDSIG 11 // PWM to LED driver to drive brightness and on/off
#define SEQ_FDBK 12 // Sequene feedback. Listen for low to know LCD changed successfully. 
#define SEQ_CNTRL 13 // Sequence control. Set high to change LCD image. Listen for 

#define ERRORSIG A0 // Signal to LED for calling out errors. 
#define example A1
#define example A2
#define example A3
#define STEP1 A4 // Motor Step pin
#define DIR1 A5  //Motor Direction pin
