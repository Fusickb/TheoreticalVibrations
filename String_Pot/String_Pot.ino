// This code is designed to read the voltage output of a String Pot and broadcast the vaule onto the CAN bus.
// Initially created by Dave, Modified by Blake and Nate
// Created for Dr. Daily's Theoretical Vibration Class, Fall 2016
// Ram Truck Shock Testing


//**********************************************************************
//Includes
#include <FlexCAN.h>


//**********************************************************************
//Defines
  //Pines
  #define analogPin   20
  #define ledPin      13

  //Serial
  #define SERIAL_BAUD         9600

  //CAN
  #define id0                 0x788
  #define CAN_BAUD            250000


//**********************************************************************
//Initializations
  //CAN
  FlexCAN CANbus(CAN_BAUD);
  static CAN_message_t txmsg;

  //Timers
  elapsedMillis canTimer;
  elapsedMillis ledTimer;

  //String Pot Analog Variables
  uint16_t stringPot = 0;
  uint8_t stringPotLSB = 0;
  uint8_t stringPotMSB = 0;

boolean ledState = false;


//**********************************************************************
//Setup
void setup() {
  //LED pin
  pinMode(ledPin, OUTPUT);
  
  //Setup Serial
  Serial.begin(SERIAL_BAUD);

  //Setup CAN Bus
  CANbus.begin();

}


//**********************************************************************
//Main
void loop(){

  if(ledTimer >= 500){
    ledTimer = 0;
    ledState = !ledState;
    digitalWrite(ledPin,ledState);
  }
  
  if (canTimer >= 10){ 
    canTimer = 0; //reset the timer
    
    stringPot = analogRead(analogPin);
    stringPotLSB = stringPot; //stores last 8 bits when casting 16 bit int to 8 bit int
    stringPotMSB = stringPot >> 8; //shifts 8 bits and stores the first 8 bits

    //DEBUG: serial printing
    //Serial.print(stringPot);
    //  Serial.print("\t");
    //Serial.print(stringPotLSB,HEX);
    //  Serial.print("\t");
    //Serial.println(stringPotMSB,HEX);

       
    txmsg.id = id0;                                                    // Create a sendable message with the CAN id 0x788, Front String Pot
    txmsg.len = 2;                                                       // that has a length of 2 bytes,
    txmsg.ext = 1;                                                       // that also has an extension (?) of 1.

    txmsg.buf[0] = stringPotLSB;
    txmsg.buf[1] = stringPotMSB;
    CANbus.write(txmsg);                                                    // Send this message out on the CAN bus.
    
  }
}





