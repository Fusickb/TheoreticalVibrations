#include <FlexCAN.h>                                                    // Includes Arduino's FlexCAN library. For reference this is available at https://github.com/teachop/FlexCAN_Library
#include <kinetis_flexcan.h>                                            // Includes Arduino's Kinetis library.

FlexCAN J1939bus(250000);                                               // Defines the J1939 CANBus to operate at a baudrate of 250,000.
static CAN_message_t txmsg,rxmsg;                                       // Defines the CAN message as having two parts that are defined as (txmsg, rxmsg) for the required CAN_message_t ("sendingmessagename", "receivingmessagename").
unsigned long txCount,rxCount;                                          // Declares txCount and rxCount as unsigned (capable of both positive and negative) and a long (64 bits).
unsigned long txTimer,rxTimer;                                          // Declares txTimer and rxTimer to the same parameters as line 18.

uint32_t currentMillis = 0;                                             // Declares an unsigned 32 bit integer type called 'currentMillis' and sets it to a value of 0. 
uint32_t previousMillis1000 = 0;                                        // Declares an unsigned 32 bit integer type called 'previousMillis1000' and sets it to a value of 0.
uint32_t previousMillis100 = 0;                                         // Declares an unsigned 32 bit integer type called 'previousMillis100' and sets it to a value of 0.
uint32_t previousMillis10 = 0;                                          // Declares an unsigned 32 bit integer type called 'previousMillis10' and sets it to a value of 0. 

elapsedMillis stupid;
byte spd = 0;                                                           // Declares a byte that will hold a speed value for the speedometer.


void setup() {
 // put your setup code here, to run once:
 J1939bus.begin();                                                      // Initializes the J1939bus.
 Serial.begin(115200);                                                  // Intializes the serial output at a baud rate of 115200.

}

void loop() {
  // put your main code here, to run repeatedly:
currentMillis = millis();                                               // Defines currentMillis as the elapsed millis (restarts the clock after every loop).
if(stupid >=10){
  //Serial.println("hi");  
}
                                                                        // This part of the loop is used to send out a message that will be sent out every 1000 milliseconds.
/*   if (currentMillis - previousMillis1000 >= 1000){                     // if the difference between the current time - previousmillis1000 is greater than 1000.
    previousMillis1000 = currentMillis;                                 // make previousMillis1000 equal to the current Millis.
    txmsg.id = 0x18E0FF00;                                              // Create a sendable message with the CAN id 0x18E0FF00,
    Serial.print("Boop");
    txmsg.len = 8;                                                      // that has a length of 8 bytes
    txmsg.ext = 1;                                                      // that also has an extension (?) of 1.
    txmsg.buf[0]= 0x00;                                                 // Makes this message's 1st part be 00.
    for (int i = 1; i < 8; i++){                                         // Loops through 2nd to 8th part and sets it to FF.
      txmsg.buf[i]= 0xFF;
    }// end for
   
    J1939bus.write(txmsg);                                              // Sends this message out on the J1939 bus.
  } //end if 

                                                                        // This part of the loop is used to send out a message that will be sent out every 10 milliseconds.
  if (currentMillis - previousMillis10 >= 10){                          // if the difference between the current time - previousmillis10 is greater than 10.
    previousMillis10 = currentMillis;                                   // make previousMillis10 equal to the current Millis.
    txmsg.id = 0x0CF00400;                                              // Create a sendable message with the CAN id 0x0CF00400
    txmsg.len = 8;                                                      // that has a length of 8 bytes
    txmsg.ext = 1;                                                      // that also has an extension (?) of 1.
    
    for (int i = 0; i < 8; i++){                                        //Loops through the eight bytes and assigns the byte to have the message 00 in hex or 0x00.
      txmsg.buf[i]= 0x00;
    } //end for
    J1939bus.write(txmsg);                                              // Sends this message out on the J1939 bus.
  } //end if

  
                                                                        // This part of the loop is used to send out a message that will be sent out every 100 milliseconds.
  if (currentMillis - previousMillis100 >= 100){                        // if the difference between the current time - perviousmillis100 is greater than 100.
    previousMillis100 = currentMillis;                                  // make previousMillis100 equal to the current Millis.
    spd+=1;                                                             // increases the value of speed by 1.
    txmsg.id = 0x18FEF100;                                              // Create a sendable message with the CAN id 0x18FEF100
    txmsg.len = 8;                                                      // That has a length of 8 bytes
    txmsg.ext = 1;                                                      // That also has an extension (?) of 1.
    for (int i = 1; i < 8; i++){                                        // Makes this message's 1st and 2nd part be 00.
      txmsg.buf[i] = 0x00;
    }// end for
    txmsg.buf[2] = spd;                                                 // Makes the 3rd part equal to the byte spd.
    for (int i = 3; i < 8; i++){                                        // Make the 4th through 8th parts 00.
      txmsg.buf[i]= 0x00;
    }// end for
    J1939bus.write(txmsg);                                              // Sends this message out on the J1939 bus.
    if (spd > 140) spd=0;                                               // if spd gets 140 or greater, it resets it to 0.
  }// end if
  */                                                                      //replace if statement with if(rxmsg.id == 0x18FEF100) if competing with another signal.
   while ( J1939bus.read(rxmsg) ) {                                      // While the Bus is receiving a message, read the message and 
   uint32_t ID = rxmsg.id;                                              // Create an ID that is unsigned and 32 bits long and makes this known as rxmsg.id which is a property of the rxmsg.
   byte DLC = rxmsg.len;                                                // Sets DLC to the rxmsg.len
  // Serial.print("OMG LIKE IT WORKS");    
                                                                        // This is the extractor to be able to read to speed outputs
       
   if ( (ID & 0x00FFFF00) >> 8 == 0xFEF1){                              // if the ID of the message is FEF1 (the J1939 Standard for the Ground speed) then,
     Serial.print(ID,HEX);                                              // print the ID in HEX,
     Serial.print(" ");                                                 // print a space,
     Serial.print(DLC,HEX);                                             // print the length of the message in HEX,
     for (int i = 0; i<DLC; i++){                                       // For each byte of the message,
       Serial.print(" ");                                               // Print a space, 
       Serial.print(rxmsg.buf[i],HEX);                                  // then print that portion of the message and repeat for all portions of the message
     } //end for
     int wholeKPH = rxmsg.buf[2];                                       // Take the third byte in the message (buf[2]), and set that to an integer called wholeKPH.
     if (wholeKPH < 251) {                                              // If the wholeKPH is less than 251,
       int fractionKPH = rxmsg.buf[1];                                  // read the second byte in the message (buf[1]), and set that to an integer called fractionKPF.
       Serial.print(" Speed: ");                                        // Print 'Speed: ' on the serial network.
       Serial.print(wholeKPH);                                          // Print the wholeKPH value on the serial network.
       Serial.print(".");                                               // Print a period. 
       Serial.print(fractionKPH/256);                                   // Print the fractionKPH/256.
       Serial.print("kph, ");                                           // Print 'kph, ' and then,
       uint16_t bigkph = word(wholeKPH,fractionKPH);                    // make an unsigned 16 bit integer, then use the word function to combine wholeKPH with fractionKPH.
       Serial.print(bigkph/256/1.607);                                  // Print a MPH version of the speed output on the serial network.
        
     }// end if
     Serial.println();                                                  // Print a new line.
     
   }// end if
   
  } //end while
  
}//end loop
