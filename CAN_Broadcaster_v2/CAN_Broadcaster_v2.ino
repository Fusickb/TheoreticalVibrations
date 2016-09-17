#include <i2c_t3.h> // the I2C library that replaces Wire.h for the Teensy 3.2
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

#include <FlexCAN.h>
#include <EEPROM.h>

#define PS0Pin      2
#define resetPin    22
#define IMUIntPin   6
#define STPin       17
#define ledPin      13

/* This driver reads raw data from the BNO055

   Connections
   ===========
   Connect SCL to analog 5
   Connect SDA to analog 4
   Connect VDD to 3.3V DC
   Connect GROUND to common ground

   History
   =======
   2015/MAR/03  - First release (KTOWN)
*/

/* Set the delay between fresh samples */
elapsedMillis canTimer;
elapsedMillis SerialOutputTimer;
elapsedMillis ledTimer;
elapsedMillis timer;

int8_t xhigh = 0;
int8_t xlow = 0;
int8_t whole = 0;
int8_t decimal = 0;
int8_t something = 0;
int8_t somethingMore = 0;
double actual = 0;

boolean ledState = false;

FlexCAN CANbus(250000);
static CAN_message_t txCANmsg,rxCANmsg;

Adafruit_BNO055 bno = Adafruit_BNO055();

/**************************************************************************/
/*
    Arduino setup function (automatically called at startup)
*/
/**************************************************************************/
void setup(void)
{
   CANbus.begin();

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
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("Orientation Sensor Raw Data Test"); Serial.println("");

  /* Initialise the sensor */
  if(!bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }

  delay(1000);

  /* Display the current temperature */
  int8_t temp = bno.getTemp();
  Serial.print("Current Temperature: ");
  Serial.print(temp);
  Serial.println(" C");
  Serial.println("");

  bno.setExtCrystalUse(true);

  Serial.println("Calibration status values: 0=uncalibrated, 3=fully calibrated");

  
}

/**************************************************************************/
/*
    Arduino loop function, called once 'setup' is complete (your own code
    should go here)
*/
/**************************************************************************/
void loop(void)
{
    imu::Vector<3> acceleration = bno.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
    imu::Vector<3> angularVelocity = bno.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);

 /* if ( canTimer >= 10){  //broadcasts messages every 10ms or 100Hz frequency
    canTimer = 0;
    txCANmsg.id=0x700;         //Sets ID
    txCANmsg.len=8;            //Sets Length
    txCANmsg.ext=1;

    actual = acceleration.x();
    Serial.println(actual);
    
    txCANmsg.buf[0]= getDecimalNumber(actual);     //Sets each byte to a value   //dec num accelX  
    txCANmsg.buf[1]= getWholeNumber(actual);                                   //whole num accelX
    txCANmsg.buf[2]= getDecimalNumber(acceleration.y());                                   //dec num accelY
    txCANmsg.buf[3]= getWholeNumber(acceleration.y());                                   //whole num accelY
    txCANmsg.buf[4]= getDecimalNumber(acceleration.z());                                   //dec num accelZ
    txCANmsg.buf[5]= getWholeNumber(acceleration.z());                                   //whole num accelZ
    txCANmsg.buf[6]= getDecimalNumber(angularVelocity.x());                                   //dec num angularVelX
    txCANmsg.buf[7]= getWholeNumber(angularVelocity.x());                                   //whole num angularVelX

    CANbus.write(txCANmsg);  //Writes message to bus
  } //end if*/

  while (CANbus.read(rxCANmsg)){   //reads CAN bus
    uint32_t ID = rxCANmsg.id;  //32 unsigned integer that contains ID of the received message
    byte DLC = rxCANmsg.len;    // byte that stores the length of received message

    
    if ( ID == 0x18FEF100 || ID ==0x700) { //if the ID has FEF1 in byte 1 and byte 2
      Serial.print("\n");
      Serial.print(ID,HEX);                //print the ID in hex
      Serial.print(" ");
      Serial.print(DLC,HEX);               //print the length in hex
      for (int i = 0; i < DLC; i++){
        Serial.print(" ");
        Serial.print(rxCANmsg.buf[i],HEX);  //print each component in hex
      } //end for
    } //end if

    if(ID ==0x700){
      something = rxCANmsg.buf[1];
      somethingMore = rxCANmsg.buf[0];
      Serial.print(something);
      Serial.print(".");
      Serial.print(somethingMore);
      
    }
  } //end while
}

 int8_t getWholeNumber(double data){ //gets whole number portion of data
    whole = data;
    return whole;
  }

  int8_t getDecimalNumber(double data){ //gets decimal portion of number and rounds it
    if(data >= 0){
      decimal = (data-whole)*100 + 0.5; //adding 0.5 to 7.5 makes it 8, and 0.3 + 0.5 = 0.8 which knocks off the decimal point
      }
     else{
      decimal = (data-whole)*100 - 0.5; //-7.5 subtracting makes it -8
     }
    return decimal;
  }

  double getActualValue(int8_t whole, int8_t decimal){ //gets the actual value of the split whole and decimal parts for checking
    actual = whole+(decimal/100.);
    return actual;
  }

