#! /usr/bin/python


import os
import time


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
os.system("sudo /script/BeagleBoneLED.py &")

time.sleep(2)

#BeagleBone's communications code
#os.system("sudo ~/data/communication_data.py &")

