#! /bin/bash

##############################
# BeagleBoneLED.sh
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




while :
    
do

    #stair from usr0 to usr3
    echo 0 > /sys/devices/platform/leds/leds/beaglebone\:green\:usr0/brightness
    echo 0 > /sys/devices/platform/leds/leds/beaglebone\:green\:usr1/brightness
    echo 0 > /sys/devices/platform/leds/leds/beaglebone\:green\:usr2/brightness
    echo 0 > /sys/devices/platform/leds/leds/beaglebone\:green\:usr3/brightness
    sleep 0.1
    echo 1 > /sys/devices/platform/leds/leds/beaglebone\:green\:usr0/brightness
    echo 0 > /sys/devices/platform/leds/leds/beaglebone\:green\:usr1/brightness
    echo 0 > /sys/devices/platform/leds/leds/beaglebone\:green\:usr2/brightness
    echo 0 > /sys/devices/platform/leds/leds/beaglebone\:green\:usr3/brightness
    sleep 0.1
    echo 0 > /sys/devices/platform/leds/leds/beaglebone\:green\:usr0/brightness
    echo 1 > /sys/devices/platform/leds/leds/beaglebone\:green\:usr1/brightness
    echo 0 > /sys/devices/platform/leds/leds/beaglebone\:green\:usr2/brightness
    echo 0 > /sys/devices/platform/leds/leds/beaglebone\:green\:usr3/brightness
    sleep 0.1
    echo 0 > /sys/devices/platform/leds/leds/beaglebone\:green\:usr0/brightness
    echo 0 > /sys/devices/platform/leds/leds/beaglebone\:green\:usr1/brightness
    echo 1 > /sys/devices/platform/leds/leds/beaglebone\:green\:usr2/brightness
    echo 0 > /sys/devices/platform/leds/leds/beaglebone\:green\:usr3/brightness
    sleep 0.1
    echo 0 > /sys/devices/platform/leds/leds/beaglebone\:green\:usr0/brightness
    echo 0 > /sys/devices/platform/leds/leds/beaglebone\:green\:usr1/brightness
    echo 0 > /sys/devices/platform/leds/leds/beaglebone\:green\:usr2/brightness
    echo 1 > /sys/devices/platform/leds/leds/beaglebone\:green\:usr3/brightness
    sleep 0.1
    echo 0 > /sys/devices/platform/leds/leds/beaglebone\:green\:usr0/brightness
    echo 0 > /sys/devices/platform/leds/leds/beaglebone\:green\:usr1/brightness
    echo 0 > /sys/devices/platform/leds/leds/beaglebone\:green\:usr2/brightness
    echo 0 > /sys/devices/platform/leds/leds/beaglebone\:green\:usr3/brightness
    sleep 0.1
    
    
    
    #jm morse code
        echo 0 > /sys/devices/platform/leds/leds/beaglebone\:green\:usr0/brightness
        sleep 0.3
        #j dot 
        echo 1 > /sys/devices/platform/leds/leds/beaglebone\:green\:usr0/brightness
        sleep 0.1
        echo 0 > /sys/devices/platform/leds/leds/beaglebone\:green\:usr0/brightness
        sleep 0.1
        #j line
        echo 1 > /sys/devices/platform/leds/leds/beaglebone\:green\:usr0/brightness
        sleep 0.2
        echo 0 > /sys/devices/platform/leds/leds/beaglebone\:green\:usr0/brightness
        sleep 0.1
        #j line
        echo 1 > /sys/devices/platform/leds/leds/beaglebone\:green\:usr0/brightness
        sleep 0.2
        echo 0 > /sys/devices/platform/leds/leds/beaglebone\:green\:usr0/brightness
        sleep 0.1
        #j line
        echo 1 > /sys/devices/platform/leds/leds/beaglebone\:green\:usr0/brightness
        sleep 0.2
        echo 0 > /sys/devices/platform/leds/leds/beaglebone\:green\:usr0/brightness
        sleep 0.3
    
    
    
    #m
    echo 1 > /sys/devices/platform/leds/leds/beaglebone\:green\:usr0/brightness
    sleep 0.2
    
    echo 0 > /sys/devices/platform/leds/leds/beaglebone\:green\:usr0/brightness
    sleep 0.1
    
    echo 1 > /sys/devices/platform/leds/leds/beaglebone\:green\:usr0/brightness
    sleep 0.2
    
    echo 0 > /sys/devices/platform/leds/leds/beaglebone\:green\:usr0/brightness
    sleep 0.3
    
    #closing code
    echo 0 > /sys/devices/platform/leds/leds/beaglebone\:green\:usr0/brightness
    echo 0 > /sys/devices/platform/leds/leds/beaglebone\:green\:usr1/brightness
    echo 0 > /sys/devices/platform/leds/leds/beaglebone\:green\:usr2/brightness
    echo 0 > /sys/devices/platform/leds/leds/beaglebone\:green\:usr3/brightness
    sleep 0.1
    echo 0 > /sys/devices/platform/leds/leds/beaglebone\:green\:usr0/brightness
    echo 0 > /sys/devices/platform/leds/leds/beaglebone\:green\:usr1/brightness
    echo 0 > /sys/devices/platform/leds/leds/beaglebone\:green\:usr2/brightness
    echo 1 > /sys/devices/platform/leds/leds/beaglebone\:green\:usr3/brightness
    sleep 0.1
    echo 0 > /sys/devices/platform/leds/leds/beaglebone\:green\:usr0/brightness
    echo 0 > /sys/devices/platform/leds/leds/beaglebone\:green\:usr1/brightness
    echo 1 > /sys/devices/platform/leds/leds/beaglebone\:green\:usr2/brightness
    echo 0 > /sys/devices/platform/leds/leds/beaglebone\:green\:usr3/brightness
    sleep 0.1
    echo 0 > /sys/devices/platform/leds/leds/beaglebone\:green\:usr0/brightness
    echo 1 > /sys/devices/platform/leds/leds/beaglebone\:green\:usr1/brightness
    echo 0 > /sys/devices/platform/leds/leds/beaglebone\:green\:usr2/brightness
    echo 0 > /sys/devices/platform/leds/leds/beaglebone\:green\:usr3/brightness
    sleep 0.1
    echo 1 > /sys/devices/platform/leds/leds/beaglebone\:green\:usr0/brightness
    echo 0 > /sys/devices/platform/leds/leds/beaglebone\:green\:usr1/brightness
    echo 0 > /sys/devices/platform/leds/leds/beaglebone\:green\:usr2/brightness
    echo 0 > /sys/devices/platform/leds/leds/beaglebone\:green\:usr3/brightness
    sleep 0.1
    echo 0 > /sys/devices/platform/leds/leds/beaglebone\:green\:usr0/brightness
    echo 0 > /sys/devices/platform/leds/leds/beaglebone\:green\:usr1/brightness
    echo 0 > /sys/devices/platform/leds/leds/beaglebone\:green\:usr2/brightness
    echo 0 > /sys/devices/platform/leds/leds/beaglebone\:green\:usr3/brightness
    sleep 0.1
    
done