//Nano#1 Motors, Sensors
#include "pins1.h"
#include "Config_Parameters.h"
#include <Stepper.h>


//PGOOD32V, with pull up. If signal low, power is bad. 
//ONOFF32V, turn signal low to turn on, Vin to turn off. Uses pull up resistor, and NPN transistor to set low. 
//Interrupt Variables
volatile bool Int1 = false;
volatile bool Int2 = false;
volatile int done = false;

Stepper st1 = Stepper(STEPS_REV, STEP1, DIR1);


void setup() {
  // put your setup code here, to run once:
  Serial.begin(38400);
  attachInterrupt(digitalPinToInterrupt(2),ISR_1,RISING); // PGOOD32V
  attachInterrupt(digitalPinToInterrupt(3),ISR_2,RISING);  // ALTSIG
  st1.setSpeed(MOTOR_SPEED);
  
  
  StartUp();
  //Standby();
}

void loop() {
  // put your main code here, to run repeatedly:
  Status("S"); //Prints pin status to Serial monitor for debugging.  
  if (Int2 == true && done == 0){ //begin launch sequence one
    //initialize all parts
    digitalWrite(ONOFF32V, LOW); //turns on 32V power
    while( digitalRead(PGOOD32V)!= LOW ){ //Lets 32V power boot up
      delay(100);
      }
    LED(0);
       
    done = 1;
    Int2 = false;
    }

  if(Int2 == true && done == 1){ //begin printing sequence
    //actually begins printing
      Print();
      Standby();
      done = 2;
    }
}

//************************************************************************//
void Status(String start){ //use start to denote which section of the code is running. P for printing, S for startup, etc. 
  String val = start + "";
  for (int i = 1; i++; i<11){ //reading digital pins
    val = val + " " + digitalRead(i);
  }//skips 11 bc reading pwm led pin stops the pwm output
  val += " " + digitalRead(12);
  val += " " + digitalRead(13);
  val += " " + digitalRead(A0);
  val += " " + digitalRead(A1);
  val += " " + digitalRead(A2);
  val += " " + digitalRead(A3);
  val += " " + digitalRead(A4);
  val += " " + digitalRead(A5);
  
  Serial.println(val);
  }
//************************************************************************//
void StartUp(){
  //check power
  digitalWrite(ONOFF32V, LOW);
  delay(500);
  while(digitalRead(PGOOD32V) == LOW){
    //turn error LED 200ms
    //turn error LED off 200ms 
    }
  
  //check LED
  //**************turn LCD all black. ******************
  //flash LED quickly. Should visually see it flash. 
  LED(500);
  delay(1000);
  digitalWrite(ONOFF32V, HIGH); //turns of 32V power supply since LED works. 
     
  //check motors. check position and move to start
  if(digitalRead(SENS1) != HIGH){ //using pull up resistor
    do{
      MOVE(0.1);
    }while(digitalRead(SENS1 != HIGH));
    }
    else{
      MOVE(-2000);
      delay(1000);
      do{
        MOVE(0.1);
      }while(digitalRead(SENS1 != HIGH));
    }
  
  //tell other Arduino to check LCD, SD card
  
  } //close to done, except for anything communicating with other LCD
//************************************************************************//
void ISR_1(){ //do not use delay or millis inside ISRs
  while(digitalRead(PGOOD32V) != HIGH){
    delayMicroseconds(200);
  }
  //Int1 = true;
  } //Pgood. Wait and check to see if power is restored before beginning printing again.
//************************************************************************//
void ISR_2(){
  Int2 = true;
  } //Apogee signal. Done. 
//************************************************************************//
void Print(){
  //check position sensor. If not in position, move until it is. 
  if(digitalRead(SENS1) != LOW){
    do{
        MOVE(0.1);
      }while(digitalRead(SENS1 != HIGH));
    }
  //load image on LCD
  digitalWrite(SEQ_CNTRL, HIGH);
  //wait for confirmation that it is loaded
  do{
    delayMicroseconds(50);
  }while(digitalRead(SEQ_FDBK)!= LOW);
  digitalWrite(SEQ_CNTRL, LOW);
  //set printbed for base layer. 
  MOVE(BASE_LAYER_HEIGHT); //be careful about direction. Test when set up ****
  LED(BASE_LED_TIME);
  MOVE(LAYER_HEIGHT);
  //Tell it to change image 
  //putting this inside loop
  do{
    
    digitalWrite(SEQ_CNTRL, HIGH); //loads next image
    do{
      delayMicroseconds(75); //delays so Nano2 has time to change SEQ_FDBK to HIGH in response to CNTRL
    }while(digitalRead(SEQ_FDBK)!= LOW); //confirmation that it is loaded
    digitalWrite(SEQ_CNTRL, LOW); //resets interupt control pin
    LED(BASE_LED_TIME);
    MOVE(LAYER_HEIGHT);    
  }while(digitalRead(SEQ_LAYER) == LOW); //next image exists
  
}

//************************************************************************//
void Standby(){} //set parameters to power saving mode. 

//************************************************************************//
void LED(int a){
  long t = millis();
  analogWrite(LEDSIG, LED_INTENSITY);
  //do{
  //}while(millis()<(t+a));
  delayMicroseconds(a);
  analogWrite(LEDSIG,0);
  } //done.
//************************************************************************//
void MOVE(float dist){ //move print bed input distance "dist" in um. Negative numbers work.
  //thread pitch is 2mm
  //200 steps/rev --> 2mm/rev --> 100 steps/ 1mm = 1000um --> 1 step = 0.1um
  int dist1 = dist/0.1;
  st1.step(dist1);
}
//************************************************************************//
void FinalStandby(){}
