#! /usr/bin/python

import commands
import os
import serial
import time
from datetime import datetime

#write to a file
def write_file(file_name,text):
    d = open('data.dat','a')
    value = str(text)
    d.write('\n')
    d.write(value)
    d.close
    
#read from a file
def read_file(file_name):
    r = open('read.txt', 'r')
    file_contents = r.read()
    print (file_contents)
    r.close()

   

#data
now = datetime.now()
date_time = '%s/%s/%s %s:%s' % (now.day, now.month, now.year, now.hour, now.minute)
os.system('echo "%s" > /dev/ttyS2' %(date_time))

#rwrite date/time to data.dat
write_file('data.dat',date_time)

#BB-UART2
counter = 0
serS2 = serial.Serial ("/dev/ttyS2")        #Open named port 
serS2.baudrate = 9600                       #Set baud rate to 9600
while (counter < 10):
    dataS2 = serS2.read()                     #Read characters from serial port$
    write_file('data.dat',dataS2)
    serS2.write('data')                     #Send back
    counter = counter +1 
serS2.close()
data_name = now.strftime("%Y-%m-%d %H:%M")
os.rename( "data.dat", "%s_%s.dat" % (data_name,'file_name'))






