#include <Libraries/UTFT/UTFT.h>
#include <Libraries/stepper/stepper.h>
//#include <StartStop.h>
#include <pins.h>

  Stepper st1 = Stepper(200, st1_1, st1_2)
  Stepper st2 = Stepper(200, st2_1, st2_2)
  
  UTFT lcd = UTFT( TFT01-2.4SP, MOSI, SCLK, CS, RST) // Reset pin not called out on documentation I got. Maybe what comes with the screen will have more details. 
  // The lcd screen we ordered might come with its own library. We can deal with that when the time comes. 
    
  
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
  lcd.InitLCD();
  stepmm(-20); //just making sure they work
  stepmm(20);
    
  pinmode(LED, OUTPUT);  
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
 while(photoarray.hasNext())
  
   //load photo onto LCD
  analogWrite(LED, 200); //pwm output for led chip. (pin, dutyCycle) between 0-255
  delayMicroseconds(100);
  analogWrite(LED, 0);
  stepmm(0.1);
  
}


// this function translates distance y in milimeters into steps for the motors and moves them each at the same time
//if we wire them in parallel we would only need one driver and they would move simultaneously, but then any leveling control and manual adjustments would be hard to correct once assembled. 
void stepmm(double y)
{
  double z = y*1;// 200 steps/rev. for stepper. ______ revs/cm for threaded rod pitch.
  st1.step(z);
  st2.step(z);
}
