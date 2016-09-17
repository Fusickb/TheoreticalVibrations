// This code is designed to read the voltage output of a String Pot and broadcast the vaule onto the CAN bus.
// Initially created by Dave, Modified by Blake and Nate
// Created for Dr. Daily's Theoretical Vibration Class, Fall 2016
// Ram Truck Shock Testing



#include <FlexCAN.h>

FlexCAN CANbus(5000000);
static CAN_message_t txmsg,rxmsg;


uint32_t currentMillis = 0;                                             // Declares an unsigned 32 bit integer type called 'currentMillis' and sets it to a value of 0. 
uint32_t previousMillis10 = 0;                                          // Declares an unsigned 32 bit integer type called 'currentMillis' and sets it to a value of 0.

elapsedMillis timer;

int analogPin = 3;
float StringPot = 0;



int8_t IntegerValue = 0;
int8_t DecimalValue = 0;


void setup() {
  // put your setup code here, to run once:
  
  CAN.begin ();

}


void loop() {
  // put your main code here, to run repeatedly:

  StringPot = analogRead(analogPin);

  IntegerValue = StringPot;
  DecimalValue = (StringPot - IntegerValue) * 100
  


  (timer >= 10){ 
   timer = 0;
   txmsg.id = 0x788;                                                    // Create a sendable message with the CAN id 0x788, Front String Pot
   txmsg.len = 2;                                                       // that has a length of 2 bytes,
   txmsg.ext = 1;                                                       // that also has an extension (?) of 1.
   txmsg.buf[0]= IntegerValue;                                          // Make this message's 1st part be Integer Value,
   txmsg.buf[1] = DecimalValue;                                         // 2nd part be Decimal Value,
   
   CAN.write(txmsg);                                                    // Send this message out on the CAN bus.

  }



  

}
