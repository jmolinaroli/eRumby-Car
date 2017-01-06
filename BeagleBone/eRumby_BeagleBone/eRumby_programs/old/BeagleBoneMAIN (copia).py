#! /usr/bin/python


import os
import time

#read info
f = open('info.txt', 'r')
file_contents = f.read()
print (file_contents)
f.close()

#double quick blink
def blink() :
    time.sleep(1)
    
    os.system("echo 0 > /sys/devices/platform/leds/leds/beaglebone\:green\:usr0/brightness")
    os.system("echo 0 > /sys/devices/platform/leds/leds/beaglebone\:green\:usr1/brightness")
    os.system("echo 0 > /sys/devices/platform/leds/leds/beaglebone\:green\:usr2/brightness")
    os.system("echo 0 > /sys/devices/platform/leds/leds/beaglebone\:green\:usr3/brightness")
    time.sleep(1)
    
    os.system("echo 1 > /sys/devices/platform/leds/leds/beaglebone\:green\:usr0/brightness")
    os.system("echo 1 > /sys/devices/platform/leds/leds/beaglebone\:green\:usr1/brightness")
    os.system("echo 1 > /sys/devices/platform/leds/leds/beaglebone\:green\:usr2/brightness")
    os.system("echo 1 > /sys/devices/platform/leds/leds/beaglebone\:green\:usr3/brightness")
    time.sleep(1)
    
    os.system("echo 0 > /sys/devices/platform/leds/leds/beaglebone\:green\:usr0/brightness")
    os.system("echo 0 > /sys/devices/platform/leds/leds/beaglebone\:green\:usr1/brightness")
    os.system("echo 0 > /sys/devices/platform/leds/leds/beaglebone\:green\:usr2/brightness")
    os.system("echo 0 > /sys/devices/platform/leds/leds/beaglebone\:green\:usr3/brightness")
    time.sleep(1)
    
    os.system("echo 1 > /sys/devices/platform/leds/leds/beaglebone\:green\:usr0/brightness")
    os.system("echo 1 > /sys/devices/platform/leds/leds/beaglebone\:green\:usr1/brightness")
    os.system("echo 1 > /sys/devices/platform/leds/leds/beaglebone\:green\:usr2/brightness")
    os.system("echo 1 > /sys/devices/platform/leds/leds/beaglebone\:green\:usr3/brightness")
    time.sleep(1)


#BeagleBone's blinking leds code
os.system("sudo /home/debian/script/BeagleBoneLED.py &")



