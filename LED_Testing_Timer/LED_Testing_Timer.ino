#include <SevSeg.h>
#include <math.h>

//Pins for testing using an Arduino Mega
//Pins for 7-Segment Display
#define A  53
#define B  51
#define C  49
#define D  47
#define E  45
#define F  43
#define G 41
#define DP 39
#define One 52
#define Two 50
#define Three 48

//Pins for other functions
#define Pot A3
#define Light 11
#define Switch 3 

//Initializations
SevSeg timeDisplay;
int duration = 500; //light duration in milliseconds
volatile boolean flag = false; //Interrupt signal to run light routine

void setup() {
  // put your setup code here, to run once:
   pinMode(A, OUTPUT);
   pinMode(B, OUTPUT);
   pinMode(C, OUTPUT);
   pinMode(D, OUTPUT);
   pinMode(E, OUTPUT);
   pinMode(F, OUTPUT);
   pinMode(G, OUTPUT);
   pinMode(DP, OUTPUT);
   pinMode(One, OUTPUT);
   pinMode(Two, OUTPUT);
   pinMode(Three, OUTPUT);
   pinMode(Pot, INPUT); //analog
   pinMode(Light, OUTPUT);
   pinMode(Switch, INPUT_PULLUP);
   digitalWrite(Switch, HIGH); //enables pull-up resistor
   attachInterrupt(digitalPinToInterrupt(Switch), ISR1, FALLING);

   byte numDigits = 3;
   byte digitPins[] = {One,Two,Three};
   byte segmentPins[] = {A,B,C,D,E,F,G,DP};
   timeDisplay.begin(COMMON_CATHODE, numDigits, digitPins, segmentPins);
   timeDisplay.setBrightness(30);
}

void loop() {
  PotRead(); //function that reads the potentiometer voltage value and converts it into a time. Also sets the display value.
  timeDisplay.refreshDisplay();
  if (flag == true)
  {
    Flash(); //function that turns on the light for the set amount of time.
  }
}

void PotRead(){
  int potVoltage = analogRead(Pot); //0->5V mapped to 0 ->1023
  //remap 0-1023 to time intervals 250ms ->5000s
  duration = (int)potVoltage * 4750.0/1023.0 + 250.0;
  if (duration >= 1000){
    timeDisplay.setNumber(duration/10, 2);
  }else {
      timeDisplay.setNumber(duration, 0);
  }
}

void Flash()
{
  digitalWrite(Light, HIGH);
  delay(duration);
  digitalWrite(Light,LOW);
  flag = false;
}

void ISR1(){
  flag = true;
}
  
