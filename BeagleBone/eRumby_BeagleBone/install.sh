#!/bin/sh

# This script configure debian for the eRumby car


# Get absolute path from which this script file was executed
# (Could be changed to "pwd -P" to resolve symlinks to their target)
SCRIPT_PATH=$( cd $(dirname $0) ; pwd )

${SCRIPT_PATH}/version.py

echo "Welcome to eRumby's BeagleBone configuration."
read -p " press ENTER" enter
echo ""

#beaglebone configuations
    #change hostname
    read -p " * do you want to change hostname? [Y/N]" yn
    case $yn in
            ([yY][eE][sS]|[yY])                
               sudo nano /etc/hostname
               sudo nano /etc/hosts
               ;;
            *)
               ;;
    esac
    echo ""
    
    #change colors terminal command prompt
    read -p " * do you want to change the colors of the terminal command prompt? [Y/N]" yn
    case $yn in
            ([yY][eE][sS]|[yY])
               read -p "    - uncomment #force_color_prompt=yes [press ENTER]" enter
               sudo nano  ~/.bashrc
               ;;
            *)
               ;;
    esac
    echo ""
    
    #ssh welcome message
    echo " * configuring ssh"
    sudo rm /etc/motd
    sudo cp ${SCRIPT_PATH}/eRumby_programs/info.txt /etc/motd
    sudo chmod 777 /etc/motd
    
    sudo rm /etc/issue
    sudo cp ${SCRIPT_PATH}/eRumby_programs/info.txt /etc/issue
    sudo chmod 777 /etc/issue
    
    sudo rm /etc/issue.net
    sudo cp ${SCRIPT_PATH}/eRumby_programs/info.txt /etc/issue.net
    sudo chmod 777 /etc/issue.net
    
    read -p "   do you want to change ssh password? [Y/N]" yn
    case $yn in
            ([yY][eE][sS]|[yY])                
               echo  "    [default password is: temppwd]"
               passwd
               ;;
            *)
               ;;
    esac
    echo "   complete."
    echo ""

#install erumby daemon
    echo " * installing eRumby daemon programs:"
    
    echo "   - copying file in /etc/init.d/erumby"
    sudo cp ${SCRIPT_PATH}/eRumby_programs/erumby /etc/init.d/erumby
    sudo chmod 777 /etc/init.d/erumby
    
    echo "   - copying file in /usr/local/sbin/erumbymain.sh"
    sudo cp ${SCRIPT_PATH}/eRumby_programs/erumbymain.sh /usr/local/sbin/erumbymain.sh
    sudo chmod 777 /usr/local/sbin/erumbymain.sh
    
    echo "   - copying file in /usr/local/sbin/erumbyoff.sh"
    sudo cp ${SCRIPT_PATH}/eRumby_programs/erumbyoff.sh /usr/local/sbin/erumbyoff.sh
    sudo chmod 777 /usr/local/sbin/erumbyoff.sh
    
    echo "   - making directory /script"
    sudo mkdir /script
    
    echo "   - copying file in /script/BeagleBoneMAIN.py"
    sudo cp ${SCRIPT_PATH}/eRumby_programs/BeagleBoneMAIN.py /script/BeagleBoneMAIN.py
    sudo chmod 777 /script/BeagleBoneMAIN.py
    
    echo "   - copying file in /script/BeagleBoneOff.py"
    sudo cp ${SCRIPT_PATH}/eRumby_programs/BeagleBoneOff.py /script/BeagleBoneOff.py
    sudo chmod 777 /script/BeagleBoneOff.py
    
    echo "   - copying file in /script/BeagleBoneLED.py"
    sudo cp ${SCRIPT_PATH}/eRumby_programs/BeagleBoneLED.py /script/BeagleBoneLED.py
    sudo chmod 777 /script/BeagleBoneLED.py
    
    sudo insserv erumby
    #daemon reload
    sudo systemctl daemon-reload
    
    echo "   eRumby daemon installation complete."
    echo ""
    
    
    
    
    
    
#end
read -p "   do you want to reboot [Y/N]" yn
    case $yn in
            ([yY][eE][sS]|[yY])                
               sudo reboot
               ;;
            *)
               ;;
    esac

    
