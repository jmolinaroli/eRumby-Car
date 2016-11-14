#include <Wire.h> 
/*****************************************************************/ 
/*********** USER SETUP AREA! Set your options here! *************/ 
/*****************************************************************/ 
 
// HARDWARE OPTIONS 
/*****************************************************************/ 
// Select your hardware here by uncommenting one line! 
#define HW__VERSION_CODE 10724 // SparkFun "9DOF Sensor Stick" version "SEN-10724" (HMC5883L magnetometer) 
 
// SENSOR CALIBRATION 
/*****************************************************************/ 
// How to calibrate? Read the tutorial at http://dev.qu.tu-berlin.de/projects/sf-razor-9dof-ahrs 
// Put MIN/MAX and OFFSET readings for your board here! 
// Accelerometer 
// "accel x,y,z (min/max) = X_MIN/X_MAX  Y_MIN/Y_MAX  Z_MIN/Z_MAX" 
/*#define ACCEL_X_MIN ((float) -270) 
#define ACCEL_X_MAX ((float) 281) 
#define ACCEL_Y_MIN ((float) -274) 
#define ACCEL_Y_MAX ((float) 309) 
#define ACCEL_Z_MIN ((float) -260) 
#define ACCEL_Z_MAX ((float) 276)*/ 
#define ACCEL_X_MIN ((float) -265) 
#define ACCEL_X_MAX ((float) 276) 
#define ACCEL_Y_MIN ((float) -256) 
#define ACCEL_Y_MAX ((float) 270) 
#define ACCEL_Z_MIN ((float) -256) 
#define ACCEL_Z_MAX ((float) 265) 
 
// Magnetometer (standard calibration mode) 
// "magn x,y,z (min/max) = X_MIN/X_MAX  Y_MIN/Y_MAX  Z_MIN/Z_MAX" 
/*#define MAGN_X_MIN ((float) -455) 
#define MAGN_X_MAX ((float) 784) 
#define MAGN_Y_MIN ((float) -655) 
#define MAGN_Y_MAX ((float) 516) 
#define MAGN_Z_MIN ((float) -631) 
#define MAGN_Z_MAX ((float) 579)*/ 
 
// Magnetometer (extended calibration mode) 
// Uncommend to use extended magnetometer calibration (compensates hard & soft iron errors) 
/*#define CALIBRATION__MAGN_USE_EXTENDED true 
const float magn_ellipsoid_center[3] = {59.9438, -7.74673, 13.9676}; 
const float magn_ellipsoid_transform[3][3] = {{0.858316, 0.0149421, -0.0390376}, {0.0149421, 0.880720, 0.0247007}, { -0.0390376, 0.0247007, 0.985644}};*/ 
#define CALIBRATION__MAGN_USE_EXTENDED true 
const float magn_ellipsoid_center[3] = {21.9899, 31.0239, -21.4518}; 
const float magn_ellipsoid_transform[3][3] = {{0.815770, 0.0101451, -0.0143001}, {0.0101451, 0.855180, -0.000794402}, { -0.0143001, -0.000794402, 0.998873}}; 
// Gyroscope 
// "gyro x,y,z (current/average) = .../OFFSET_X  .../OFFSET_Y  .../OFFSET_Z 
/*#define GYRO_AVERAGE_OFFSET_X ((float) 40.68) 
#define GYRO_AVERAGE_OFFSET_Y ((float) -35.50) 
#define GYRO_AVERAGE_OFFSET_Z ((float) 26.62)*/ 
#define GYRO_AVERAGE_OFFSET_X ((float) 39.56) 
#define GYRO_AVERAGE_OFFSET_Y ((float) -35.84) 
#define GYRO_AVERAGE_OFFSET_Z ((float) 27.21) 
 
// DEBUG OPTIONS 
/*****************************************************************/ 
// When set to true, gyro drift correction will not be applied 
#define DEBUG__NO_DRIFT_CORRECTION false 
/*****************************************************************/ 
/****************** END OF USER SETUP AREA!  *********************/ 
/*****************************************************************/ 
// Check if hardware version code is defined 
#ifndef HW__VERSION_CODE 
// Generate compile error 
#error YOU HAVE TO SELECT THE HARDWARE YOU ARE USING! See "HARDWARE OPTIONS" in "USER SETUP AREA" at top of Razor_AHRS.ino! 
#endif 
 
// Sensor calibration scale and offset values 
#define ACCEL_X_OFFSET ((ACCEL_X_MIN + ACCEL_X_MAX) / 2.0f) 
#define ACCEL_Y_OFFSET ((ACCEL_Y_MIN + ACCEL_Y_MAX) / 2.0f) 
#define ACCEL_Z_OFFSET ((ACCEL_Z_MIN + ACCEL_Z_MAX) / 2.0f) 
#define ACCEL_X_SCALE (GRAVITY / (ACCEL_X_MAX - ACCEL_X_OFFSET)) 
#define ACCEL_Y_SCALE (GRAVITY / (ACCEL_Y_MAX - ACCEL_Y_OFFSET)) 
