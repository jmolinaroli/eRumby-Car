#include <stdio.h>
#include "serialib.h"
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>

using namespace std;


#if defined (_WIN32) || defined( _WIN64)
#define         ARDUINO_MEGA            "COM1"                               // COM1 for windows
#define         BEAGLEBONE              "COM2"                               // COM1 for windows
#endif

#ifdef __linux__
#define         ARDUINO_MEGA             "/dev/ttyS4"                         // ttyS0 for linux
#define         BEAGLEBONE               "/dev/ttyS2"                         // ttyS0 for linux
#endif



// STEARING - DUTY CYCLE VALUES
#define DUTY_STEARING_DX 1052
#define DUTY_STEARING_IDLE 1476
#define DUTY_STEARING_SX 1890

// Previous values in 16 bit
// Nominal Value = 6881
// Boundaries = +/- 27% nominal value
#define DUTY_SERVO_DX 5024//4915  
#define DUTY_SERVO_MIDDLE 6881
#define DUTY_SERVO_SX 8738//8847 

// MOTOR - DUTY CYCLE VALUES
#define DUTY_MOTOR_MAX 2032
#define DUTY_MOTOR_IDLE 1500
#define DUTY_MOTOR_IDLE_SAFE 1340
#define DUTY_MOTOR_MIN 1000

// Previous values in 16 bit
// Nominal Value = 7012
// Boundaries = +/- 20% nominal value
#define DUTY_ESC_MAX 8412//8064
#define DUTY_ESC_MAX_SAFE 7200
#define DUTY_ESC_IDLE 7010
#define DUTY_ESC_MIN 5608//5960

//MODE - DUTY CYCLE VALUES
#define DUTY_MODE_HIGH 2024          
#define DUTY_MODE_MIDDLE 1504        
#define DUTY_MODE_LOW 980    



typedef struct Sensor{
    float encoder[4];
    float acc[3];
    float gyro[3];
    float orientation[3];
    short int gps[2];
}sensor_t;

typedef struct Actuator{
    short int traction_motor;
    short int steering_motor;
}actuator_t;

actuator_t actuator = {.traction_motor = DUTY_ESC_IDLE,.steering_motor = DUTY_SERVO_MIDDLE};

typedef union {
    sensor_t sensor;
    char sensor_data_byte[56];
}sensor_data_t;

typedef union {
    actuator_t actuator;
    char actuator_data_byte[4];
}actuator_data_t;

sensor_data_t sensor_data;
// Declare and Initialize for actuator data
actuator_data_t actuator_data = {actuator};


int main()
{

	serialib LS_AM;                                                                 // Object of the serialib class
	int Ret_AM;                                                                     // Used for return values
	serialib LS_BB;                                                                 // Object of the serialib class
	int Ret_BB;                                                                     // Used for return values
	char val[1];
	

	//int i = 0;
	std::ofstream out;
	out.open("/home/debian/data/data.txt", std::ios::app);
    clock_t Start = clock();
	std::string str = ("time \tenc_fl \tenc_fr \tenc_rl \tenc_rr \tacc_x \tacc_y \tacc_z \tgyro_x \tgyro_y \tgyro_z \tori_x \tori_y \tori_z \tgps_x \tgps_y \n");
	out << str;

	// Open serial ports
	Ret_AM=LS_AM.Open(ARDUINO_MEGA,115200);                                         // Open serial link at 115200 bauds
	Ret_BB=LS_BB.Open(BEAGLEBONE,115200);                                           // Open serial link at 115200 bauds

	if (Ret_AM!=1) {                                                                   // If an error occured...
		printf ("Error while opening port. Permission problem ?\n");                // ... display a message ...
		return Ret_AM;                                                                 // ... quit the application
	} else {
		printf ("Serial port opened successfully !\n");
	}

	while (1) {
        usleep(30);

		// Write the AT command on the serial port
		//char val = 'a';
		Ret_BB=LS_BB.Read(val,1,1);
		Ret_AM=LS_AM.Write(&val,1);                                            	    // Send the command on the serial port

        printf(val);
                                                                                    // The final character of the string must be a line feed ('\n')
		if (Ret_BB>0){                                                                 // If a string has been read from, print the string
            
            Ret_BB=LS_BB.Write("prova",6);	
		}
		//else {
		    //printf ("TimeOut reached. No data received !\n");                     // If not, print a message.
		//}
	
        Ret_AM=LS_AM.Read(sensor_data.sensor_data_byte,56,5000);			        // Read a maximum of 128 characters with a timeout of 5 seconds
        out << clock() ;
        out << "\t" << sensor_data.sensor.encoder[0] << "\t" <<  sensor_data.sensor.encoder[1] << "\t" << sensor_data.sensor.encoder[2] << "\t" << sensor_data.sensor.encoder[3];
        out << "\t" << sensor_data.sensor.acc[0] << "\t" << sensor_data.sensor.acc[1] << "\t" << sensor_data.sensor.acc[2];
        out << "\t" << sensor_data.sensor.gyro[0] << "\t" << sensor_data.sensor.gyro[1] << "\t" << sensor_data.sensor.gyro[2];
        out << "\t" << sensor_data.sensor.orientation[0] << "\t" << sensor_data.sensor.orientation[1] << "\t" << sensor_data.sensor.orientation[2];
        out << "\t" << sensor_data.sensor.gps[0] << "\t" << sensor_data.sensor.gps[1] << "\n";;

	}

	// Close the connection with the devices
	LS_AM.Close();
	LS_BB.Close();

}

