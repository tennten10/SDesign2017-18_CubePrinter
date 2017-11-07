// Constants will be stored in this file. In all code, use the names rather than the values to make changing them easier. 
#define SCREEN_X 320 //Pixel dimensions of the LCD Screen
#define SCREEN_Y 240

#define BASE_LED_TIME 1000 //Beginning LED time on, in milliseconds
#define LAYER_LED_TIME 500 //layer LED time on
#define LED_INTENSITY 200 //For PWM input to LED driver chip. Values between 0-255. Check references to make sure pulses are readable from chip and confirm in notes here. 

#define STEPS_REV 200 //change to 400, 800, 1600, etc if using 1/2, 1/4, 1/8 microstepping
#define MOTOR_SPEED 50 //Speed in rpm
