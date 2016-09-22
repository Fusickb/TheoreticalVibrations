#include <FlexCAN.h>                                                    // Includes Arduino's FlexCAN library. For reference this is available at https://github.com/teachop/FlexCAN_Library

#define ledPin  13                                                      // Defines the pin 13 as the ledPin

FlexCAN J1939bus(250000);                                               // Defines the J1939 CANBus to operate at a baudrate of 250,000.
static CAN_message_t rxmsg;                                             // Defines the received CAN message as rxmsg

elapsedMillis ledTimer;                                                 // Sets a variable that counts up in milliseconds to blink the Led

boolean ledState = false;                                               // Sets a boolean variable to false (light is off)

void setup() {
 // put your setup code here, to run once:
 J1939bus.begin();                                                      // Initializes the J1939bus.
 Serial.begin(115200);                                                  // Intializes the serial output at a baud rate of 115200.

 pinMode(ledPin, OUTPUT);                                               // Sets the ledPin to output mode.
}

void loop() {
  // put your main code here, to run repeatedly:
                                 
   while ( J1939bus.read(rxmsg) ) {                                      // While the Bus is receiving a message, read the message and 
   uint32_t ID = rxmsg.id;                                              // Create an ID that is unsigned and 32 bits long and makes this known as rxmsg.id which is a property of the rxmsg.
   byte DLC = rxmsg.len;                                                // Sets DLC to the rxmsg.len (the length of the transmitted message in bytes)
 
   // This led flash proves that the Teensy is getting CAN traffic.
   if (ledTimer >= 1000){                                               // When the timer reaches 1 second
    ledTimer = 0;                                                       // Reset the timer to 0
    ledState = !ledState;                                               // Switch the variable for the ledState to the opposite value
    digitalWrite(ledPin,ledState);                                      // Write that change to the pin
   }
       
   if ( ID > 0x700 && ID < 0x724 ){                                     // if the ID of the message is 701 to 723
     Serial.print(ID,HEX);                                              // print the ID in HEX,
     Serial.print(" ");                                                 // print a space,
     Serial.print(DLC,HEX);                                             // print the length of the message in HEX,
     for (int i = 0; i<DLC; i++){                                       // For each byte of the message,
       Serial.print(" ");                                               // Print a space, 
       Serial.print(rxmsg.buf[i],HEX);                                  // then print that portion of the message in HEX and repeat for all portions of the message
     } //end for
     Serial.println();                                                  // Print a new line.
     
   }// end if
   
  } //end while
  
}//end loop
