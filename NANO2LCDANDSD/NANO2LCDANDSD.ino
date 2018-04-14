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
/*char* layerIMG[] = {  "out0000.RAW", "out0001.RAW", "out0002.RAW", "out0003.RAW",..., "out0498.RAW", "out0499.RAW"}; 
//include names of all layers of the print that are stored on the SD card */

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
/********************End Global Variables and inits********************/
volatile bool standby_flag = false;
volatile bool recheck_flag = false;
volatile bool print_flag = false;
char filename[12] = "out00000.bmp"
/***********************Begin Arduino Main Loops***********************/
void setup() {


	StartUp();
	StatusCheck();

}
void loop() {
	//loop to check for standby
	//loop to print?
	if (standby_flag){
		//do standby stuff
	}
	else if (print_flag){
		int hold1 = i;
		int hold2 = 0;
		int count = 0;
		int thing = 0;
		// interrupt handles changing the condition for next layer
		// change image
		filename[7] = char(i);
		while (hold1 > 0){
			hold2 = hold1 / 10;
			thing = hold1 - hold2 * 10;
			hold1 = hold2;
			filename[7 - count] = char(thing);
			count++;
		}
		drawBMP( *filename, 0, 0, 1);
		//turn seq_fdbk low
	}
	else if (recheck_flag){
		StatusCheck();
	}
	else if ()






	/**********************************************************************************/
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
	drawBMP("csatstar.bmp", 0, 0, BU_BMP);
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
	Standby();
}
/**********************************************************************/
void CommCheck(){
	int num = 0;
	//Wait for all comms to go high to signal the start of CommCheck
	//pinModes alreadys set in StartUp
	while (digitalRead(SEQ_CNTRL) && digitalRead(SEQ_FDBK) && digitalRead(SEQ_LAYER)){ ; }
	//
	//First pair: Testing SEQ_CNTRL as output and SEQ_FDBK as input
	pinMode(SEQ_CNTRL, INPUT);
	pinMode(SEQ_FDBK, OUTPUT);
	num = listen(SEQ_CNTRL);
	send(num, SEQ_FDBK);

	//Second pair: Testing SEQ_FDBK as output and SEQ_LAYER as input
	pinMode(SEQ_FDBK, INPUT);
	pinMode(SEQ_LAYER, OUTPUT);
	num = listen(SEQ_FDBK);
	send(num, SEQ_LAYER);

	//Third pair: Testing SEQ_LAYER as output and SEQ_CNTRL as input
	pinMode(SEQ_LAYER, INPUT);
	pinMode(SEQ_CNTRL, OUTPUT);
	num = listen(SEQ_LAYER);
	send(num, SEQ_CNTRL);
	
	delay(50);
	pinMode(SEQ_CNTRL, INPUT);
	pinMode(SEQ_FDBK, OUTPUT);
	pinMode(SEQ_LAYER, OUTPUT);

	attachInterrupt(digitalPinToInterrupt(SEQ_CNTRL, RISING);
}
void SDCheck(){
	int num = 0;
	while (digitalRead(SEQ_CNTRL) == LOW){ ; }
	//Method to check if SD works and return type of error if not
	if (!SD.begin(lcdCS, SPI_FULL_SPEED)) {  // sdFat library allows speed setting, e.g. SPI_HALF_SPEED
		//if (!SD.begin(_sdcs)) {              // Only needed when standard SD library is used
		num = 6; //SD initialization failed
	}
	else if ((bmpFile = SD.open("Black000.bmp")) == NULL) {
		num = 3; // File not found. 
	}

	send(num);
	
}
void LCDCheck(){
	int num = 0;
	//stuff

	send(num);
}
int listen(){ //default is SEQ_CNTRL
	int num = 0;
	bool status, prevStatus = digitalRead(SEQ_CNTRL);
	prevTime = millis();

	do{
		status = digitalRead(SEQ_CNTRL);
		if (status != prevStatus && status == HIGH){
			num++;
			prevStatus = status;
			prevTime = millis();
		}
	} while ((millis() - prevTime) < 500 || num < 1);
	return num;
}
int listen(int pin){
	int num = 0;
	bool status, prevStatus = digitalRead(pin);
	prevTime = millis();

	do{
		status = digitalRead(pin);
		if (status != prevStatus && status == HIGH){
			num++;
			prevStatus = status;
			prevTime = millis();
		}
	} while ((millis() - prevTime) < 500 || num < 1);
	return num;
}
void send(int n){ //default is to SEQ_FDBK pin
	delay(50);
	for (int i = 0; i < n; i++){
		digitalWrite(SEQ_FDBK, LOW);
		delay(50);
		digitalWrite(SEQ_FDBK, HIGH);
		delay(50);
	}
}
void send(int n, int pin){
	delay(50);
	for (int i = 0; i < n; i++){
		digitalWrite(pin, LOW);
		delay(50);
		digitalWrite(pin, HIGH);
		delay(50);
	}
}
/**************************End StartUp Methods*************************/
/************************Begin Printing Methods************************/
void ISR_2_1(){ //listens for next command from Nano1 //Needs work. Not sure how to work it in with the rest of it. 
	//listen();
	//Can't use listen because of millis(). Will probably use a flag to check in the loop. 
	if (digitalRead(SEQ_FDBK) == ? &&digitalRead(SEQ_LAYER) == ? ){ //this runs in case the button to reset and recheck the systems is run.
		detachInterrupt(digitalPinToInterrupt(SEQ_CNTRL));
		recheck_flag = true;
		return;
	}
	if (print_flag){
		//do something for the print. maybe increment the layer?
		digitalWrite(SEQ_FDBK, HIGH);
		i = i + 1; //increments image index
	}
	if (standby_flag){
		//Not sure tbh
	}
	else{}
	/***************************/
	digitalWrite(SEQ_FDBK, HIGH);
	i = i + 1; //increments image index
}

/**********************************************************************/

void Print(){ //Not necessary. Will use a series of flags and ifs in loop() to run the sequence of events. 

}
//Work on these later
void Standby(){
	//See if there is a low power mode to the LCD. Maybe uninitialize the pins? or set all low?
	pinMode(lcdCS, INPUT);
	pinMode(lcdDC, INPUT);
	pinMode(lcdMISO, INPUT);
	pinMode(lcdMOSI, INPUT);
	pinMode(lcdRST, INPUT);
	pinMode(SDCS, INPUT);
	pinMode(SEQ_CNTRL, INPUT);
	pinMode(SEQ_FDBK, INPUT);
	pinMode(SEQ_LAYER, INPUT);


}
void Reboot(){
	// do the oposite of Standby
	pinMode(lcdCS, OUTPUT);
	pinMode(lcdDC, OUTPUT);
	pinMode(lcdMISO, OUTPUT);
	pinMode(lcdMOSI, OUTPUT);
	pinMode(lcdRST, OUTPUT);
	pinMode(SDCS, OUTPUT);
	pinMode(SEQ_CNTRL, INPUT);
	pinMode(SEQ_FDBK, OUTPUT);
	pinMode(SEQ_LAYER, OUTPUT);

	//Send LCD the command to exit sleep mode
}
/***************************************************************************************
** Function name:           drawBMP
** Descriptions:            draw a BMP format bitmap to the screen
***************************************************************************************/

// This function opens a Windows Bitmap (BMP) file and
// displays it at the given coordinates.  It's sped up
// by reading many pixels worth of data at a time
// (rather than pixel by pixel).  Increasing the buffer
// size makes loading a little faster but the law of
// rapidly diminishing speed improvements applies.
// Suggest 8 minimum and 85 maximum (3 x this value is
// stored in a byte = 255/3 max!)
// A value of 8 is only ~20% slower than 24 or 48!
// Note that 5 x this value of RAM bytes will be needed
// Increasing beyond 48 gives little benefit.
// Use integral division of TFT (or typical often used image)
// width for slightly better speed to avoid short buffer purging

#define BUFF_SIZE 80
void drawBMP(char *filename, int x, int y, boolean flip) {
	if ((x >= tft.width()) || (y >= tft.height())) return;
	File     bmpFile;
	int16_t  bmpWidth, bmpHeight;   // Image W+H in pixels
	//uint8_t  bmpDepth;            // Bit depth (must be 24) but we dont use this
	uint32_t bmpImageoffset;        // Start address of image data in file
	uint32_t rowSize;               // Not always = bmpWidth; may have padding
	uint8_t  sdbuffer[3 * BUFF_SIZE];    // SD read pixel buffer (8 bits each R+G+B per pixel)
	uint16_t tftbuffer[BUFF_SIZE];       // TFT pixel out buffer (16-bit per pixel)
	uint8_t  sd_ptr = sizeof(sdbuffer); // sdbuffer pointer (so BUFF_SIZE must be less than 86)
	boolean  goodBmp = false;            // Flag set to true on valid header parse
	int16_t  w, h, row, col;             // to store width, height, row and column
	//uint8_t  r, g, b;   // brg encoding line concatenated for speed so not used
	uint8_t rotation;     // to restore rotation
	uint8_t  tft_ptr = 0;  // buffer pointer

	// Check file exists and open it
	if ((bmpFile = SD.open(filename)) == NULL) {
		Serial.println(F("File not found")); // Can comment out if not needed
		return;
	}

	drawTime = millis(); // Save current time for performance evaluation, comment out if not needed

	// Parse BMP header to get the information we need
	if (read16(bmpFile) == 0x4D42) { // BMP file start signature check
		read32(bmpFile);       // Dummy read to throw away and move on
		read32(bmpFile);       // Read & ignore creator bytes
		bmpImageoffset = read32(bmpFile); // Start of image data
		read32(bmpFile);       // Dummy read to throw away and move on
		bmpWidth = read32(bmpFile);  // Image width
		bmpHeight = read32(bmpFile);  // Image height

		//if (read16(bmpFile) == 1) { // Number of image planes -- must be '1'
		// Only proceed if we pass a bitmap file check
		if ((read16(bmpFile) == 1) && (read16(bmpFile) == 24) && (read32(bmpFile) == 0)) { // Must be depth 24 and 0 (uncompressed format)
			//goodBmp = true; // Supported BMP format -- proceed!
			// BMP rows are padded (if needed) to 4-byte boundary
			rowSize = (bmpWidth * 3 + 3) & ~3;
			// Crop area to be loaded
			w = bmpWidth;
			h = bmpHeight;

			// We might need to alter rotation to avoid tedious pointer manipulation
			// Save the current value so we can restore it later
			rotation = tft.getRotation();
			// Use TFT SGRAM coord rotation if flip is set for 25% faster rendering
			if (flip) tft.setRotation((rotation + (flip << 2)) % 8); // Value 0-3 mapped to 4-7

			// We might need to flip and calculate new y plot coordinate
			// relative to top left corner as well...
			switch (rotation) {
			case 0:
				if (flip) y = tft.height() - y - h; break;
			case 1:
				y = tft.height() - y - h; break;
				break;
			case 2:
				if (flip) y = tft.height() - y - h; break;
				break;
			case 3:
				y = tft.height() - y - h; break;
				break;
			}

			// Set TFT address window to image bounds
			// Currently, image will not draw or will be corrputed if it does not fit
			// TODO -> efficient clipping, I don't need it to be idiot proof ;-)
			tft.setAddrWindow(x, y, x + w - 1, y + h - 1);

			// Finally we are ready to send rows of pixels, writing like this avoids slow 32 bit multiply
			for (uint32_t pos = bmpImageoffset; pos < bmpImageoffset + h * rowSize; pos += rowSize) {
				// Seek if we need to on boundaries and arrange to dump buffer and start again
				if (bmpFile.position() != pos) {
					bmpFile.seek(pos);
					sd_ptr = sizeof(sdbuffer);
				}

				// Fill the pixel buffer and plot
				for (col = 0; col < w; col++) { // For each column...
					// Time to read more pixel data?
					if (sd_ptr >= sizeof(sdbuffer)) {
						// Push tft buffer to the display
						if (tft_ptr) {
							// Here we are sending a uint16_t array to the function
							tft.pushColors(tftbuffer, tft_ptr);
							tft_ptr = 0; // tft_ptr and sd_ptr are not always in sync...
						}
						// Finally reading bytes from SD Card
						bmpFile.read(sdbuffer, sizeof(sdbuffer));
						sd_ptr = 0; // Set buffer index to start
					}
					// Convert pixel from BMP 8+8+8 format to TFT compatible 16 bit word
					// Blue 5 bits, green 6 bits and red 5 bits (16 bits total)
					// Is is a long line but it is faster than calling a library fn for this
					tftbuffer[tft_ptr++] = (sdbuffer[sd_ptr++] >> 3) | ((sdbuffer[sd_ptr++] & 0xFC) << 3) | ((sdbuffer[sd_ptr++] & 0xF8) << 8);
				} // Next row
			}   // All rows done

			// Write any partially full buffer to TFT
			if (tft_ptr) tft.pushColors(tftbuffer, tft_ptr);
			drawTime = millis() - drawTime;
		} // End of bitmap access
	}   // End of bitmap file check
	//}     // We can close the file now

	bmpFile.close();
	//if(!goodBmp) Serial.println(F("BMP format not recognized."));
	tft.setRotation(rotation); // Put back original rotation
}

/***************************************************************************************
** Function name:           Support functions for drawBMP()
** Descriptions:            Read 16- and 32-bit types from the SD card file
***************************************************************************************/

// BMP data is stored little-endian, Arduino is little-endian too.
// May need to reverse subscript order if porting elsewhere.

uint16_t read16(File& f) {
	uint16_t result;
	((uint8_t *)&result)[0] = f.read(); // LSB
	((uint8_t *)&result)[1] = f.read(); // MSB
	return result;
}

uint32_t read32(File& f) {
	uint32_t result;
	((uint8_t *)&result)[0] = f.read(); // LSB
	((uint8_t *)&result)[1] = f.read();
	((uint8_t *)&result)[2] = f.read();
	((uint8_t *)&result)[3] = f.read(); // MSB
	return result;
}
