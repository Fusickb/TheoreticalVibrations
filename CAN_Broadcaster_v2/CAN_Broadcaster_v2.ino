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
#define SERIAL_BAUD 115200

  //CAN Bus
#define CAN_BAUD    250000

  //Teensy ID
#define CAN_ID0     0x701
#define CAN_ID1     0x702
#define CAN_ID2     0x703
#define CAN_ID3     0x704

//*********************************************************************************
//Instantiations
/* Set the delay between fresh samples */
elapsedMillis canTimer;

FlexCAN CANbus(250000);
static CAN_message_t txCANmsg;

  //Make Accelerometer Object
Adafruit_BNO055 bno = Adafruit_BNO055();

//LED Blinking
int msgCounter = 0;     //Counts the number of messages sent
int msgRollover = 50;  //msgCounter resets to 1 after when greater than this
boolean ledState = false; //this will toggle on every rollover
boolean SIXTEEN_G_MODE = false;


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
  if(SIXTEEN_G_MODE){
    bno.begin(bno.OPERATION_MODE_CONFIG);
    delay(1000);
    bno.write8(bno.BNO055_PAGE_ID_ADDR, 1); //switch to memory page 1
    byte currentAccelConfig = bno.read8(bno.BNO055_ACCEL_DATA_X_LSB_ADDR) & 0xFC; //read the 6 msb of the accel config
    byte newAccelConfig = currentAccelConfig | 0x3; //update the config to have 16G acceleration
    bno.write8(bno.BNO055_ACCEL_DATA_X_LSB_ADDR, newAccelConfig); //write the update to the config register
    bno.setMode(bno.OPERATION_MODE_AMG);
  }
  else{
    bno.begin();
    delay(1000);
  }
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
    imu::Vector<3> linearAcceleration = bno.getShort(Adafruit_BNO055::VECTOR_LINEARACCEL);
    imu::Vector<3> eulerAngle = bno.getShort(Adafruit_BNO055::VECTOR_EULER);
    imu::Vector<3> gravity = bno.getShort(Adafruit_BNO055::VECTOR_GRAVITY);

    int8_t accelXLSB = (int8_t)((int16_t)acceleration.x());     //acceleration x lsb  
    int8_t accelXMSB = (int8_t)((int16_t)acceleration.x()>>8);  //acceleration x msb
    int8_t accelYLSB = (int8_t)((int16_t)acceleration.y());     //acceleration y lsb
    int8_t accelYMSB = (int8_t)((int16_t)acceleration.y()>>8);  //acceleration y msb                               
    int8_t accelZLSB = (int8_t)((int16_t)acceleration.z());     //acceleration z lsb              
    int8_t accelZMSB = (int8_t)((int16_t)acceleration.z()>>8);  //accelertion z msb            
    int8_t angVelXLSB = (int8_t)((int16_t)angularVelocity.x());  //angular velocity x lsb             
    int8_t angVelXMSB = (int8_t)((int16_t)angularVelocity.x()>>8);  //angular velocity x msb   
    int8_t angVelYLSB = (int8_t)((int16_t)angularVelocity.y()); // angular velocity y lsb
    int8_t angVelYMSB = (int8_t)((int16_t)angularVelocity.y()>>8); //angular velocity y msb
    int8_t angVelZLSB = (int8_t)((int16_t)angularVelocity.z()); //angular velocity z lsb
    int8_t angVelZMSB = (int8_t)((int16_t)angularVelocity.z()>>8); //angular velocity z msb
    int8_t linAccelXLSB =(int8_t)((int16_t)linearAcceleration.x());     //linear acceleration x lsb  
    int8_t linAccelXMSB =(int8_t)((int16_t)linearAcceleration.x()>>8);     //linear acceleration x msb  
    int8_t linAccelYLSB =(int8_t)((int16_t)linearAcceleration.y());     //linear acceleration y lsb  
    int8_t linAccelYMSB = (int8_t)((int16_t)linearAcceleration.y()>>8);     //linear acceleration y msb  
    int8_t linAccelZLSB =(int8_t)((int16_t)linearAcceleration.z());     //linear acceleration z lsb  
    int8_t linAccelZMSB =(int8_t)((int16_t)linearAcceleration.z()>>8);     //linear acceleration z msb
    int8_t eulerAngleXLSB = (int8_t)((int16_t)eulerAngle.x());     //euler angle x lsb   
    int8_t eulerAngleXMSB =(int8_t)((int16_t)eulerAngle.x()>>8);     //euler angle x msb  
    int8_t eulerAngleYLSB =(int8_t)((int16_t)eulerAngle.y());     //euler angle y lsb  
    int8_t eulerAngleYMSB = (int8_t)((int16_t)eulerAngle.y()>>8);     //euler angle y msb  
    int8_t eulerAngleZLSB =(int8_t)((int16_t)eulerAngle.z());     //euler angle z lsb  
    int8_t eulerAngleZMSB = (int8_t)((int16_t)eulerAngle.z()>>8);     //euler angle z msb
    int8_t gravityXLSB =(int8_t)((int16_t)gravity.x());     //gravity x lsb  
    int8_t gravityXMSB = (int8_t)((int16_t)gravity.x()>>8);     //gravity x msb  
    int8_t gravityYLSB = (int8_t)((int16_t)gravity.y());     //gravity y lsb  
    int8_t gravityYMSB =(int8_t)((int16_t)gravity.y()>>8);     //gravity y msb  
    int8_t gravityZLSB =(int8_t)((int16_t)gravity.z());     //gravity z lsb  
    int8_t gravityZMSB =(int8_t)((int16_t)gravity.z()>>8);     //gravity z msb   
    
    //Frame 0
    txCANmsg.id=CAN_ID0;         //Sets ID
    txCANmsg.len=8;            //Sets Length
    txCANmsg.ext=1;
    
    txCANmsg.buf[0]= accelXLSB;     
    txCANmsg.buf[1]= accelXMSB; 
    txCANmsg.buf[2]= accelYLSB;     
    txCANmsg.buf[3]= accelYMSB;                                 
    txCANmsg.buf[4]= accelZLSB;                   
    txCANmsg.buf[5]= accelZMSB;              
    txCANmsg.buf[6]= angVelXLSB;               
    txCANmsg.buf[7]= angVelXMSB;  

    CANbus.write(txCANmsg);

    txCANmsg.id = CAN_ID1;

    txCANmsg.buf[0]= angVelYLSB;       
    txCANmsg.buf[1]= angVelYMSB;  
    txCANmsg.buf[2]= angVelZLSB;     
    txCANmsg.buf[3]= angVelZMSB;                               
    txCANmsg.buf[4]= eulerAngleXLSB;            
    txCANmsg.buf[5]= eulerAngleXMSB;       
    txCANmsg.buf[6]= eulerAngleYLSB;             
    txCANmsg.buf[7]= eulerAngleYMSB;

     CANbus.write(txCANmsg);

     txCANmsg.id = CAN_ID2;

     txCANmsg.buf[0] = eulerAngleZLSB;
     txCANmsg.buf[1] = eulerAngleZMSB;
     txCANmsg.buf[2] = linAccelXLSB;
     txCANmsg.buf[3] = linAccelXMSB;
     txCANmsg.buf[4] = linAccelYLSB;
     txCANmsg.buf[5] = linAccelYMSB;
     txCANmsg.buf[6] = linAccelZLSB;
     txCANmsg.buf[7] = linAccelZMSB;

     CANbus.write(txCANmsg);

     txCANmsg.id = CAN_ID3;

     txCANmsg.buf[0] = gravityXLSB;
     txCANmsg.buf[1] = gravityXMSB;
     txCANmsg.buf[2] = gravityYLSB;
     txCANmsg.buf[3] = gravityYMSB;
     txCANmsg.buf[4] = gravityZLSB;
     txCANmsg.buf[5] = gravityZMSB;
     txCANmsg.buf[6] = 0xDE;
     txCANmsg.buf[7] = 0xAD;

     CANbus.write(txCANmsg);
    
     msgCounter++; //increment the message counter
      
      if(msgCounter>msgRollover){ //If counter is high enough
        msgCounter=1;             //rollover
        ledState = !ledState;     //Toggle LED
        digitalWrite(ledPin,ledState);
        }
    
  } //end if*/

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

