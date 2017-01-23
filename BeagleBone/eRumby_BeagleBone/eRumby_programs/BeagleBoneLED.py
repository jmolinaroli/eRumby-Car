#! /usr/bin/python

##############################
# BeagleBoneLED.py
# Script to control BeagleBone's LEDs
# On BeagleBone we have 4 LEDs:
#  - usr0
#  - usr1
#  - usr2
#  - usr3
#
# LED OFF
# echo 0 > /sys/devices/platform/leds/leds/beaglebone\:green\:usr0/brightness 
#
# LED ON
# echo 1 > /sys/devices/platform/leds/leds/beaglebone\:green\:usr0/brightness 
##############################

import os
import time

def led(status,number):
    os.system("echo %s > /sys/devices/platform/leds/leds/beaglebone\:green\:usr%s/brightness" %(status,number))

def blink(s0,s1,s2,s3):
    led(s0,0)
    led(s1,1)
    led(s2,2)
    led(s3,3)
    
def stair():
    #stair from 3 to 0    
    
    blink(0,0,0,1)
    time.sleep(0.05)
    blink(0,0,1,0)
    time.sleep(0.05)
    blink(0,1,0,0)
    time.sleep(0.05)
    blink(1,0,0,0)
    time.sleep(0.05)
    blink(1,0,0,0)
    time.sleep(0.05)
    blink(0,1,0,0)
    time.sleep(0.05)
    blink(0,0,1,0)
    time.sleep(0.05)
    blink(0,0,0,1)
    time.sleep(0.05)
  

counter = 0
while (counter<2):
    blink(0,0,0,0)
    time.sleep(0.1)
    blink(1,1,1,1)
    time.sleep(0.1)
    blink(0,0,0,0)
    time.sleep(0.1)
    blink(1,1,1,1)
    time.sleep(0.1)
    blink(0,0,0,0)
    time.sleep(0.1)
    blink(1,1,1,1)
    time.sleep(0.1)
    time.sleep(0.2)
    stair()
    counter=counter+1
    time.sleep(0.3)
    
blink(0,0,0,0)
time.sleep(0.1)
blink(1,1,1,1)
time.sleep(0.1)
blink(0,0,0,0)
time.sleep(0.1)
blink(1,1,1,1)
time.sleep(0.1)
blink(0,0,0,0)
time.sleep(0.1)
blink(1,1,1,1)
time.sleep(0.1)
blink(0,0,0,0)
time.sleep(0.1)
  
while True:
    
    a = 0
    while (a < 2) :
        stair()
        a = a+1
    
    blink(0,0,0,0)
    time.sleep(0.1)
    time.sleep(0.3)
    
    #j morse code
    blink(1,0,0,0)  #dot
    time.sleep(0.05)
    blink(0,0,0,0)
    time.sleep(0.1)
    blink(1,0,0,0)  #line
    time.sleep(0.2)
    blink(0,0,0,0)
    time.sleep(0.1)
    blink(1,0,0,0)  #line
    time.sleep(0.2)
    blink(0,0,0,0)
    time.sleep(0.1)
    blink(1,0,0,0)  #line
    time.sleep(0.2)
    blink(0,0,0,0)
    time.sleep(0.1)
    
    time.sleep(0.3)
    
    #m morse code
    blink(1,0,0,0)  #line
    time.sleep(0.2)
    blink(0,0,0,0)
    time.sleep(0.1)
    blink(1,0,0,0)  #line
    time.sleep(0.2)
    blink(0,0,0,0)
    time.sleep(0.1)
    
    time.sleep(0.2)
    
