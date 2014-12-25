/***************************************************************/
/* FSX - NAV Radio on 7-seg Display     Version: 1.1           */
/* By: Patrick Castro     Date: 11-20-14                       */
/***************************************************************/

/* 
    This code is in the public domain
    For use with "Link2fs_Multi"
    Jimspage.co.nz
    My thanks to the Guys that gave me snippets of code. 
        
    >>>>>>> You must tick "=E" (NAV1 Frequency) in Multi. <<<<<<<
    >>>>> You must tick "=F" (NAV1 Standby) in Multi. <<<<<
*/



//Define variables
byte debug = 0; //set to "1" to enter debug state
int CodeIn; // used on all serial reads
byte clock1 = 2, latch1 = 3, data1 = 4; //used for serial output to shift register
unsigned int dis1_1 = 2, dis1_2 = 4, dis1_3 = 8, dis1_4 = 16, dis1_5 = 32, dp_1 = 1;  //Enable each digit of first display
unsigned int dis2_1 = 128, dis2_2 = 256, dis2_3 = 512, dis2_4 = 1024, dis2_5 = 2048, dp_2 = 64; //Enable each digit of second display
byte hunds_1, tens_1, ones_1, tenth_1, hundth_1; //Stored values for output to first display
byte hunds_2, tens_2, ones_2, tenth_2, hundth_2; //Stored values for output to second display
int ledpin = 13, errorState = 0; //Used for led on pin 13 of arduino
unsigned int nav1_int, nav1sb_int;
int showDig = 12, wait = 0, multiplex = 0;



// Define Arduino pins as INPUT or OUTPUT
void setup() 
{
  //Shift Register #1 - Digit Enable
  pinMode(clock1, OUTPUT); //orange
  pinMode(latch1, OUTPUT); //green
  pinMode(data1, OUTPUT); //blue

  pinMode(ledpin, OUTPUT); //sets pin "ledpin" as output for an LED
  
  Serial.begin(115200); //Serial baud rate set to Link2fs_Multi config
} //End of setup function



void loop() {
  //Illuminate amber LED (pin13) on arduino on ERROR
  if (errorState == 0) {digitalWrite(ledpin, LOW);}
  if (errorState == 1) {digitalWrite(ledpin, HIGH);}
  if (debug == 1) {debugMode();}
  
  //Only fetch data from serial buffer if data is recieved
  if (Serial.available()) {
    CodeIn = getChar(); //get a character
    if (CodeIn == '=') {EQUAL();} // The first identifier is "="
  } //end of serial check
  
  //Multiplex Display #1
  if (nav1_int >= 10000){segOut(dis1_1, hunds_1, multiplex);} else {segOut(0, 0, multiplex);}
  if (nav1_int >= 1000){segOut(dis1_2, tens_1, multiplex);} else {segOut(0, 0, multiplex);}
  segOut(dis1_3, ones_1, multiplex);
  segOut(dp_1, 0, multiplex);
  segOut(dis1_4, tenth_1, multiplex);
  segOut(dis1_5, hundth_1, multiplex);
  //End of Multiplex Display #1
  
  //Multiplex Display #2
  if (nav1sb_int >= 10000){segOut(dis2_1, hunds_2, multiplex);} else {segOut(0, 0, multiplex);}
  if (nav1sb_int >= 1000){segOut(dis2_2, tens_2, multiplex);} else {segOut(0, 0, multiplex);}
  segOut(dis2_3, ones_2, multiplex);
  segOut(dp_2, 0, multiplex);
  segOut(dis2_4, tenth_2, multiplex);
  segOut(dis2_5, hundth_2, multiplex);
  //End of Multiplex Display #2
  
} //end of main loop



// GET A CHARACTER FROM SERIAL BUFFER
char getChar() // Get a character from the serial buffer
{
  while(Serial.available() == 0); // wait for data
  return((char)Serial.read()); // Thanks Doug
} //End of getChar function



/* If serial data is read from the serial buffer that contains the "=" tag, 
the following code in the "EQUAL" void will execute. */
void EQUAL(){    // The first identifier was "="
CodeIn = getChar(); // Get another character



  // NAV1 Frequency - The following section of code grabs the serial data with tag "=E" and stores each individual digit as an integer.
  switch(CodeIn){case 'E': String nav1_1, nav1_2, nav1_3, nav1_4, nav1_5, nav1_dp; nav1_1 = ""; nav1_1 += getChar(); hunds_1 = nav1_1.toInt(); nav1_2 = ""; nav1_2 += getChar(); tens_1 = nav1_2.toInt(); nav1_3 = ""; nav1_3 += getChar(); ones_1 = nav1_3.toInt(); nav1_dp = ""; nav1_dp += getChar(); nav1_4 = ""; nav1_4 += getChar(); tenth_1 = nav1_4.toInt(); nav1_5 = ""; nav1_5 += getChar(); hundth_1 = nav1_5.toInt(); String nav1_string = ""; nav1_string += nav1_1; nav1_string += nav1_2; nav1_string += nav1_3; nav1_string += nav1_4; nav1_string += nav1_5; nav1_int = nav1_string.toInt(); break;} //End of "NAV1 Frequency" function

  // NAV1 Standby - The following section of code grabs the serial data with tag "=F" and stores each individual digit as an integer.                                                     */
  switch(CodeIn){case 'F': String nav1sb_1, nav1sb_2, nav1sb_3, nav1sb_4, nav1sb_5, nav1sb_dp; nav1sb_1 = ""; nav1sb_1 += getChar(); hunds_2 = nav1sb_1.toInt(); nav1sb_2 = ""; nav1sb_2 += getChar(); tens_2 = nav1sb_2.toInt(); nav1sb_3 = ""; nav1sb_3 += getChar(); ones_2 = nav1sb_3.toInt(); nav1sb_dp = ""; nav1sb_dp += getChar(); nav1sb_4 = ""; nav1sb_4 += getChar(); tenth_2 = nav1sb_4.toInt(); nav1sb_5 = ""; nav1sb_5 += getChar(); hundth_2 = nav1sb_5.toInt(); String nav1sb_string = ""; nav1sb_string += nav1sb_1; nav1sb_string += nav1sb_2; nav1sb_string += nav1sb_3; nav1sb_string += nav1sb_4; nav1sb_string += nav1sb_5; nav1sb_int = nav1sb_string.toInt(); break;} //End of "NAV1 Standby" function
} //End of LESSTHAN function



/* *********************************** SEGMENT OUT *********************************** */
/* This function sends a 16-bit integer to a shift register that drives a 7-segment    */
/* display driver while multiplexing the display.                                      */
/*                                                                                     */
/* EXAMPLE: segOut(digit, value, multiplex);                                           */
/*                                                                                     */
/* *********************************************************************************** */
void segOut(unsigned int dig, unsigned int val, unsigned int mplx){unsigned int shift = ((val<<12)+dig); byte val2, val3; if (shift >= 256){val2 = (shift>>8); val3 = (shift);} if (shift <= 255){val2 = 0; val3 = shift;} digitalWrite(latch1, LOW); shiftOut(data1, clock1,MSBFIRST, val2); shiftOut(data1, clock1,MSBFIRST, val3); digitalWrite(latch1, HIGH); delay (mplx);} //End of segOut function



void debugMode(){} //End of debugMode function
