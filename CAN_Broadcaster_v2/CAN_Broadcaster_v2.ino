//*********************************************************************************
//Includes
#include <i2c_t3.h> // the I2C library that replaces Wire.h for the Teensy 3.2
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

#include <FlexCAN.h>
#include <EEPROM.h>


//*********************************************************************************
//Defines

  //Pins
#define PS0Pin      2
#define resetPin    22
#define IMUIntPin   6
#define STPin       17
#define ledPin      13

  //Serial Connection
#define SERIAL_BAUD 9600

  //CAN Bus
#define CAN_BAUD    250000

  //Teensy ID
#define CAN_ID0     0x701


//*********************************************************************************
//Instantiations
/* Set the delay between fresh samples */
elapsedMillis canTimer;

FlexCAN CANbus(250000);
static CAN_message_t txCANmsg,rxCANmsg;

  //Make Accelerometer Object
Adafruit_BNO055 bno = Adafruit_BNO055();

//LED Blinking
int msgCounter = 0;     //Counts the number of messages sent
int msgRollover = 50;  //msgCounter resets to 1 after when greater than this
boolean ledState = false; //this will toggle on every rollover


//*********************************************************************************
//Setup
void setup(void)
{
  //Setup CAN Bus
  CANbus.begin();

  //Setup pins
  pinMode(ledPin, OUTPUT);
  pinMode(PS0Pin,OUTPUT);
  digitalWrite(PS0Pin,LOW);
  pinMode(resetPin,OUTPUT);
  digitalWrite(resetPin,LOW);
  pinMode(STPin,OUTPUT);
  digitalWrite(STPin,LOW);
  pinMode(IMUIntPin,INPUT);
  delay(20);
  digitalWrite(resetPin,HIGH);

  //Setup Serial
  Serial.begin(115200);
  delay(1000);

  //Initialize the sensor
  bno.begin();
  delay(1000);
  bno.setExtCrystalUse(true);

}


//*********************************************************************************
//Main
void loop(void)
{
  if ( canTimer >= 10){  //broadcasts messages every 10ms or 100Hz frequency
    canTimer = 0;              //Zero the timer

    //Use Rake's custom functions to read the raw, unscaled register values
      //These will have to be scaled after they are imported
    imu::Vector<3> acceleration = bno.getShort(Adafruit_BNO055::VECTOR_ACCELEROMETER);
    imu::Vector<3> angularVelocity = bno.getShort(Adafruit_BNO055::VECTOR_GYROSCOPE);

    int8_t accelXLSB = (int8_t)((int16_t)acceleration.x());     //acceleration x lsb  
    int8_t accelXMSB = (int8_t)((int16_t)acceleration.x()>>8);  //acceleration x msb
    int8_t accelYLSB = (int8_t)((int16_t)acceleration.y());     //acceleration y lsb
    int8_t accelYMSB = (int8_t)((int16_t)acceleration.y()>>8);  //acceleration y msb                               
    int8_t accelZLSB = (int8_t)((int16_t)acceleration.z());     //acceleration z lsb              
    int8_t accelZMSB = (int8_t)((int16_t)acceleration.z()>>8);  //accelertion z msb            
    int8_t angVelXLSB = (int8_t)((int16_t)angularVelocity.x());  //angular velocity x lsb             
    int8_t angVelXMSB = (int8_t)((int16_t)angularVelocity.x());  //angular velocity x msb   

    Serial.print(acceleration.x(),HEX);
    Serial.print(accelXLSB,HEX);
    Serial.print(accelXMSB,HEX);

    //Frame 0
    txCANmsg.id=CAN_ID0;         //Sets ID
    txCANmsg.len=8;            //Sets Length
    txCANmsg.ext=1;
    
    txCANmsg.buf[0]= (int8_t)((int16_t)acceleration.x());     //acceleration x lsb  
    txCANmsg.buf[1]= (int8_t)((int16_t)acceleration.x()>>8);  //acceleration x msb
    txCANmsg.buf[2]= (int8_t)((int16_t)acceleration.y());     //acceleration y lsb
    txCANmsg.buf[3]= (int8_t)((int16_t)acceleration.y()>>8);  //acceleration y msb                               
    txCANmsg.buf[4]= (int8_t)((int16_t)acceleration.z());     //acceleration z lsb              
    txCANmsg.buf[5]= (int8_t)((int16_t)acceleration.z()>>8);  //accelertion z msb            
    txCANmsg.buf[6]= (int8_t)((int16_t)angularVelocity.x());  //angular velocity x lsb             
    txCANmsg.buf[7]= (int8_t)((int16_t)angularVelocity.x());  //angular velocity x msb                                 

    if(CANbus.write(txCANmsg)){  //Writes message to bus
      msgCounter++; //increment the message counter
      
      if(msgCounter>msgRollover){ //If counter is high enough
        msgCounter=1;             //rollover
        ledState = !ledState;     //Toggle LED
        digitalWrite(ledPin,ledState);
        }
    }
  } //end if*/

//  while (CANbus.read(rxCANmsg)){   //reads CAN bus
//    uint32_t ID = rxCANmsg.id;  //32 unsigned integer that contains ID of the received message
//    byte DLC = rxCANmsg.len;    // byte that stores the length of received message
//
//    
//    if ( ID == 0x18FEF100 || ID ==0x700) { //if the ID has FEF1 in byte 1 and byte 2
//      Serial.print("\n");
//      Serial.print(ID,HEX);                //print the ID in hex
//      Serial.print(" ");
//      Serial.print(DLC,HEX);               //print the length in hex
//      for (int i = 0; i < DLC; i++){
//        Serial.print(" ");
//        Serial.print(rxCANmsg.buf[i],HEX);  //print each component in hex
//      } //end for
//    } //end if
//
//    if(ID ==0x700){
//      something = rxCANmsg.buf[1];
//      somethingMore = rxCANmsg.buf[0];
//      Serial.print(something);
//      Serial.print(".");
//      Serial.print(somethingMore);
//      
//    }
//    
//  } //end while

} //End (Main) Loop


//*********************************************************************************
//Old Functions
// int8_t getWholeNumber(double data){ //gets whole number portion of data
//    whole = data;
//    return whole;
//  }
//
//  int8_t getDecimalNumber(double data){ //gets decimal portion of number and rounds it
//    if(data >= 0){
//      decimal = (data-whole)*100 + 0.5; //adding 0.5 to 7.5 makes it 8, and 0.3 + 0.5 = 0.8 which knocks off the decimal point
//      }
//     else{
//      decimal = (data-whole)*100 - 0.5; //-7.5 subtracting makes it -8
//     }
//    return decimal;
//  }
//
//  double getActualValue(int8_t whole, int8_t decimal){ //gets the actual value of the split whole and decimal parts for checking
//    actual = whole+(decimal/100.);
//    return actual;
//  }

