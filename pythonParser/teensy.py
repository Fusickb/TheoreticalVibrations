# -*- coding: utf-8 -*-
"""
Created on Wed Sep 21 09:48:33 2016

@author: njr602
"""


#**************************************************************************
def readCANFile(fileObject, delimiter=',', eol='\n'):
    "Reads a file containing CAN data in a predefined format and parses that data to a different, predefined format"
    data = []
    
    fileObject.seek(0)
    rawFile = fileObject.readlines()
    for line in rawFile:
        dataRow = line.strip(eol).split(delimiter)
        data.append(dataRow)
        
    return(data)

def CANtoVal(lsb, msb, valType):
    "Converts the Teensy's raw register values (for a single quantity) to a single scaled value of a specified type"
    valType=valType.strip().replace(' ','').lower() #clean up the valType variable
    
    scaleFactor = {             #defin the allowed val types 
    "raw":1,                        #and how they shoul be scaled
    "acceleration":100.0,
    "angularvelocity":900.0,
    "linearacceleration":100.0,
    "eulerangle":16.0,
    "gravity":100.0
    }
    
    short = msb.zfill(2)+lsb.zfill(2)   #make a short from the 2 bytes
    numBits = 16                        #this short must have 16 bits

    rawVal = int(short,16)              #get the register value
    if rawVal >= (1<<(numBits-1)):      #do 2s complement if negative
        rawVal = rawVal-(1<<numBits)
        
    return(rawVal/scaleFactor[valType]) #scale the register value and return
    
    
def parseCANFrame0(dataRow):   #This parses an addresses frame 0
    "Takes a row of raw, CAN data and converts it to a row of human readable data"
    dateStamp = dataRow[0]
    timeStamp = dataRow[1]  
    unixTime = dataRow[2] 
    senderID = dataRow[3]
    dataLen = dataRow[4]
    accelXlsb = dataRow[5]
    accelXmsb = dataRow[6]  
    accelYlsb = dataRow[7]
    accelYmsb = dataRow[8]
    accelZlsb = dataRow[9]
    accelZmsb = dataRow[10]
    angVelXlsb = dataRow[11]
    angVelXmsb = dataRow[12]

    accelX = str( CANtoVal(accelXlsb, accelXmsb, 'acceleration') )
    accelY = str( CANtoVal(accelYlsb, accelYmsb, 'acceleration') )
    accelZ = str( CANtoVal(accelZlsb, accelZmsb, 'acceleration') )
    angVelX = str( CANtoVal(angVelXlsb,angVelXmsb, 'angular velocity') )
    
    output = list((
                  dateStamp,
                  timeStamp,
                  unixTime,
                  senderID,
                  dataLen,
                  accelX,
                  accelY,
                  accelZ,
                  angVelX
                  ))
    return(output)
    
    
def parseCANFrame1(dataRow):   #This parses an addresses frame 0
    "Takes a row of raw, CAN data and converts it to a row of human readable data"
    dateStamp = dataRow[0]
    timeStamp = dataRow[1]  
    unixTime = dataRow[2] 
    senderID = dataRow[3]
    dataLen = dataRow[4]
    angVelYlsb = dataRow[5]
    angVelYmsb = dataRow[6]  
    angVelZlsb = dataRow[7]
    angVelZmsb = dataRow[8]
    eulerXlsb = dataRow[9]
    eulerXmsb = dataRow[10]
    eulerYlsb = dataRow[11]
    eulerYmsb = dataRow[12]

    angVelY = str( CANtoVal(angVelYlsb, angVelYmsb, 'angular velocity') )
    angVelZ = str( CANtoVal(angVelZlsb, angVelZmsb, 'angular velocity') )
    eulerX = str( CANtoVal(eulerXlsb, eulerXmsb, 'euler angle') )
    eulerY = str( CANtoVal(eulerYlsb, eulerYmsb, 'euler angle') )
    
    output = list((
                  dateStamp,
                  timeStamp,
                  unixTime,
                  senderID,
                  dataLen,
                  angVelY,
                  angVelZ,
                  eulerX,
                  eulerY
                  ))
    return(output)
    
    
def parseCANFrame2(dataRow):   #This parses an addresses frame 0
    "Takes a row of raw, CAN data and converts it to a row of human readable data"
    dateStamp = dataRow[0]
    timeStamp = dataRow[1] 
    unixTime = dataRow[2]  
    senderID = dataRow[3]
    dataLen = dataRow[4]
    eulerZlsb = dataRow[5]
    eulerZmsb = dataRow[6]  
    linAccelXlsb = dataRow[7]
    linAccelXmsb = dataRow[8]
    linAccelYlsb = dataRow[9]
    linAccelYmsb = dataRow[10]
    linAccelZlsb = dataRow[11]
    linAccelZmsb = dataRow[12]

    eulerZ = str( CANtoVal(eulerZlsb, eulerZmsb, 'euler angle') )
    linAccelX = str( CANtoVal(linAccelXlsb, linAccelXmsb, 'linear acceleration') )
    linAccelY = str( CANtoVal(linAccelYlsb, linAccelYmsb, 'linear acceleration') )
    linAccelZ = str( CANtoVal(linAccelZlsb, linAccelZmsb, 'linear acceleration') )
    
    output = list((
                  dateStamp,
                  timeStamp,
                  unixTime,
                  senderID,
                  dataLen,
                  eulerZ,
                  linAccelX,
                  linAccelY,
                  linAccelZ
                  ))
    return(output)

    
def parseCANFrame3(dataRow):   #This parses an addresses frame 0
    "Takes a row of raw, CAN data and converts it to a row of human readable data"
    dateStamp = dataRow[0]
    timeStamp = dataRow[1]  
    unixTime = dataRow[2] 
    senderID = dataRow[3]
    dataLen = dataRow[4]
    gravityXlsb = dataRow[5]
    gravityXmsb = dataRow[6]  
    gravityYlsb = dataRow[7]
    gravityYmsb = dataRow[8]
    gravityZlsb = dataRow[9]
    gravityZmsb = dataRow[10]
    blanklsb = dataRow[11]
    blankmsb = dataRow[12]

    gravityX = str( CANtoVal(gravityXlsb, gravityXmsb, 'gravity') )
    gravityY = str( CANtoVal(gravityYlsb, gravityYmsb, 'gravity') )
    gravityZ = str( CANtoVal(gravityZlsb, gravityZmsb, 'gravity') )
    blank = blankmsb+blanklsb
    
    output = list((
                  dateStamp,
                  timeStamp,
                  unixTime,
                  senderID,
                  dataLen,
                  gravityX,
                  gravityY,
                  gravityZ,
                  blank
                  ))
    return(output)