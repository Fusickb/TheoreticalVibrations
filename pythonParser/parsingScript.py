# -*- coding: utf-8 -*-
"""
Created on Wed Sep 21 05:44:33 2016

@author: Nathanael Rake
"""

#**************************************************************************
#Imports
import sys
import teensy


#**************************************************************************
#Definitions

id0group = [701, 705, 709, 713, 717, 721, 725]
id1group = [702, 706, 710, 714, 718, 722, 726]
id2group = [703, 707, 711, 715, 719, 723, 727]
id3group = [704, 708, 712, 716, 720, 724, 728]

inputFile = "C:/Users/Nathanael/Desktop/DATA27.csv"
inputDelimiter = ','
inputEOL = '\n'

outputFile0 = "C:/Users/Nathanael/Desktop/output-data0.csv"
outputFile1 = "C:/Users/Nathanael/Desktop/output-data1.csv"
outputFile2 = "C:/Users/Nathanael/Desktop/output-data2.csv"
outputFile3 = "C:/Users/Nathanael/Desktop/output-data3.csv"
outputDelimiter = ','
outputEOL = '\n'

#**************************************************************************
#main

#create file objects
try:
    fin = open(inputFile)
except OSError:
    sys.exit("Failed to open "+inputFile)
    
try:
    fout0 = open(outputFile0,"w+")
    fout1 = open(outputFile1,"w+")
    fout2 = open(outputFile2,"w+")
    fout3 = open(outputFile3,"w+")
except OSError:
    fin.close()
    sys.exit("Failed to open output")
    
print("Files opened successfully\n")


#read the file and perform preliminary parsing
print("Reading data\n")
rawData = teensy.readCANFile(fin, delimiter=inputDelimiter, eol=inputEOL)
print("Reading completed\n")


#parse the rawData and add it to output data
outputData = []

#header = outputDelimiter.join(rawData[0])+outputEOL
#fout.write(header)
print("Parsing and writing data\n")
for rawRow in rawData[1:]:
    if int(rawRow[3]) in id0group:
        outputRow = teensy.parseCANFrame0(rawRow)
        fout0.write(outputDelimiter.join(outputRow)+outputEOL)
    elif int(rawRow[3]) in id1group:
        outputRow = teensy.parseCANFrame1(rawRow)
        fout1.write(outputDelimiter.join(outputRow)+outputEOL)
    elif int(rawRow[3]) in id2group:
        outputRow = teensy.parseCANFrame2(rawRow)
        fout2.write(outputDelimiter.join(outputRow)+outputEOL)
    elif int(rawRow[3]) in id3group:
        outputRow = teensy.parseCANFrame3(rawRow)
        fout3.write(outputDelimiter.join(outputRow)+outputEOL)
print("Writing completed\n")
    
#Close the files
print("Closing files\n")
fin.close()
fout0.close()
fout1.close()
fout2.close()
fout3.close()