
/* 
    This code is in the public domain
    For use with "Link2fs_Multi"
    Jimspage.co.nz
    My thanks to the Guys that gave me snippets of code. 
        
    >>>> You must tick "=E" (NAV1) and "=F" (NAV1 Standby) in Multi. <<<<<<
*/

//Define Variables
int CodeIn; // used on all serial reads
int data1 = 2, clock1 = 3, latch1 = 4; //Used for serial output to shift register IC
//int data2 = 6, clock2 = 7, latch2 = 8; //Used for serial output to shift register IC
//int data3 = 9, clock3 = 10, latch3 = 11; //Used for serial output to shift register IC
int ledpin = 13; //Used for led on pin 13 of arduino



/* SETUP ALL ARDUINO PIN MODES */
void setup() 
{
  pinMode(data1, OUTPUT); //pin x on shift register IC
  pinMode(clock1, OUTPUT); //pin x on shift register IC
  pinMode(latch1, OUTPUT); //pin x on shift register IC
  //pinMode(data2, OUTPUT); //pin x on shift register IC
  //pinMode(clock2, OUTPUT); //pin x on shift register IC
  //pinMode(latch2, OUTPUT); //pin x on shift register IC
  //pinMode(data3, OUTPUT); //pin x on shift register IC
  //pinMode(clock3, OUTPUT); //pin x on shift register IC
  //pinMode(latch3, OUTPUT); //pin x on shift register IC
  pinMode(ledpin, OUTPUT); //sets pin "ledpin" as output for an LED
  pinMode(12, INPUT); //DEBUG
  Serial.begin(115200); //Serial baud rate set to Link2fs_Multi config
} //End of setup void



void loop() {
  int val = digitalRead(12);
  if(val = 1) {digitalWrite(ledpin, HIGH);}
     else (digitalWrite(ledpin, LOW)); //Turns off LED on pin 13 of arduino
  if (Serial.available()) {
    //digitalWrite(ledpin, LOW); //Turns on LED on pin 13 of arduino
    CodeIn = getChar(); //get a character
    if (CodeIn == '=') {EQUAL();} // The first identifier is "="
  } //end of serial check
  //else (digitalWrite(ledpin, HIGH)); //Turns off LED on pin 13 if no serial data present
} //end of void loop



/* GET A CHARACTER FROM SERIAL BUFFER */
char getChar() // Get a character from the serial buffer
{
  while(Serial.available() == 0); // wait for data
  return((char)Serial.read()); // Thanks Doug
} //End of getChar function



