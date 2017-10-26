#include <Libraries/UTFT/UTFT.h>
#include <StartStop.h>
#include <pins.h>

void setup(){
//code here will run once at start
//start routine pseudocode
  on power up 
    initialize all parts
    test all parts to make sure they are working
      return 0 if not running, 1 if running
        if any part has a 0, blink an error light. Run again. IF same result, do again. IF fails again, stop operation. 
  move print bed to launch position
  turn off UV leds
  turn off camera
  
          
}

void loop(){
//code here will run repeatedly
  attach interupt for pre-launch signal
    turn on camera a little before launch. begin print routine (before/at/after) launch
      - move print bed to begining position
      loop: - change LCD image, turn on UV
            - Wait curing time, turn off UV, Black out LCD, adjust print bed
      - once all layers are printed, move bed to finished position
      - keep camera running but put all else on standby 
      
    
}
