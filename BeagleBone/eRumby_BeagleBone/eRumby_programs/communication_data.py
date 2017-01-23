#! /usr/bin/python
#coding: utf-8
import commands
import os
import serial
import time
from datetime import datetime

#write to a file
def write_file(file_name,text):
    d = open('/home/debian/data/'+file_name,'a')
    value = str(text)
    d.write('\n')
    d.write(value)
    d.close

#read from a file
def read_file(file_name):
    r = open('/home/debian/data/'+file_name, 'r')
    file_contents = r.read()
    print (file_contents)
    r.close()

#header
def header(file_name):
    now = datetime.now()
    date_name = now.strftime("%Y%m%d_%H%M%S")
    write_file(file_name,date_name)

#BB-UART2
serS2 = serial.Serial ("/dev/ttyS2",timeout=0.2)        #Open named port 
serS2.baudrate = 9600                      #Set baud rate to 9600

#BB-UART4
serS4 = serial.Serial ("/dev/ttyS4")        #Open named port 
serS4.baudrate = 9600 #115200                       #Set baud rate to 9600

#BB-UART5
serS5 = serial.Serial ("/dev/ttyS5")        #Open named port 
serS5.baudrate = 9600                       #Set baud rate to 9600

#date time
now = datetime.now()
date_name_start = now.strftime("%Y%m%d_%H%M%S")
day = now.strftime('%Y%m%d')

header('%s_data.dat' % (day))

read_var = 'a'

while True:

    dataS2 = serS2.read(1)

    if (str(dataS2) == str('n')):
        now = datetime.now()
        date_name_new = now.strftime("%Y%m%d_%H%M%S")
        header('%s_data.dat' % (date_name_new))
        read_var = 'n'

    dataS4 = serS4.readline()
    dataS5 = serS5.readline()
    
    data_sensors = str(dataS5) + '\t' + str(dataS4) + '\t' +str(dataS2) 
    
    #data_sensors to PC w XBee
    serS2.write(data_sensors)
    #PC input to motors
    serS5.write(dataS2)
    
    #write da file
    write_file('%s_data_imu-gps.dat' % (day), dataS5)
    write_file('%s_data_encoders.dat' % (day), dataS4)
    write_file('%s_data.dat' % (day), data_sensors)
    
    #make new files
    if (str(read_var) == str('n')):
        write_file('%s_data_imu-gps.dat' % (date_name_new), dataS5)
        write_file('%s_data_encoders.dat' % (date_name_new), dataS4)
        write_file('%s_data.dat' % (date_name_new), data_sensors)
