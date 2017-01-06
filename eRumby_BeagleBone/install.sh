#!/bin/sh

# This script configure debian for the eRumby car


# Get absolute path from which this script file was executed
# (Could be changed to "pwd -P" to resolve symlinks to their target)
SCRIPT_PATH=$( cd $(dirname $0) ; pwd )


#install erumby daemon 
sudo cp ${SCRIPT_PATH}/eRumby_programs/erumby /etc/init.d/erumby
sudo chmod 777 /etc/init.d/erumby

sudo cp ${SCRIPT_PATH}/eRumby_programs/erumbymain.sh /usr/local/sbin/erumbymain.sh
sudo chmod 777 /usr/local/sbin/erumbymain.sh

sudo cp ${SCRIPT_PATH}/eRumby_programs/erumbyoff.sh /usr/local/sbin/erumbyoff.sh
sudo chmod 777 /usr/local/sbin/erumbyoff.sh

sudo mkdir ~/script
sudo cp ${SCRIPT_PATH}/eRumby_programs/BeagleBoneMAIN.py ~/script/BeagleBoneMAIN.py
sudo chmod 777 ~/script/BeagleBoneMAIN.py

sudo cp ${SCRIPT_PATH}/eRumby_programs/BeagleBoneOff.py ~/script/BeagleBoneOff.py
sudo chmod 777 ~/script/BeagleBoneOff.py

sudo cp ${SCRIPT_PATH}/eRumby_programs/BeagleBoneLED.py ~/script/BeagleBoneLED.py
sudo chmod 777 ~/script/BeagleBoneLED.py