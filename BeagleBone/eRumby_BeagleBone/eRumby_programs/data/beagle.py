#! /usr/bin/python

import commands
import os
import serial
import time
from datetime import datetime

#write to a file
def write_file(file_name,text):
    d = open(file_name,'a')
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
    
#header
def header(file_name):
    now = datetime.now()
    date_name = now.strftime("%Y%m%d_%H%M%S")
    write_file(file_name,date_name)

#BB-UART2
serS2 = serial.Serial ("/dev/ttyS2",timeout=0.1)        #Open named port 
serS2.baudrate = 9600                       #Set baud rate to 9600

#BB-UART4
serS4 = serial.Serial ("/dev/ttyS4",timeout=0.2)        #Open named port 
serS4.baudrate = 9600                       #Set baud rate to 9600

#BB-UART5
serS5 = serial.Serial ("/dev/ttyS5",timeout=0.2)        #Open named port 
serS5.baudrate = 9600                       #Set baud rate to 9600

#date time
now = datetime.now()
date_name_start = now.strftime("%Y%m%d_%H%M%S")

header('data.dat')
header('%s_data.dat' % (date_name_start))

read_var = 'a'

while True:
    
    dataS2 = serS2.read()
    
    
    if (str(dataS2) == str('n')):
        now = datetime.now()
        date_name_new = now.strftime("%Y%m%d_%H%M%S")
        header('%s_data.dat' % (date_name_new))
        read_var = 'n'
        

    dataS4 = serS4.read()
    dataS5 = serS5.read()
            
    #dataS4 = 'dataS4'
    #dataS5 = 'dataS5'
            
    data_sensors = str(dataS4) + str(dataS5) + str(dataS2)
            
    serS2.write(data_sensors)
    write_file('data.dat', data_sensors)
    write_file('%s_data.dat' % (date_name_start), data_sensors)
    
    if (str(read_var) == str('n')):
        write_file('%s_data.dat' % (date_name_new), data_sensors)
