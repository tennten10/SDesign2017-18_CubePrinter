//Code modified from Adafruit ILI9341_draw_bitmap_v2 example
// On the SD card use 24 bit color BMP files (be sure they are 24-bit!)
// There are examples images in the sketch folder.

// Change IDE compiler option to -O2 as per Instructable to boost speed 
// http://www.instructables.com/id/Arduino-IDE-16x-compiler-optimisations-faster-code/
// this example then uses 85% of UNO FLASH and 52% of RAM for dynamic storage


// MS Paint can be used to crop, resize and flip and save images in 24bit BMP format

//          ######################## WARNINGS ########################
// ####     Only enable Font 2 or the UNO willl run out of FLASH memory!      ####
// ####  Do not try to use the F_AS_T option in the ILI9341_AS library yet    ####
// #### Change IDE compiler option to -O2 as per Instructable to boost speed  ####


// Updated 31/3/15 to include new drawRAW() function that draws raw 16 bit images.
// Raw images are created with UTFT library tool (libraries\UTFT\Tools\ImageConverter565.exe)
// Save as .raw file as this can be easily piped to the TFT pushColors() function
// without any tedious byte swapping etc

#include <Adafruit_GFX_AS.h>     // Core graphics library
#include <Adafruit_ILI9341_AS.h> // Hardware-specific library
//#include <SD.h>                // SD card library a bit slower and bigeger than SdFat
#include <SdFat.h>               // More compact and faster than SD
SdFat SD;                        // For SD compatibility
#include <SPI.h>                 // SPI libray obviously!

#include "Config_Parameters.h"
#include "pins2.h"

Adafruit_ILI9341_AS tft = Adafruit_ILI9341_AS(lcdCS, lcdDC, lcdRST); // Invoke custom library

long prevTime = 0;

/**********************************************************************/

