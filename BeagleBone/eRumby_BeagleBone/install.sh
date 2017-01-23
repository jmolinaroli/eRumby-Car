#!/bin/sh

# This script configure debian for the eRumby car


# Get absolute path from which this script file was executed
# (Could be changed to "pwd -P" to resolve symlinks to their target)
SCRIPT_PATH=$( cd $(dirname $0) ; pwd )

${SCRIPT_PATH}/version.py

read -p "Welcome to eRumby's BeagleBone configuration. [press ENTER]" enter
echo ""
    
    #ssh welcome message
    read -p " * do you want to configure ssh? [Y/N] " yn
    case $yn in
            ([yY][eE][sS]|[yY])
                sudo rm /etc/motd
                sudo cp ${SCRIPT_PATH}/eRumby_programs/info.txt /etc/motd
                sudo chmod 777 /etc/motd
                
                sudo rm /etc/issue
                sudo cp ${SCRIPT_PATH}/eRumby_programs/info.txt /etc/issue
                sudo chmod 777 /etc/issue
                
                sudo rm /etc/issue.net
                sudo cp ${SCRIPT_PATH}/eRumby_programs/info.txt /etc/issue.net
                sudo chmod 777 /etc/issue.net
                echo "   complete."
                ;;
            *)
                ;;
    esac
    echo ""
    read -p " * do you want to change ssh password? [Y/N] " yn
    case $yn in
            ([yY][eE][sS]|[yY])                
                echo  "    [default password is: temppwd]"
                echo ""
                passwd
                echo "   complete."
                ;;
            *)
                ;;
    esac
    echo ""

#install erumby daemon
    read -p " * do you want to install the eRumby daemon programs? [Y/N] " yn
    case $yn in
            ([yY][eE][sS]|[yY])
                echo "    - copying file /etc/init.d/erumby"
                sudo rm /etc/init.d/erumby
                sudo cp ${SCRIPT_PATH}/eRumby_programs/erumby /etc/init.d/erumby
                sudo chmod 777 /etc/init.d/erumby
                
                echo "    - copying file /usr/local/sbin/erumbymain.sh"
                sudo rm /usr/local/sbin/erumbymain.sh
                sudo cp ${SCRIPT_PATH}/eRumby_programs/erumbymain.sh /usr/local/sbin/erumbymain.sh
                sudo chmod 777 /usr/local/sbin/erumbymain.sh
                
                echo "    - copying file /usr/local/sbin/erumbyoff.sh"
                sudo rm /usr/local/sbin/erumbyoff.sh
                sudo cp ${SCRIPT_PATH}/eRumby_programs/erumbyoff.sh /usr/local/sbin/erumbyoff.sh
                sudo chmod 777 /usr/local/sbin/erumbyoff.sh
                
                echo "    - making directory /script"
                #sudo rm -r /script
                sudo mkdir /script
                
                echo "    - making directory /home/debian/data"
                #sudo rm -r /script
                sudo mkdir /home/debian/data/
                
                echo "    - copying file /script/BeagleBoneMAIN.py"
                sudo cp ${SCRIPT_PATH}/eRumby_programs/BeagleBoneMAIN.py /script/BeagleBoneMAIN.py
                sudo chmod 777 /script/BeagleBoneMAIN.py
                
                echo "    - copying file /script/BeagleBoneOff.py"
                sudo cp ${SCRIPT_PATH}/eRumby_programs/BeagleBoneOff.py /script/BeagleBoneOff.py
                sudo chmod 777 /script/BeagleBoneOff.py
                
                echo "    - copying file /script/BeagleBoneLED.py"
                sudo cp ${SCRIPT_PATH}/eRumby_programs/BeagleBoneLED.py /script/BeagleBoneLED.py
                sudo chmod 777 /script/BeagleBoneLED.py
                
                echo "    - copying file /script/communication_data.py"
                sudo cp ${SCRIPT_PATH}/eRumby_programs/communication_data.py /script/communication_data.py
                sudo chmod 777 /script/communication_data.py
                
                sudo insserv erumby
                #daemon reload
                sudo systemctl daemon-reload
    
                echo "    eRumby daemon installation complete."
                ;;
            *)
                ;;
    esac
    echo ""
    
#install Xbee communications
    read -p " * do you want to install the XBee communications? [Y/N] " yn
    case $yn in
            ([yY][eE][sS]|[yY])
                echo "    - copy inside:" 
                read -p "       'cape_enable=bone_capemgr.enable_partno=BB-UART0,BB-UART1,BB-UART2,BB-UART4,BB-UART5' [press ENTER]" enter
                sudo nano /boot/uEnv.txt
                echo "    XBee communications installation complete."
                ;;
            *)
                ;;
    esac
    echo ""
    
       
#beaglebone configuations   
    #change colors terminal command prompt
    read -p " * do you want to change the colors of the terminal command prompt? [Y/N] " yn
    case $yn in
            ([yY][eE][sS]|[yY])
                read -p "    - uncomment #force_color_prompt=yes [press ENTER]" enter
                sudo nano  ~/.bashrc
                echo "   complete."
                ;;
            *)
                ;;
    esac
    echo ""

    #change hostname
    read -p " * do you want to change hostname? [Y/N] " yn
    case $yn in
            ([yY][eE][sS]|[yY])
                read -p "    - change 'beaglebone' with the new hostname [press ENTER]" enter
                sudo nano /etc/hostname
                sudo nano /etc/hosts
                echo "   complete."
                ;;
            *)
                ;;
    esac
    echo ""
    
    #flash eMMC
    read -p " * do you want to flash the eMMC from the micro-SD card? [Y/N] " yn
    case $yn in
            ([yY][eE][sS]|[yY])
                read -p "    - uncomment the cmdline [press ENTER]" enter
                sudo nano /boot/uEnv.txt
                echo " "
                echo "    - shutdown and power down."
                echo "    - hold down “User Boot” and power up BeagleBone again." 
                ;;
            *)
                ;;
    esac
    echo ""    
    
    
#end
echo ""  
echo "configuration complete."
echo ""

read -p " * do you want to reboot [Y/N] " yn
    case $yn in
            ([yY][eE][sS]|[yY])                
                sudo reboot
                ;;
            *)
                ;;
    esac

    
