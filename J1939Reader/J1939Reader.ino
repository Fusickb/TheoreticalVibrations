#include <FlexCAN.h>                                                    // Includes Arduino's FlexCAN library. For reference this is available at https://github.com/teachop/FlexCAN_Library

#define ledPin  13                                                      // Defines the pin 13 as the ledPin

FlexCAN J1939bus(250000);                                               // Defines the J1939 CANBus to operate at a baudrate of 250,000.
static CAN_message_t rxmsg;                                             // Defines the received CAN message as rxmsg

elapsedMillis ledTimer;                                                 // Sets a variable that counts up in milliseconds to blink the Led

int16_t accelerationX = 0;                                              //These variables will be used for storing the 16 bit integer of the IMU read from CAN
int16_t accelerationY = 0;
int16_t accelerationZ = 0;
int16_t angVelocityX = 0;
int16_t angVelocityY = 0;
int16_t angVelocityZ = 0;
int16_t linearAccelX = 0;
int16_t linearAccelY = 0;
int16_t linearAccelZ = 0;
int16_t eulerAngleX = 0;
int16_t eulerAngleY = 0;
int16_t eulerAngleZ = 0;

float accelerationResolution = 100.0;                                       // These resolutions are used to convert the above variables to actual values (from the data sheet)
int angVelocityResolution = 1;
int linearAccelResolution = 1;
int eulerAngleResolution = 1;

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
       
   if (/* ID > 0x700 && ID < 0x724*/ ID ==0x701 ){                                     // if the ID of the message is 701 to 723
     Serial.print(ID,HEX);                                              // print the ID in HEX,
     Serial.print(" ");                                                 // print a space,
     Serial.print(DLC,HEX);                                             // print the length of the message in HEX,
     for (int i = 0; i<DLC; i++){                                       // For each byte of the message,
       Serial.print(" ");                                               // Print a space, 
       Serial.print(rxmsg.buf[i],HEX);                                  // then print that portion of the message in HEX and repeat for all portions of the message
     } //end for

//nested if statements only run if the ID is in the range from the above if statement, and prints results based on ID number
 
/*     if (ID == 0x701 || ID == 0x704 || ID == 0x707 || ID == 0x710 || ID == 0x713 || ID == 0x716 || ID == 0x719){ //these IDs will store accelerationX,Y,Z, and linearVelocityX. This will print the values.
     Serial.print(" ");
     accelerationX = (accelerationX | rxmsg.buf[1]) << 8; // stores most significant byte
     Serial.print(accelerationX);
     Serial.print(" ");
     accelerationX = accelerationX | rxmsg.buf[0]; //stores least significant byte
     Serial.print(accelerationX);
     Serial.print(" ");
     accelerationX /= accelerationResolution;  //resolves the actual value by dividing by the resolution
     Serial.print("AccelX: ");
     Serial.print(accelerationX); 
     Serial.print(" ");

     accelerationY = (accelerationY | rxmsg.buf[3]) << 8;
     accelerationY = accelerationY | rxmsg.buf[2];
     accelerationY /= accelerationResolution;
     Serial.print("AccelY: ");
     Serial.print(accelerationY);
     Serial.print(" ");

     accelerationZ = (accelerationZ | rxmsg.buf[5]) << 8;
     accelerationZ = accelerationZ | rxmsg.buf[4];
     accelerationZ /= accelerationResolution;
     Serial.print("AccelZ: ");
     Serial.print(accelerationZ);
     Serial.print(" ");
     
     angVelocityX = (angVelocityX | rxmsg.buf[7]) << 8;
     angVelocityX = angVelocityX | rxmsg.buf[6];
     angVelocityX /= angVelocityResolution;
     Serial.print("AngVelX: ");
     Serial.print(angVelocityX);
     Serial.print(" ");
     } //end if

     else if (ID == 0x702 || ID == 0x705 || ID == 0x708 || ID == 0x711 || ID == 0x714 || ID == 0x717 || ID == 0x720){//these IDs will store linearVelocityY,Z, linearAccelX,Y. This will print the values.
     Serial.print(" ");
     angVelocityY = (angVelocityY | rxmsg.buf[1]) << 8;
     angVelocityY = angVelocityY | rxmsg.buf[0];
     angVelocityY /= angVelocityResolution;
     Serial.print("AngVelY: ");
     Serial.print(angVelocityY); 
     Serial.print(" ");

     angVelocityZ = (angVelocityZ | rxmsg.buf[3]) << 8;
     angVelocityZ = angVelocityZ | rxmsg.buf[2];
     angVelocityZ /= angVelocityResolution;
     Serial.print("AngVelZ: ");
     Serial.print(angVelocityZ);
     Serial.print(" ");

     linearAccelX = (linearAccelX | rxmsg.buf[5]) << 8;
     linearAccelX = linearAccelX| rxmsg.buf[4];
     linearAccelX /= linearAccelResolution;
     Serial.print("LinearAccelX: ");
     Serial.print(accelerationZ);
     Serial.print(" ");
     
     linearAccelY = (linearAccelY | rxmsg.buf[7]) << 8;
     linearAccelY = linearAccelY | rxmsg.buf[6];
     linearAccelY /= linearAccelResolution;
     Serial.print("LinearAccelY: ");
     Serial.print(linearAccelY);
     Serial.print(" ");
     } //end else if

     else if (ID == 0x703 || ID == 0x706 || ID == 0x709 || ID == 0x712 || ID == 0x715 || ID == 0x718 || ID == 0x721){//these IDs will store linearAccelZ, eulerAngleX,Y,Z. This will print the values.
     Serial.print(" ");
     linearAccelZ = (linearAccelZ | rxmsg.buf[1]) << 8;
     linearAccelZ = linearAccelZ | rxmsg.buf[0];
     linearAccelZ /= linearAccelResolution;
     Serial.print("LinearAccelZ: ");
     Serial.print(linearAccelZ); 
     Serial.print(" ");

     eulerAngleX = (eulerAngleX | rxmsg.buf[3]) << 8;
     eulerAngleX = eulerAngleX | rxmsg.buf[2];
     eulerAngleX /= eulerAngleResolution;
     Serial.print("EulerAngleX: ");
     Serial.print(eulerAngleX);
     Serial.print(" ");

     eulerAngleY = (eulerAngleY | rxmsg.buf[5]) << 8;
     eulerAngleY = eulerAngleY| rxmsg.buf[4];
     eulerAngleY /= eulerAngleResolution;
     Serial.print("EulerAngleY: ");
     Serial.print(eulerAngleY);
     Serial.print(" ");
     
     eulerAngleZ = (eulerAngleZ | rxmsg.buf[7]) << 8;
     eulerAngleZ = eulerAngleZ | rxmsg.buf[6];
     eulerAngleZ /= eulerAngleResolution;
     Serial.print("EulerAngleZ: ");
     Serial.print(eulerAngleZ);
     Serial.print(" ");
     } //end else if */
     
     Serial.println();                                                  // Print a new line.
     
   }// end if
   
  } //end while
  
}//end loop