//set up for cone ring test print
/*char* layerIMG[] = {  "out0000.RAW", "out0001.RAW", "out0002.RAW", "out0003.RAW"}; /*, "out0004.RAW", "out0005.RAW", "out0006.RAW", "out0007.RAW",
"out0008.RAW", "out0009.RAW", "out0010.RAW", "out0011.RAW", "out0012.RAW", "out0013.RAW", "out0014.RAW", "out0015.RAW", "out0016.RAW",
"out0017.RAW", "out0018.RAW", "out0019.RAW", "out0020.RAW", "out0021.RAW", "out0022.RAW", "out0023.RAW", "out0024.RAW", "out0025.RAW",
"out0026.RAW", "out0027.RAW", "out0028.RAW", "out0029.RAW", "out0030.RAW", "out0031.RAW", "out0032.RAW", "out0033.RAW", "out0034.RAW",
"out0035.RAW", "out0036.RAW", "out0037.RAW", "out0038.RAW", "out0039.RAW", "out0040.RAW", "out0041.RAW", "out0042.RAW", "out0043.RAW",
"out0044.RAW", "out0045.RAW", "out0046.RAW", "out0047.RAW", "out0048.RAW", "out0049.RAW", "out0050.RAW", "out0051.RAW", "out0052.RAW",
"out0053.RAW", "out0054.RAW", "out0055.RAW", "out0056.RAW", "out0057.RAW", "out0058.RAW", "out0059.RAW", "out0060.RAW", "out0061.RAW",
"out0062.RAW", "out0063.RAW", "out0064.RAW", "out0065.RAW", "out0066.RAW", "out0067.RAW", "out0068.RAW", "out0069.RAW", "out0070.RAW",
"out0071.RAW", "out0072.RAW", "out0073.RAW", "out0074.RAW", "out0075.RAW", "out0076.RAW", "out0077.RAW", "out0078.RAW", "out0079.RAW",
"out0080.RAW", "out0081.RAW", "out0082.RAW", "out0083.RAW", "out0084.RAW", "out0085.RAW", "out0086.RAW", "out0087.RAW", "out0088.RAW",
"out0089.RAW", "out0090.RAW", "out0091.RAW", "out0092.RAW", "out0093.RAW", "out0094.RAW", "out0095.RAW", "out0096.RAW", "out0097.RAW",
"out0098.RAW", "out0099.RAW", "out0100.RAW", "out0101.RAW", "out0102.RAW", "out0103.RAW", "out0104.RAW", "out0105.RAW", "out0106.RAW",
"out0107.RAW", "out0108.RAW", "out0109.RAW", "out0110.RAW", "out0111.RAW", "out0112.RAW", "out0113.RAW", "out0114.RAW", "out0115.RAW",
"out0116.RAW", "out0117.RAW", "out0118.RAW", "out0119.RAW", "out0120.RAW", "out0121.RAW", "out0122.RAW", "out0123.RAW", "out0124.RAW",
"out0125.RAW", "out0126.RAW", "out0127.RAW", "out0128.RAW", "out0129.RAW", "out0130.RAW", "out0131.RAW", "out0132.RAW", "out0133.RAW",
"out0134.RAW", "out0135.RAW", "out0136.RAW", "out0137.RAW", "out0138.RAW", "out0139.RAW", "out0140.RAW", "out0141.RAW", "out0142.RAW",
"out0143.RAW", "out0144.RAW", "out0145.RAW", "out0146.RAW", "out0147.RAW", "out0148.RAW", "out0149.RAW", "out0150.RAW", "out0151.RAW",
"out0152.RAW", "out0153.RAW", "out0154.RAW", "out0155.RAW", "out0156.RAW", "out0157.RAW", "out0158.RAW", "out0159.RAW", "out0160.RAW",
"out0161.RAW", "out0162.RAW", "out0163.RAW", "out0164.RAW", "out0165.RAW", "out0166.RAW", "out0167.RAW", "out0168.RAW", "out0169.RAW",
"out0170.RAW", "out0171.RAW", "out0172.RAW", "out0173.RAW", "out0174.RAW", "out0175.RAW", "out0176.RAW", "out0177.RAW", "out0178.RAW",
"out0179.RAW", "out0180.RAW", "out0181.RAW", "out0182.RAW", "out0183.RAW", "out0184.RAW", "out0185.RAW", "out0186.RAW", "out0187.RAW",
"out0188.RAW", "out0189.RAW", "out0190.RAW", "out0191.RAW", "out0192.RAW", "out0193.RAW", "out0194.RAW", "out0195.RAW", "out0196.RAW",
"out0197.RAW", "out0198.RAW", "out0199.RAW", "out0200.RAW", "out0201.RAW", "out0202.RAW", "out0203.RAW", "out0204.RAW", "out0205.RAW",
"out0206.RAW", "out0207.RAW", "out0208.RAW", "out0209.RAW", "out0210.RAW", "out0211.RAW", "out0212.RAW", "out0213.RAW", "out0214.RAW",
"out0215.RAW", "out0216.RAW", "out0217.RAW", "out0218.RAW", "out0219.RAW", "out0220.RAW", "out0221.RAW", "out0222.RAW", "out0223.RAW",
"out0224.RAW", "out0225.RAW", "out0226.RAW", "out0227.RAW", "out0228.RAW", "out0229.RAW", "out0230.RAW", "out0231.RAW", "out0232.RAW",
"out0233.RAW", "out0234.RAW", "out0235.RAW", "out0236.RAW", "out0237.RAW", "out0238.RAW", "out0239.RAW", "out0240.RAW", "out0241.RAW",
"out0242.RAW", "out0243.RAW", "out0244.RAW", "out0245.RAW", "out0246.RAW", "out0247.RAW", "out0248.RAW", "out0249.RAW", "out0250.RAW",
"out0251.RAW", "out0252.RAW", "out0253.RAW", "out0254.RAW", "out0255.RAW", "out0256.RAW", "out0257.RAW", "out0258.RAW", "out0259.RAW",
"out0260.RAW", "out0261.RAW", "out0262.RAW", "out0263.RAW", "out0264.RAW", "out0265.RAW", "out0266.RAW", "out0267.RAW", "out0268.RAW",
"out0269.RAW", "out0270.RAW", "out0271.RAW", "out0272.RAW", "out0273.RAW", "out0274.RAW", "out0275.RAW", "out0276.RAW", "out0277.RAW",
"out0278.RAW", "out0279.RAW", "out0280.RAW", "out0281.RAW", "out0282.RAW", "out0283.RAW", "out0284.RAW", "out0285.RAW", "out0286.RAW",
"out0287.RAW", "out0288.RAW", "out0289.RAW", "out0290.RAW", "out0291.RAW", "out0292.RAW", "out0293.RAW", "out0294.RAW", "out0295.RAW",
"out0296.RAW", "out0297.RAW", "out0298.RAW", "out0299.RAW", "out0300.RAW", "out0301.RAW", "out0302.RAW", "out0303.RAW", "out0304.RAW",
"out0305.RAW", "out0306.RAW", "out0307.RAW", "out0308.RAW", "out0309.RAW", "out0310.RAW", "out0311.RAW", "out0312.RAW", "out0313.RAW",
"out0314.RAW", "out0315.RAW", "out0316.RAW", "out0317.RAW", "out0318.RAW", "out0319.RAW", "out0320.RAW", "out0321.RAW", "out0322.RAW",
"out0323.RAW", "out0324.RAW", "out0325.RAW", "out0326.RAW", "out0327.RAW", "out0328.RAW", "out0329.RAW", "out0330.RAW", "out0331.RAW",
"out0332.RAW", "out0333.RAW", "out0334.RAW", "out0335.RAW", "out0336.RAW", "out0337.RAW", "out0338.RAW", "out0339.RAW", "out0340.RAW",
"out0341.RAW", "out0342.RAW", "out0343.RAW", "out0344.RAW", "out0345.RAW", "out0346.RAW", "out0347.RAW", "out0348.RAW", "out0349.RAW",
"out0350.RAW", "out0351.RAW", "out0352.RAW", "out0353.RAW", "out0354.RAW", "out0355.RAW", "out0356.RAW", "out0357.RAW", "out0358.RAW",
"out0359.RAW", "out0360.RAW", "out0361.RAW", "out0362.RAW", "out0363.RAW", "out0364.RAW", "out0365.RAW", "out0366.RAW", "out0367.RAW",
"out0368.RAW", "out0369.RAW", "out0370.RAW", "out0371.RAW", "out0372.RAW", "out0373.RAW", "out0374.RAW", "out0375.RAW", "out0376.RAW",
"out0377.RAW", "out0378.RAW", "out0379.RAW", "out0380.RAW", "out0381.RAW", "out0382.RAW", "out0383.RAW", "out0384.RAW", "out0385.RAW",
"out0386.RAW", "out0387.RAW", "out0388.RAW", "out0389.RAW", "out0390.RAW", "out0391.RAW", "out0392.RAW", "out0393.RAW", "out0394.RAW",
"out0395.RAW", "out0396.RAW", "out0397.RAW", "out0398.RAW", "out0399.RAW", "out0400.RAW", "out0401.RAW", "out0402.RAW", "out0403.RAW",
"out0404.RAW", "out0405.RAW", "out0406.RAW", "out0407.RAW", "out0408.RAW", "out0409.RAW", "out0410.RAW", "out0411.RAW", "out0412.RAW",
"out0413.RAW", "out0414.RAW", "out0415.RAW", "out0416.RAW", "out0417.RAW", "out0418.RAW", "out0419.RAW", "out0420.RAW", "out0421.RAW",
"out0422.RAW", "out0423.RAW", "out0424.RAW", "out0425.RAW", "out0426.RAW", "out0427.RAW", "out0428.RAW", "out0429.RAW", "out0430.RAW",
"out0431.RAW", "out0432.RAW", "out0433.RAW", "out0434.RAW", "out0435.RAW", "out0436.RAW", "out0437.RAW", "out0438.RAW", "out0439.RAW",
"out0440.RAW", "out0441.RAW", "out0442.RAW", "out0443.RAW", "out0444.RAW", "out0445.RAW", "out0446.RAW", "out0447.RAW", "out0448.RAW",
"out0449.RAW", "out0450.RAW", "out0451.RAW", "out0452.RAW", "out0453.RAW", "out0454.RAW", "out0455.RAW", "out0456.RAW", "out0457.RAW",
"out0458.RAW", "out0459.RAW", "out0460.RAW", "out0461.RAW", "out0462.RAW", "out0463.RAW", "out0464.RAW", "out0465.RAW", "out0466.RAW",
"out0467.RAW", "out0468.RAW", "out0469.RAW", "out0470.RAW", "out0471.RAW", "out0472.RAW", "out0473.RAW", "out0474.RAW", "out0475.RAW",
"out0476.RAW", "out0477.RAW", "out0478.RAW", "out0479.RAW", "out0480.RAW", "out0481.RAW", "out0482.RAW", "out0483.RAW", "out0484.RAW",
"out0485.RAW", "out0486.RAW", "out0487.RAW", "out0488.RAW", "out0489.RAW", "out0490.RAW", "out0491.RAW", "out0492.RAW", "out0493.RAW",
"out0494.RAW", "out0495.RAW", "out0496.RAW", "out0497.RAW", "out0498.RAW", "out0499.RAW"}; //include names of all layers of the print that are stored on the SD card */

