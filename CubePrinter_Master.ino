// LCD function modified from Demo_Landscape example on RinkyDinkElectronics.com

#include <UTFT.h> // make sure the libraries are installed on the computer that the code is being run on. 
#include <tinyFAT.h>
#include <UTFT_tinyFAT.h>
#include <stepper.h>
#include "Config_Parameters.h" //input constants into the system
#include <pins.h>

  Stepper st1 = Stepper(STEPS_REV, st1_1, st1_2)
  Stepper st2 = Stepper(STEPS_REV, st2_1, st2_2)
  
  
///////////////////  Initial Screen & SD declarations
extern uint8_t SmallFont[];

UTFT LCD = UTFT(TFT01_70, 38, 39, 40, 41); //will need to change pins for our specific LCD. Look in UTFT documentation for correct numbers. 
UTFT_tinyFAT myFiles(&LCD); //loads the images from the SD card

char* layerIMG[] = {"example1.RAW", "example2.RAW", "example3.RAW"}; //include names of all layers of the print that are stored on the SD card 
boolean display_rendertime = false;
boolean display_filename = false;

word res;
long sm, em;  
 /////////////////////////////

void setup(){
//code here will run once at start
//start routine pseudocode
/*  on power up 
    initialize all parts
    test all parts to make sure they are working
      return 0 if not running, 1 if running
        if any part has a 0, blink an error light. Run again. IF same result, do again. IF fails again, stop operation. 
  move print bed to launch position
  turn off UV leds
  turn off camera
 */ 
  LEDSetup();
  LCDSetup();
  MotorSetup();
     
}

void loop(){
//code here will run repeatedly
/*  attach interupt for pre-launch signal
    turn on camera a little before launch. begin print routine (before/at/after) launch
      - move print bed to begining position
      loop: - change LCD image, turn on UV
            - Wait curing time, turn off UV, Black out LCD, adjust print bed
      - once all layers are printed, move bed to finished position
      - keep camera running but put all else on standby 
 */     
 \\pseudocode beneath this line. Will not actually run. 
 /*while(photoarray.hasNext())
  
   //load photo onto LCD
  ONled(LAYER_LED_TIME);
  stepmm(0.1);
  */
   //////////Below loops through photos stored on an SD card and holds them on the screen for X time 
   for (int i=0; i<(sizeof(layerIMG)/sizeof(*layerIMG)); i++)
  {
    if (layerIMG[i]!="")
    {
       
      sm=millis();
      res=myFiles.loadBitmap(0, 0, SCREEN_X, SCREEN_Y, layerIMG[i]); //displays the image
      em=millis();
      //Error handling loop if files not there.
      if (res!=0)
      {
        if (res==0x10)
        {
          LCD.print("File not found...", 0, 0);
          LCD.print(layerIMG[i], 0, 14);
        }
        else
        {
          LCD.print("ERROR: ", 0, 0);
          LCD.printNumI(res, 56, 0);
        }
        delay(3000);
        LCD.clrScr();
      }
      else
      {
        /*if (display_rendertime==true)
        {
          LCD.print("Rendertime (secs):", 0, 0);
          LCD.printNumF(float((em-sm)/1000.0), 2, 160,0);
        }
        if (display_filename==true)
        {
          myGLCD.print(files[i], CENTER, myGLCD.getDisplayYSize()-12);
        }
        delay(3000);*/
      }
    }
  }
  ///////////////////////////////////////
}


// Will run only once in the setup part of the code. Sets initial settings for LCD. 
void LCDSetup(){
  LCD.InitLCD();
  LCD.clrScr();
  file.initFAT();
  LCD.setColor(255,255,255);
  LCD.setFont(SmallFont);
}
void MotorSetup(){
  st1.setSpeed(MOTOR_SPEED);
  st2.setSpeed(MOTOR_SPEED);
  // Find a way to home and level the print bed and move to starting/launch position
}
void LEDSetup(){
  pinMode(LEDpin, OUTPUT);
  analogWrite(LEDpin,0); //makes sure it is off initially
  //Not sure what else to do?
}


void ONled(int time){ // Input is the time in milliseconds for LED to be on. 
  analogWrite(LEDpin, LED_INTENSITY); //pwm output for led chip. (pin, dutyCycle) between 0-255
  delayMicroseconds(time);
  analogWrite(LEDpin, 0);
}



// this function translates distance y in milimeters into steps for the motors and moves them each at the same time
//if we wire them in parallel we would only need one driver and they would move simultaneously, but then any leveling control and manual adjustments would be hard to correct once assembled. 
void stepmm(double y)
{
  double z = y*1;// 200 steps/rev. for stepper. ______ revs/cm for threaded rod pitch.
  st1.step(z);
  st2.step(z);
}
