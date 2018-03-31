#include <Azande.h> //https://zeijlonsystems.se/products/azande/download/arduinolib/download.html
#include "ERRORS.h"
#include <Stepper.h>
#include "pins1.h"
#include "Config_Parameters.h"


/*********Begin Global Variables and Object Initializations*********/

volatile bool Int2 = false;
volatile int done = 0;

bool LCD_flag = false;
bool SD_flag = false;
bool Comm_flag = false;
bool p5V_flag = false;
bool p32V_flag = false;
bool LED_flag = false;
bool Altimeter_flag = false;
bool Motor_flag = false;
bool Begin_flag = false;
bool Position_flag = false;
bool Startup_check_flag = false;
bool Azande_flag = true;
ERRORS e;

// Azande Feature Macros

define_text_event(current_system, "Testing: ", , 64);
define_text_event(test_status, "Status", , 64);
define_text_event(user_instructions, "Instructions", 2, 64);
define_void_command(retest, "Reset and test again", AzandeStartupReset, );
define_bool_event(comm_disp, "Comm test status", 3, "Pass", "Fail");
define_bool_event(SD_disp, "SD test status", 4, "Pass", "Fail");
define_bool_event(LCD_disp, "LCD test status", 5, "Pass", "Fail");
define_bool_event(p5V_disp, "p5V test status", 6, "Pass", "Fail");
define_bool_event(p32V_disp, "p32V test status", 7, "Pass", "Fail");
define_bool_event(LED_disp, "LED test status", 8, "Pass", "Fail");
define_bool_event(ALT_disp, "Altimeter test status", 9, "Pass", "Fail");
define_bool_event(MOT_disp, "Motors test status", 10, "Pass", "Fail");
define_bool_event(POS_disp, "Position test status", 11, "Pass", "Fail");
define_bool_event(BEG_disp, "Begin test status", 12, "Pass", "Fail");
define_bool_event(START_disp, "Startup test status", , "Pass", "Fail");
define_void_command(finish, "Testing Successful. Close and prime for launch. This will put the printer to sleep for 5 minutes and then listen for the Begin sensor, followed by the altimeter.", CloseAzandeAndPrime, );
// Variables
long value;               // The VALUE that is used for all calculations.
Azande azande;            // The Azande object variable.
Stepper st1 = Stepper(STEPS_REV, STEP, DIR);
long prevTime = 0;
bool r = HIGH;
bool StartSwitch = false;
/**********End Global Variables and Object Initializations**********/
/********************Begin Arduino Main Loops***********************/
void setup()
{
	// Setup Variables.

	azande.begin(19200);  // Init Azande with baudrate 19200

	// Add features to be visible on Azande Studio.

	azande.add(current_system);
	azande.add(test_status);
	azande.add(user_instructions);
	azande.add(retest);
	azande.add(comm_disp);
	azande.add(SD_disp);
	azande.add(LCD_disp);
	azande.add(p5V_disp);
	azande.add(p32V_disp);
	azande.add(LED_disp);
	azande.add(ALT_disp);
	azande.add(MOT_disp);
	azande.add(POS_disp);
	azande.add(BEG_disp);
	azande.add(START_disp);


	//
	StartUp();
	SystemCheck(); //Still need to code MotorCheck() and PositionCheck() 
}

