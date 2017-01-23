#!/usr/bin/python
#coding: utf-8
import commands
import os
import serial
import time
from datetime import datetime

#header
def header(directory,file_name):
    now = datetime.now()
    date_name = now.strftime("%Y%m%d_%H%M%S")
    value = str(date_name)
    file_name.write('\n')
    file_name.write(value)
    

#BB-UART2
serS2 = serial.Serial ("/dev/ttyS2",timeout=0.05)        #Open named port 
serS2.baudrate = 115200                                 #Set baud rate to 9600

#BB-UART4
serS4 = serial.Serial ("/dev/ttyS4")                    #Open named port 
serS4.baudrate = 115200                                 #Set baud rate to 9600

#BB-UART5
serS5 = serial.Serial ("/dev/ttyS5")                    #Open named port
serS5.baudrate = 115200                                 #Set baud rate to 9600

#date time
now = datetime.now()
date_name_start = now.strftime("%Y%m%d_%H%M%S")
day = now.strftime('%Y%m%d')

dir_data = '/home/debian/data/%s_data.txt' % (day)
dir_data_encoders = '/home/debian/data/%s_data_encoders.txt' % (day)
dir_data_imugps = '/home/debian/data/%s_data_imu-gps.txt' % (day)

file_data = open(dir_data, 'a')
file_encoders = open(dir_data_encoders, 'a')
file_imugps = open(dir_data_imugps, 'a')

header(dir_data,file_data)
header(dir_data_encoders,file_encoders)
header(dir_data_imugps,file_imugps)

read_var = 'a'

time_now = now.second
time_old = now.second

while True:

    dataS2 = serS2.read(1)

    if (str(dataS2) == str('n')):
        now = datetime.now()
        date_name_new = now.strftime("%Y%m%d_%H%M%S")
        header('%s_data.dat' % (date_name_new))
        read_var = 'n'
        serS2.write('\t new files \n')

    dataS4 = serS4.readline()   
    dataS5 = serS5.readline()
    
    data_sensors = '\t' + str(dataS5) + '\t' + '\t' + str(dataS2) + '\t' + str(dataS4)
    data_sensors = data_sensors.translate(None, '\n')
    
    #with open('/home/debian/data/%s_data_encoders.dat' % (day)) as inf:
    #    for line in inf:
    #        parts = line.split() # split line into parts
    #        if len(parts) > 7:   # if at least 2 parts/columns
    #            output = parts[7]  
    #data_output = '\t' + dataS2 + '\t' + output + '\t' + '\t' + dataS4
    
    serS2.write(data_sensors)
    serS5.write(dataS2)
    
    now = datetime.now()
    date_time = now.strftime("%H.%M.%S : ")
    
    file_imugps.write('\n' + date_time + dataS4)
    file_encoders.write('\n' + date_time + dataS5)
    file_data.write('\n' + date_time + data_sensors)
    
    
    #if (str(read_var) == str('n')):
    #    write_file('file_imugps', date_time + dataS4)
    #    write_file('file_encoders', date_time + dataS5)
    #    write_file('file_data', date_time + data_sensors)





