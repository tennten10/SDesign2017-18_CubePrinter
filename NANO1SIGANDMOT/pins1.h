//Declare the function of each arduino pin and what it is connected to. Maybe also include colors if we are going to color code the physical ones for ease of assembly. 
//
//Arduino Nano pins for Motors and Sensors
//
#define example1     1  // 
#define PGOOD32V     2  //  if signal low, power is bad. Attach interupt to handle this
#define ALTSIG       3  // Altimeter signal. Fires 9V. Will want a resistor to drop to 5V and diode to have "check valve" for two input times
#define ONOFF32V     4  // turns 32V boost on/off. Set low to turn on.  
#define SENS5        5  // 
#define SENS4        6  // 
#define SENS3        7  // Photosensor for testing LED
#define SENS2        8  // start 
#define SENS1        9  // 
#define SEQ_LAYER   10  // Reads if next layer exists. High if yes, low if no. 
#define LEDSIG      11  // PWM to LED driver to drive brightness and on/off
#define SEQ_FDBK    12  // Sequene feedback. Listen for low to know LCD changed successfully. 
#define SEQ_CNTRL   13  // Sequence control. Set high to change LCD image. 

#define ERRORSIG    A0  // Signal to LED for calling out errors. 
#define exampleA1   A1
#define PGOOD5V     A2  // Should this have a pull up?
#define exampleA3   A3
#define STEP        A4  // Motor Step pin
#define DIR         A5  // Motor Direction pin