void loop()
{
	//flags to run azande loop while its waiting for the primed status
	//flag to run printing loop 
	if (Azande_flag){
		delay(20);
		azande.serialEvent();
		if (Startup_check_flag && done==0){
			azande.add(finish);
			azande.notify();
			done = 1;
		}
	}
	else if (Int2 == true && done == 0){ //checks for accelerometer signal
		//Boots everything back up to be ready for printing
		Int2 = false;
		Reboot();
	}
	else if (Int2 == true && done == 1){ //checks for appogee signal
		//Actually does the printing
		Print();
		Standby();
		done = 2;
	}
	else if(done==2){
		//blink to show that its still alive.
		if (millis() - prevTime > 3000){
			r = !r;
			digitalWrite(ERRORSIG, r);
			prevTime = millis();
		}
	}
	else{
		while (!StartSwitch){
			StartSwitch = !digitalRead(SENS2);
		}
	}
}
/*********************End Arduino Main Loops************************/
/***********************Begin GUI methods***************************/
void StartUp(){ //Recheck these pull ups and initializations with everyone
	pinMode(PGOOD32V, INPUT); //is pulled up externally
	pinMode(ALTSIG, INPUT);
	pinMode(ONOFF32V, OUTPUT);
	pinMode(SENS5, INPUT_PULLUP);
	pinMode(SENS4, INPUT_PULLUP);
	pinMode(SENS3, INPUT_PULLUP);
	pinMode(SENS2, INPUT_PULLUP);
	pinMode(SENS1, INPUT_PULLUP);
	pinMode(SEQ_LAYER, INPUT);
	pinMode(LEDSIG, OUTPUT);
	pinMode(SEQ_FDBK, INPUT);
	pinMode(SEQ_CNTRL, OUTPUT);
	pinMode(ERRORSIG, OUTPUT);
	pinMode(PGOOD5V, INPUT);
	pinMode(STEP, OUTPUT);
	pinMode(DIR, OUTPUT);
	
	digitalWrite(PGOOD32V, LOW);
	digitalWrite(ALTSIG, LOW);
	digitalWrite(ONOFF32V, HIGH);
	digitalWrite(SENS5, HIGH);
	digitalWrite(SENS4, HIGH);
	digitalWrite(SENS3, HIGH);
	digitalWrite(SENS2, HIGH);
	digitalWrite(SENS1, HIGH);
	digitalWrite(SEQ_LAYER, LOW);
	digitalWrite(LEDSIG, LOW);
	digitalWrite(SEQ_FDBK, LOW);
	digitalWrite(SEQ_CNTRL, LOW);
	digitalWrite(ERRORSIG, LOW);
	digitalWrite(PGOOD5V, HIGH);
	digitalWrite(STEP, LOW);
	digitalWrite(DIR, LOW);

	attachInterrupt(digitalPinToInterrupt(PGOOD32V), ISR_1, FALLING);
	attachInterrupt(digitalPinToInterrupt(ALTSIG), ISR_2, RISING);
}
/*******************************************************************/
void SystemCheck(){
	Startup_check_flag = true; //if any below fail, they will turn it false
	
	//azande.send(comm_disp, Comm_flag);
	CommCheck();
	SDCheck();
	LCDCheck();
	p5VCheck();
	p32VCheck();
	LEDCheck();
	AltimeterCheck();
	MotorCheck();
	PosCheck();
	BeginCheck(); 

	azande.send(START_disp, Startup_check_flag);
}
/*******************************************************************/
void CommCheck(){
	azande.send(current_system, "CommCheck");
	delay(10);
	azande.serialEvent();
	//Pulse a random number, and have the other Arduino repeat back that same number to confirm connection for each communication channel
	srand(millis());
	int num = rand() % 20 + 1; 
	bool status, prevStatus;
	int count = 0;
	int err[3] = { 0, 0, 0 };
	//First pair: Testing SEQ_CNTRL as output and SEQ_FDBK as input
	pinMode(SEQ_CNTRL, OUTPUT);
	pinMode(SEQ_FDBK, INPUT);
	for (int i = 0; i < num; i++){
		digitalWrite(SEQ_CNTRL, LOW);
		delay(50);
		digitalWrite(SEQ_CNTRL, HIGH);
		delay(50);
	}
	prevTime = millis();
	do{
		status = digitalRead(SEQ_FDBK);
		if (status != prevStatus && status == LOW){
			count++;
			prevStatus = status;
			prevTime = millis();
		}
	} while ((millis() - prevTime) < 500 );
	if (count != num){
		err[0] = 1;
	}
	//Second pair: Testing SEQ_FDBK as output and SEQ_LAYER as input
	pinMode(SEQ_FDBK, OUTPUT);
	pinMode(SEQ_LAYER, INPUT);	//Pulse a random number, and have the other Arduino repeat back that same number to confirm connection for each communication channel
	num = rand() % 20 + 1;
	count = 0;
	for (int i = 0; i < num; i++){
		digitalWrite(SEQ_FDBK, LOW);
		delay(50);
		digitalWrite(SEQ_FDBK, HIGH);
		delay(50);
	}
	prevTime = millis();
	do{
		status = digitalRead(SEQ_LAYER);
		if (status != prevStatus && status == LOW){
			count++;
			prevStatus = status;
			prevTime = millis();
		}
	} while ((millis() - prevTime) < 500);
	if (count != num){
		err[1] =1; 
	}
	//Third pair: Testing SEQ_LAYER as output and SEQ_CNTRL as input
	pinMode(SEQ_LAYER, OUTPUT);
	pinMode(SEQ_CNTRL, INPUT);
	num = rand() % 20 + 1;
	count = 0;
	prevTime = millis();
	for (int i = 0; i < num; i++){
		digitalWrite(SEQ_LAYER, LOW);
		delay(50);
		digitalWrite(SEQ_LAYER, HIGH);
		delay(50);
	}
	prevTime = millis();
	do{
		status = digitalRead(SEQ_CNTRL);
		if (status != prevStatus && status == LOW){
			count++;
			prevStatus = status;
			prevTime = millis();
		}
	} while ((millis() - prevTime) < 500);
	if (count != num){
		err[2] = 1;
	}

	pinMode(SEQ_CNTRL, OUTPUT);
	pinMode(SEQ_FDBK, INPUT);
	pinMode(SEQ_LAYER, INPUT);

	Comm_flag = ErrorCheck(eCOM, err);
	azande.send(comm_disp, Comm_flag);
	azande.serialEvent();
}
void SDCheck(){
	azande.send(current_system, "SDCheck");
	delay(10);
	azande.serialEvent();

	digitalWrite(SEQ_CNTRL, LOW);
	bool status, prevStatus;
	int count = 0;
	prevTime = millis();
	do{
		status = digitalRead(SEQ_FDBK);
		if (status != prevStatus && status == LOW){
			count++;
			prevStatus = status;
			prevTime = millis();
		}
		if (millis() - prevTime > 2000)
			break;
	} while ((millis() - prevTime) < 500 || (count < 1 ));
	delay(100);
	digitalWrite(SEQ_CNTRL, HIGH);

	SD_flag = ErrorCheck(eSD, count);
	azande.send(SD_disp, SD_flag);
	azande.serialEvent();
}
void LCDCheck(){
	azande.send(current_system, "LCDCheck");
	delay(10);
	azande.serialEvent();

	digitalWrite(SEQ_CNTRL, LOW);
	bool status, prevStatus;
	int count = 0;
	prevTime = millis();
	do{
		status = digitalRead(SEQ_FDBK);
		if (status != prevStatus && status == LOW){
			count++;
			prevStatus = status;
			prevTime = millis();
		}
		if (millis() - prevTime > 2000)
			break;
	} while ((millis() - prevTime) < 500 || (count < 1));
	delay(100);
	digitalWrite(SEQ_CNTRL, HIGH);

	LCD_flag = ErrorCheck(eLCD, count);
	azande.send(LCD_disp, LCD_flag);
	azande.serialEvent();
}
void p5VCheck(){
	azande.send(current_system, "p5VCheck");
	delay(10);
	azande.serialEvent();
	int read = digitalRead(PGOOD5V);
	p5V_flag = ErrorCheck(e5V, read);
	azande.send(p5V_disp, p5V_flag);
	azande.serialEvent();
}
void p32VCheck(){
	azande.send(current_system, "p32VCheck");
	delay(10);
	azande.serialEvent();
	int read = 1;
	prevTime = millis();
	digitalWrite(ONOFF32V, LOW); //low sets 32V power on and leave on.
	while (!digitalRead(PGOOD32V)){
		if (millis() - prevTime > 5000){
			read = 0;
			break;
		}
	}
	p32V_flag = ErrorCheck(e32V, read);
	azande.send(p32V_disp, p32V_flag);
	azande.serialEvent();
}
void LEDCheck(){ //revisit this check method
	azande.send(current_system, "LEDCheck");
	delay(10);
	azande.serialEvent();
	long t = millis();
	int read;
	analogWrite(LEDSIG, LED_INTENSITY);
	delay(100);
	read = digitalRead(SENS3); // assuming a photosensor can be placed on the tank to capture the light data. Use the right voltage divider to sense HIGH or LOW. 
	delay(100);
	analogWrite(LEDSIG, 0);

	LED_flag = ErrorCheck(eLED, read);
	azande.send(LED_disp, LED_flag);
	azande.serialEvent();
}
void AltimeterCheck(){
	azande.send(current_system, "AltimeterCheck");
	delay(10);
	azande.serialEvent();
	azande.send(user_instructions, "Connect 9V to first altimeter pin for one second.");
	azande.serialEvent();
	int read = 0;
	long t = millis();
	do{
		if ((millis() - t) > 10000){ //10 seconds to perfrom this before it times out
			azande.send(user_instructions, "Pin 1 timeout failure.");
			azande.serialEvent();
			read += 2;
			break;
		}
	} while (!Int2);
	Int2 = false;
	delay(100);
	azande.send(user_instructions, "Connect 9V to second altimeter pin for one second.");
	do{
		if ((millis() - t) > 10000){ //10 seconds to perfrom this before it times out
			azande.send(user_instructions, "Pin 2 timeout failure.");
			azande.serialEvent();
			read += 3;
			break;
		}
	} while (!Int2);
	Int2 = false;
	azande.send(user_instructions, "");
	Altimeter_flag = ErrorCheck(eALT, read);
	azande.send(ALT_disp, Altimeter_flag);
	azande.serialEvent();
}
void MotorCheck(){
	azande.send(current_system, "MotorCheck");
	delay(10);
	azande.serialEvent();
	int read = 0;
	// add check code here
	Motor_flag = ErrorCheck(eMOTOR, read);
	azande.send(MOT_disp, Motor_flag);
	azande.serialEvent();
} //Unsure of how to check this for now. 
void PosCheck(){
	azande.send(current_system, "PositionCheck");
	delay(10);
	azande.serialEvent(); 
	int read = 0;
	// add check code here
	Position_flag = ErrorCheck(ePOS, read);
	azande.send(POS_disp, Position_flag);
	azande.serialEvent();
} //Will need sensor on the legs/frame
void BeginCheck(){
	azande.send(current_system, "BeginCheck");
	delay(10);
	azande.serialEvent(); 
	int read = digitalRead(SENS2);
	int previous = read;
	azande.send(user_instructions, "Start Sensor Test: Please Trigger Sensor");
	azande.serialEvent();
	long t = millis();

	do{
		if ((millis() - t) > 20000){ //20 seconds to perfrom this before it times out
			azande.send(user_instructions, "Start Sensor Timeout Failure.");
			azande.serialEvent();
			read = 1;
			break;
		}
	} while (previous != read);
	Begin_flag = ErrorCheck(eBEGIN, read);
	azande.send(BEG_disp, Begin_flag);
	azande.serialEvent();
} //
/*******************************************************************/
bool ErrorCheck(ERRORS str, int num){
	//check against error codes, run LED blink, and tell the serial interface what happened. true if no errors. False if errors. 
	//change StartupCheck_flag, and flag for each system
	switch (str){
	case eCOM: 
		switch (num){
		case 0: 
			azande.send(test_status, "");
			return false;
		}
		break;
	case eSD: 
		switch (num){
		case(0) :
			azande.send(test_status, "SD Error: Time out. Check connections between boards.");
			azande.serialEvent();
			Startup_check_flag = false;
			return false;
		case(1) :
			azande.send(test_status, "SD: Test successful.");
			azande.serialEvent();
			return true;
		case(3) :
			azande.send(test_status, "SD Error: File not found.");
			azande.serialEvent();
			Startup_check_flag = false;
			return false;
		case(5) :
			azande.send(test_status, "Misc. Error: Retry.");
			azande.serialEvent();
			Startup_check_flag = false;
			return false;
		default:
			azande.send(test_status, "Error code not recognized. This isn't good!");
			azande.serialEvent();
			Startup_check_flag = false;
			return false;
		}
		break;
	case eLCD:
		switch (num){
		case(0) :
			azande.send(test_status, "LCD Error: Time out. Check connections between boards.");
			azande.serialEvent();
			Startup_check_flag = false;
			return false;
		case(1) :
			azande.send(test_status, "LCD: Test successful.");
			azande.serialEvent();
			return true;
		case(3) :
			azande.send(test_status, "LCD Error: File not found.");
			azande.serialEvent();
			Startup_check_flag = false;
			return false;
		default:
			azande.send(test_status, "Error code not recognized. This isn't good!");
			azande.serialEvent();
			Startup_check_flag = false;
			return false;
		}
		break;
	case e5V:
		switch (num){
		case 0: 
			azande.send(test_status, "5V Power error.");
			azande.serialEvent();
			Startup_check_flag = false;
			return false;
		case 1:
			azande.send(test_status, "5V Power Test: Success");
			azande.serialEvent();
			return true;
		default: azande.send(test_status, "5V undefined error.");
			azande.serialEvent();
			Startup_check_flag = false;
			return false;
		}
		break;
	case e32V:
		switch (num){
		case 0:
			azande.send(test_status, "32V Power error.");
			azande.serialEvent();
			Startup_check_flag = false;
			return false;
		case 1:
			azande.send(test_status, "32V Power Test: Success");
			azande.serialEvent();
			return true;
		default: azande.send(test_status, "32V undefined error.");
			azande.serialEvent();
			Startup_check_flag = false;
			return false;
		}
		break;
	case eLED: //Not any chip method of error sensing. Will use a photosensor atached to SENS3
		switch (num){
		case 0: 
			azande.send(test_status, "LED Test error.");
			azande.serialEvent();
			Startup_check_flag = false;
			return false;
		case 1:
			azande.send(test_status, "LED Test: Success");
			azande.serialEvent();
			return true;
		default: 
			azande.send(test_status, "LED undefined error.");
			azande.serialEvent();
			Startup_check_flag = false;
			return false;
		}
		break;
	case eALT:
		switch (num){
		case 0:
			azande.send(test_status, "Altimeter Test success.");
			azande.serialEvent();
			return true;
		case 2:
			azande.send(test_status, "Altimeter Test: Pin 1 failure.");
			azande.serialEvent();
			Startup_check_flag = false;
			return false;
		case 3: 
			azande.send(test_status, "Altimeter Test: Pin 2 failure.");
			azande.serialEvent();
			Startup_check_flag = false;
			return false;
		case 5:
			azande.send(test_status, "Altimeter Test: Pin 1 & Pin 2 failure.");
			azande.serialEvent();
			Startup_check_flag = false;
			return false;
		default:
			azande.send(test_status, "Altimeter undefined error.");
			azande.serialEvent();
			Startup_check_flag = false;
			return false;
		}
		break;
	case eMOTOR:
		switch (num){
		default: 
			azande.send(test_status, "Motor Test: ");
			azande.serialEvent();
			Startup_check_flag = false;
			return false;
		}
		break;
	case ePOS:
		switch (num){
		default:
			azande.send(test_status, "Position Test: ");
			azande.serialEvent();
			Startup_check_flag = false;
			return false;
		}
		break;
	case eBEGIN:
		switch (num){
		case 0: 
			azande.send(test_status, "Start Sensor Test: Success");
			azande.serialEvent();
			return true;
		case 1: 
			azande.send(test_status, "Start Sensor Test Failure");
			azande.serialEvent();
			Startup_check_flag = false;
			return false;
		default:
			azande.send(test_status, "Start undefined error");
			azande.serialEvent();
			Startup_check_flag = false;
			return false;
		}
		break;
	default:
		azande.send(test_status, "INVALID PARAMETER PASSED TO ERRORCHECK()");
		azande.serialEvent();
		Startup_check_flag = false;
		return false;
		break;
	}
	return false;
}
bool ErrorCheck(ERRORS str, int num[]){ //Array version of ErrorCheck to be used with CommCheck();
	//check against error codes, run LED blink, and tell the serial interface what happened. true if no errors. False if errors. 
	//change StartupCheck_flag, and flag for each system
	String message = "Communication Status: ";
	switch (str){
	case eCOM:
		if (num[0]){
			message += "SEQ_CNTRL-O SEQ_FDBK-I failure. ";
		}
		if (num[1]){
			message += "SEQ_FDBK-O and SEQ_LAYER-I failure. ";
		}
		if (num[2]){
			message += "SEQ_LAYER-O and SEQ_CNTRL-I failure. ";
		}
		if (num[0] || num[1] || num[2]){
			char msg[message.length() + 1];
			message.toCharArray(msg, message.length() + 1);
			azande.send(test_status, msg);
			azande.serialEvent();
			Startup_check_flag = false;
			return false;
		}
		char msg[message.length() + 1];
		message.toCharArray(msg, message.length() + 1);
		message += "Success.";
		azande.send(test_status, msg);
		azande.serialEvent();
		return true;
		break;
	default:
		azande.send(test_status, "INVALID PARAMETER PASSED TO ERRORCHECK()");
		azande.serialEvent();
		Startup_check_flag = false;
		return false;
		break;
	}
	return false;
}
/*******************************************************************/
void AzandeStartupReset(){
	//reset all testing flags for all systems. 
	LCD_flag = false;
	SD_flag = false;
	Comm_flag = false;
	p5V_flag = false;
	p32V_flag = false;
	LED_flag = false;
	Altimeter_flag = false;
	Motor_flag = false;
	Begin_flag = false;
	Position_flag = false;

	Startup_check_flag = false;
	SystemCheck();
}
/*******************************************************************/
void CloseAzandeAndPrime(){
	azande.clear();
	azande.notify();
	Serial.end();
	digitalWrite(ERRORSIG, HIGH); //sets the board LED on
	Standby();
	long time = millis();
	while (millis() - time < 300000){ ; }
	digitalWrite(ERRORSIG, LOW); //sets the board LED off to signal timer is finished
	Azande_flag = false;
	done = 0;
}
/**************************END GUI methods**************************/
/**********************Begin Printing Methods***********************/
void ISR_1(){ //do not use delay or millis inside ISRs
	if (!Azande_flag){
		while (digitalRead(PGOOD32V) != HIGH){
			delayMicroseconds(200);
		}
	}
} //Pgood. Wait and check to see if power is restored before beginning printing again.
/*******************************************************************/
void ISR_2(){
	Int2 = true;
} //Apogee signal. Done. 
/*******************************************************************/
void Standby(){ //Explore what else could be easily powered down without effecting operations. 
	//signal to Arduino 2 to enter standby mode
	for (int i = 0; i < 5; i++){ //5 HIGH pulses within 600 ms will signal to the other to enter/leave standby by switching a standby flag on Arduino 2 code. 
		digitalWrite(SEQ_CNTRL, HIGH);
		delay(50);
		digitalWrite(SEQ_CNTRL, LOW);
		delay(50);
	} //There doesn't need to be a delay since everything in Arduino 2 runs off 5V and only 32V will be shut down.
	//Standby settings for Arduino 1
	analogWrite(LEDSIG, 0); //turns off LED
	detachInterrupt(digitalPinToInterrupt(PGOOD32V));
	digitalWrite(ONOFF32V, HIGH); //HIGH=off, LOW=on. Runs both motors and LED
} //Still needs work
/*******************************************************************/
void Reboot(){
	digitalWrite(ERRORSIG, HIGH);
	//Start Arduino 2
	for (int i = 0; i < 5; i++){ //5 HIGH pulses within 600 ms will signal to the other to enter/leave standby by switching a standby flag on Arduino 2 code. 
		digitalWrite(SEQ_CNTRL, HIGH);
		delay(50);
		digitalWrite(SEQ_CNTRL, LOW);
		delay(50);
	}
	//Positioning sensor

	//32V power
	digitalWrite(ONOFF32V, LOW);
	//LED chip?
	//Checking if power is good
	attachInterrupt(digitalPinToInterrupt(PGOOD32V), ISR_1, FALLING);

}//Still needs work
/*******************************************************************/
void Print(){ //Needs to be double checked
	//load image on LCD
	digitalWrite(SEQ_CNTRL, HIGH);
	//Wait for confirmation that it is loaded
	do{
		delayMicroseconds(50);
	} while (digitalRead(SEQ_FDBK) != LOW);
	digitalWrite(SEQ_CNTRL, LOW);
	//set printbed for base layer
	MOVE(BASE_LAYER_HEIGHT); //be careful about direction. Test when set up ****
	LED(BASE_LED_TIME);
	MOVE(LAYER_HEIGHT);
	//Signal to change the image
	//Loop
	do{
		digitalWrite(SEQ_CNTRL, HIGH); //loads next image
		do{
			delayMicroseconds(75); //delays so Nano2 has time to change SEQ_FDBK to HIGH in response to CNTRL
		} while (digitalRead(SEQ_FDBK) != LOW); //confirmation that it is loaded
		digitalWrite(SEQ_CNTRL, LOW); //resets interupt control pin
		LED(BASE_LED_TIME);
		MOVE(LAYER_HEIGHT);
	} while (digitalRead(SEQ_LAYER) == LOW); //next image exists
}
/*******************************************************************/
void LED(int a){ //a is in milliseconds
	long t = millis();
	analogWrite(LEDSIG, LED_INTENSITY);
	while (millis() < (t + a)){ ; }
	analogWrite(LEDSIG, 0);
}
/*******************************************************************/
void MOVE(float dist){ //dist is in um (micrometeters). Negative numbers work. 
	//thread pitch is 2mm/rev
	//200 steps/rev --> 2mm/rev --> 100 steps/ 1mm = 1000um --> 1 step = 0.1um
	st1.step((int)dist / .1);
}
/*******************************************************************/
