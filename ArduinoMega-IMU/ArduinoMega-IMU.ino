//Receiver Code
#include "SoftwareSerial.h"
#include "inttypes.h"

void PrintData();
void IMUupdate();

void setup() {
  Serial.begin(9600);
  //Serial.flush();
  // Init sensors
  delay(50);  // Give sensors enough time to start
  I2C_Init();
  Accel_Init();
  Magn_Init();
  Gyro_Init();
  // Read sensors, init DCM algorithm
  delay(20);  // Give sensors enough time to collect data
  reset_sensor_fusion();
}

void loop() {

    PrintData();
    IMUupdate();
    delay(50);
}