/* If serial data is read from the serial buffer that contains the "=" tag, 
the following code in the "EQUAL" void will execute. */
void EQUAL(){    // The first identifier was "="
CodeIn = getChar(); // Get another character



  /* NAV1 RADIO DISPLAY - The following section of code grabs the serial 
  data with tag "=E" and stores each individual digit as an integer. */
  switch(CodeIn) { // Now lets find what to do with it
    case 'E': //Found the second identifier ("E" - NAV1 Radio Display)
      
      //Define variables
      String nav1_1, nav1_2, nav1_3, nav1_4, nav1_5, nav1_dec; //hold string values from serial stream
      
      nav1_1 = "";  
      nav1_1 += getChar(); //grab 1st character
      int nav1_dig1 = nav1_1.toInt(); //convert to integer
      nav1_dig1 = (nav1_dig1<<4)+1; //shift value to upper 4bits of Byte
      byte nav1_dig1B = byte(nav1_dig1); //convert to binary byte
      
      nav1_2 = "";
      nav1_2 += getChar(); //grab second character
      int nav1_dig2 = nav1_2.toInt(); //convert to integer
      nav1_dig2 = (nav1_dig2<<4)+15; //shift value to upper 4bits of Byte
      byte nav1_dig2B = byte(nav1_dig2); //convert to binary byte
      
      nav1_3 = "";
      nav1_3 += getChar(); //grab third character
      int nav1_dig3 = nav1_3.toInt(); //convert to integer
      nav1_dig3 = (nav1_dig3<<4)+15; //shift value to upper 4bits of Byte
      byte nav1_dig3B = byte(nav1_dig3); //convert to binary byte
      
      nav1_dec = ""; //grab decimal place holder (".")
      nav1_dec += getChar(); //store in random variable
      
      nav1_4 = "";
      nav1_4 += getChar(); //grab fourth character
      int nav1_dig4 = nav1_4.toInt(); //convert to integer
      nav1_dig4 = (nav1_dig4<<4)+15; //shift value to upper 4bits of Byte
      byte nav1_dig4B = byte(nav1_dig4); //convert to binary byte
      
      nav1_5 = "";
      nav1_5 += getChar(); //grab fifth character
      int nav1_dig5 = nav1_5.toInt(); //convert to integer
      nav1_dig5 = (nav1_dig5<<4)+1; //shift value to upper 4bits of Byte
      byte nav1_dig5B = byte(nav1_dig5); //convert to binary byte
      
      String nav1_string = "";
      nav1_string += nav1_1;
      nav1_string += nav1_2;
      nav1_string += nav1_3;
      nav1_string += nav1_4;
      nav1_string += nav1_5;
      int nav1_int = nav1_string.toInt(); //convert to integer
      double nav1_number = (nav1_int * .01); //store integer as double with correct place value      
      
      /* DEBUG */
      Serial.print("NAV1:    ");
      Serial.print(nav1_dig1B);
      Serial.print("    ");
      Serial.print(nav1_dig2B);
      Serial.print("    ");
      Serial.print(nav1_dig3B);
      Serial.print("    ");
      Serial.print(".    ");
      Serial.print(nav1_dig4B);
      Serial.print("    ");
      Serial.println(nav1_dig5B);
      /* END DEBUG */
      
      
      if(nav1_number >= 100) {
        digitalWrite(latch1, LOW);
        shiftOut(data1, clock1,MSBFIRST, 0); //(datapin, clockpin, data)
        shiftOut(data1, clock1,MSBFIRST, nav1_dig1B); //(datapin, clockpin, data)
        digitalWrite(latch1, HIGH);
        Serial.println(nav1_dig1B);
        delay(500);}
      
      if(nav1_number >= 10) {
        digitalWrite(latch1, LOW);
        shiftOut(data1, clock1,MSBFIRST, 0); //(datapin, clockpin, data)
        shiftOut(data1, clock1,MSBFIRST, nav1_dig2B); //(datapin, clockpin, data)
        digitalWrite(latch1, HIGH);
        Serial.println(nav1_dig1B);
        delay(500);}
        
      if(nav1_number >= 0) {
        digitalWrite(latch1, LOW);
        shiftOut(data1, clock1,MSBFIRST, 0); //(datapin, clockpin, data)
        shiftOut(data1, clock1,MSBFIRST, nav1_dig3B); //(datapin, clockpin, data)
        digitalWrite(latch1, HIGH);
        Serial.println(nav1_dig1B);
        delay(500);}
        
      if(nav1_number >= .01) {
        digitalWrite(latch1, LOW);
        shiftOut(data1, clock1,MSBFIRST, 0); //(datapin, clockpin, data)
        shiftOut(data1, clock1,MSBFIRST, nav1_dig4B); //(datapin, clockpin, data)
        digitalWrite(latch1, HIGH);
        Serial.println(nav1_dig1B);
        delay(500);}
      
      /*if(nav1_number >= .001) {
        digitalWrite(latch3, LOW);
        shiftOut(data3, clock3,MSBFIRST, nav1_dig5B); //(datapin, clockpin, data)
        digitalWrite(latch3, HIGH);
        delay(1000);}*/
            
      break;
      
  } //End of "NAV1 Radio Display" function
  
  
  
/* NAV1 STANDBY DISPLAY - The following section of code grabs the serial 
data with tag "=F" and stores each individual digit as an integer. */      
  switch(CodeIn) { // Now lets find what to do with it
    case 'F': //Found the second identifier ("F" - NAV1 Standby Display)
      
      //Define Variables
      String nav1sb, nav1sb_1, nav1sb_2, nav1sb_3, nav1sb_4, nav1sb_5; //hold string values from serial stream
      
      nav1sb_1 = "";  //Reset string to empty (required for next line)
      nav1sb_1 += getChar(); //grab 1st character
      int nav1sb_dig1 = nav1sb_1.toInt(); //convert to integer
      nav1sb_dig1 = (nav1sb_dig1<<4)+1; //shift value to upper 4bits of Byte
      
      nav1sb_2 = ""; //Reset string to empty (required for next line)
      nav1sb_2 += getChar(); //grab second character
      int nav1sb_dig2 = nav1sb_2.toInt(); //convert to integer
      nav1sb_dig2 = (nav1sb_dig2<<4)+2; //shift value to upper 4bits of Byte
      
      nav1sb_3 = ""; //Reset string to empty (required for next line)
      nav1sb_3 += getChar(); //grab third character
      int nav1sb_dig3 = nav1sb_3.toInt(); //convert to integer
      nav1sb_dig3 = (nav1sb_dig3<<4)+4; //shift value to upper 4bits of Byte
      
      nav1sb = ""; //grab decimal place holder (".")
      nav1sb += getChar(); //store in random variable
      
      nav1sb_4 = ""; //Reset string to empty (required for next line)
      nav1sb_4 += getChar(); //grab fourth character
      int nav1sb_dig4 = nav1sb_4.toInt(); //convert to integer
      nav1sb_dig4 = (nav1sb_dig4<<4)+8; //shift value to upper 4bits of Byte
      
      nav1sb_5 = ""; //Reset string to empty (required for next line)
      nav1sb_5 += getChar(); //grab fifth character
      int nav1sb_dig5 = nav1sb_5.toInt(); //convert to integer
      nav1sb_dig5 = (nav1sb_dig5<<4)+2; //shift value to upper 4bits of Byte
      
      String nav1sb_string = "";
      nav1sb_string += nav1sb_1;
      nav1sb_string += nav1sb_2;
      nav1sb_string += nav1sb_3;
      nav1sb_string += nav1sb_4;
      nav1sb_string += nav1sb_5;
      int nav1sb_int = nav1sb_string.toInt(); //convert to integer
      double nav1sb_number = (nav1sb_int * .01); //store integer as double with correct place value
      
      /* DEBUG */
      Serial.print("NAV1 STANDBY: ");
      Serial.print(nav1sb_dig1);
      Serial.print("    ");
      Serial.print(nav1sb_dig2);
      Serial.print("    ");
      Serial.print(nav1sb_dig3);
      Serial.print("    ");
      Serial.print(".    ");
      Serial.print(nav1sb_dig4);
      Serial.print("    ");
      Serial.println(nav1sb_dig5);
      /* END DEBUG */
      
      /*
      if(nav1sb_number >= 100) {
        digitalWrite(latch2, LOW);
        shiftOut(data2, clock2,MSBFIRST, nav1sb_dig1); //(datapin, clockpin, data)
        digitalWrite(latch2, HIGH);
        delay(500);}
      
      if(nav1sb_number >= 10) {
        digitalWrite(latch2, LOW);
        shiftOut(data2, clock2,MSBFIRST, nav1sb_dig2); //(datapin, clockpin, data)
        digitalWrite(latch2, HIGH);
        delay(500);}
        
      if(nav1sb_number >= 0) {
        digitalWrite(latch2, LOW);
        shiftOut(data2, clock2,MSBFIRST, nav1sb_dig3); //(datapin, clockpin, data)
        digitalWrite(latch2, HIGH);
        delay(500);}
        
      if(nav1sb_number >= .01) {
        digitalWrite(latch2, LOW);
        shiftOut(data2, clock2,MSBFIRST, nav1sb_dig4); //(datapin, clockpin, data)
        digitalWrite(latch2, HIGH);
        delay(500);}
        
      if(nav1sb_number >= .001) {
        digitalWrite(latch3, LOW);
        shiftOut(data3, clock3,MSBFIRST, nav1sb_dig5); //(datapin, clockpin, data)
        digitalWrite(latch3, HIGH);
        delay(500);}
      */
      
      break;
      
     } //End of "NAV1 Standby Display" function
     
     
     
} //End of EQUALS void
