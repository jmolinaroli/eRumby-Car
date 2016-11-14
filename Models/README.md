#Car Model

On the car there is:
* Arduino Mega x1
* Beaglebone x1
* Encoder x4
* GPS x1
* IMU x1
* Microduino x4
* Radio Receiver x1
* Stearing Motor x1
* Traction Motor x1
* ZigBee x1


![alt tag] (https://github.com/jmolinaroli/eRumby-Car/blob/master/Models/Img/boards_model.png)


Model for eRumby with all electric buses and signals interfaces:
* I2C protocol between Microduino – Arduino Mega – IMU with Arduino Mega as Master;
* serial communications between Arduino Mega and Beaglebone;
* radio communications between remote controller and car receiver;
* WiFi communications between Beaglebone board and PC;
* wireless communications over ZigBee between Beaglebone board and PC.


![alt tag] (https://github.com/jmolinaroli/eRumby-Car/blob/master/Models/Img/connections_model.png)
