// This code is designed to read the voltage output of a String Pot and broadcast the vaule onto the CAN bus.
// Initially created by Dave, Modified by Blake and Nate
// Created for Dr. Daily's Theoretical Vibration Class, Fall 2016
// Ram Truck Shock Testing



#include <FlexCAN.h>

#define analogPin   20
#define ledPin      13

FlexCAN CANbus(250000);
static CAN_message_t txmsg;

elapsedMillis canTimer;

int16_t stringPot = 0;
int8_t stringPotLSB = 0;
int8_t stringPotMSB = 0;

boolean ledState = false;

void setup() {
  // put your setup code here, to run once:
  
  CANbus.begin ();

}


void loop() {
  // put your main code here, to run repeatedly:
  
  if (canTimer >= 10){ 
    stringPot = analogRead(analogPin);
    stringPotLSB = stringPot; //stores last 8 bits when casting 16 bit int to 8 bit int
    stringPotMSB = stringPot >> 8; //shifts 8 bits and stores the first 8 bits
    
    canTimer = 0;
    txmsg.id = 0x788;                                                    // Create a sendable message with the CAN id 0x788, Front String Pot
    txmsg.len = 2;                                                       // that has a length of 2 bytes,
    txmsg.ext = 1;                                                       // that also has an extension (?) of 1.

    txmsg.buf[0] = stringPotLSB;
    txmsg.buf[1] = stringPotMSB;

    ledState = !ledState;
    digitalWrite(ledPin,ledState);
    
   
    CANbus.write(txmsg);                                                    // Send this message out on the CAN bus.

  }



  

}