//idea. Create component parts of the file names and then increment the names in the code instead of storing a large number of them in memory. 
char layerIMG[LAYERS_IN_PRINT];
char* extension = ".RAW";
char* beg = "out";

boolean display_rendertime = false;
boolean display_filename = false;

word res;
long sm, em;
volatile int i = 0;
int lll;

int last = 0;
/***********************Begin Arduino Main Loops***********************/
void setup() {
	/*
	// put your setup code here, to run once:
	LCD.InitLCD();	
	LCD.clrScr();
	file.initFAT();
	LCD.setColor(255, 255, 255);
	//LCD.setFont(SmallFont);

	last = -1;*/
	StartUp();
	StatusCheck();

}
void loop() {

	if (i < LAYERS_IN_PRINT && last != i){
		last = i;
		sm = millis();
		lll = sprintf(layerIMG, "%s%.4d%s", beg, i, extension);
		res = myFiles.loadBitmap(0, 0, SCREEN_X, SCREEN_Y, layerIMG); //displays the image
		em = millis();
		//Error handling loop if files not there. Make sure to test this. Normally shouldn't print anything.
		if (res != 0)
		{
			if (res == 0x10)
			{
				LCD.print("File not found...", 0, 0);
				LCD.print(layerIMG, 0, 14);
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
			digitalWrite(SEQ_FDBK, LOW); // use pull up to hold high during use
		}
	}
	else{
		digitalWrite(SEQ_LAYER, HIGH);
	}
}
/************************End Arduino Main Loops************************/
/***************************Begin StartUp Methods**********************/
void StartUp(){
	pinMode(SEQ_CNTRL, INPUT);
	pinMode(SDCS, OUTPUT);
	pinMode(SEQ_FDBK, INPUT);
	pinMode(SEQ_LAYER, INPUT);
	pinMode(lcdRST, OUTPUT);
	pinMode(lcdDC, OUTPUT);
	pinMode(lcdCS, OUTPUT);
	pinMode(lcdMOSI, OUTPUT);
	pinMode(lcdMISO, OUTPUT);
	pinMode(SCLK, OUTPUT);

	//Wait for CommCheck before attaching the interrupts
}
/**********************************************************************/
void StatusCheck(){
	//Loop through system checks when prompted by comms
	
	CommCheck();
	SDCheck();
	LCDCheck();

}
/**********************************************************************/
void CommCheck(){
	//Wait for all comms to go high to signal the start of CommCheck
	//pinModes alreadys set in StartUp
	while (digitalRead(SEQ_CNTRL) && digitalRead(SEQ_FDBK) && digitalRead(SEQ_LAYER)){ ; }
	//
	//First pair: Testing SEQ_CNTRL as output and SEQ_FDBK as input
	int num = 0;
	bool status, prevStatus=digitalRead(SEQ_CNTRL);

	pinMode(SEQ_CNTRL, INPUT);
	pinMode(SEQ_FDBK, OUTPUT);
	
	prevTime = millis();
	do{
		status = digitalRead(SEQ_CNTRL);
		if (status != prevStatus && status == HIGH){
			num++;
			prevStatus = status;
			prevTime = millis();
		}
	} while ((millis() - prevTime) < 500 || num < 1);

	
	for (int i = 0; i < num; i++){
		digitalWrite(SEQ_FDBK, LOW);
		delay(50);
		digitalWrite(SEQ_FDBK, HIGH);
		delay(50);
	}
	//Second pair: Testing SEQ_FDBK as output and SEQ_LAYER as input
	pinMode(SEQ_FDBK, INPUT);
	pinMode(SEQ_LAYER, OUTPUT);
	num = 0;
	prevTime = millis();
	do{
		status = digitalRead(SEQ_FDBK);
		if (status != prevStatus && status == HIGH){
			num++;
			prevStatus = status;
			prevTime = millis();
		}
	} while ((millis() - prevTime) < 500 || num < 1);


	for (int i = 0; i < num; i++){
		digitalWrite(SEQ_LAYER, LOW);
		delay(50);
		digitalWrite(SEQ_LAYER, HIGH);
		delay(50);
	}
	//Third pair: Testing SEQ_LAYER as output and SEQ_CNTRL as input
	pinMode(SEQ_LAYER, INPUT);
	pinMode(SEQ_CNTRL, OUTPUT);
	num = 0;
	prevTime = millis();
	do{
		status = digitalRead(SEQ_LAYER);
		if (status != prevStatus && status == HIGH){
			num++;
			prevStatus = status;
			prevTime = millis();
		}
	} while ((millis() - prevTime) < 500 || num < 1);


	for (int i = 0; i < num; i++){
		digitalWrite(SEQ_CNTRL, LOW);
		delay(50);
		digitalWrite(SEQ_CNTRL, HIGH);
		delay(50);
	}
	delay(50);
	pinMode(SEQ_CNTRL, INPUT);
	pinMode(SEQ_FDBK, OUTPUT);
	pinMode(SEQ_LAYER, OUTPUT);

	attachInterrupt(digitalPinToInterrupt(2), ISR_2_1, RISING);
	attachInterrupt(digitalPinToInterrupt(3), ISR_2_2, RISING);
}
/**********************************************************************/
void SDCheck(){
	int num = 0;
	while (digitalRead(SEQ_CNTRL) == LOW){ ; }
	//Create method to check if SD works and return type of error if not
	
	if (!SD.begin(lcdCS, SPI_FULL_SPEED)) {  // sdFat library allows speed setting, e.g. SPI_HALF_SPEED
		//if (!SD.begin(_sdcs)) {              // Only needed when standard SD library is used
		//Serial.println(F("failed!"));
		num = 6; //Initialization Error
	}
	else if ((bmpFile = SD.open("Black.bmp")) == NULL) {
		//Serial.println(F("File not found")); // Can comment out if not needed
		num = 3; // File not found. 
	}


	for (int i = 0; i < num; i++){
		digitalWrite(SEQ_FDBK, LOW);
		delay(50);
		digitalWrite(SEQ_FDBK, HIGH);
		delay(50);
	}
	
}
/**********************************************************************/
void LCDCheck(){
	int num = 0;
	//stuff

	for (int i = 0; i < num; i++){
		digitalWrite(SEQ_FDBK, LOW);
		delay(50);
		digitalWrite(SEQ_FDBK, HIGH);
		delay(50);
	}
}
/**************************End StartUp Methods*************************/
/************************Begin Printing Methods************************/
void ISR_2_1(){ //listens for next command from Nano1 
	digitalWrite(SEQ_FDBK, HIGH);
	i = i + 1; //increments image index
}

/**********************************************************************/
void ISR_2_2(){ delayMicroseconds(50); } //does nothing right now.

/**********************************************************************/

void Print(){}
void Standby(){}
void Reboot(){}
