#MICRODUINO ENCODERS READER

Based on [arduino-encoder-board](https://github.com/UniTN-Mechatronics/arduino-encoder-board) project.

On the car we have four microduino as slaves. Each one reads the input signal from one encoder, elaborates the collected and sends the values data to the master by I2C protocol.

![alt tag] (https://github.com/jmolinaroli/eRumby-Car/blob/master/Models/Img/encoders_model.png)

For this project (differently from the **arduino-encoder-board** project) we fixed the configuration settings of the microduino boards in **EncoderSlave.cpp**:

```c++
void EncoderSlave::default_settings() {
   settings_u.settings.n = 1;
   settings_u.settings.res = 100.;
   settings_u.settings.lost_pulses_th = 40;
   for(int i = 1; i <= settings_u.settings.n; i++){
	   settings_u.settings.a[i] = 0;
	   settings_u.settings.b[i] = 0;
	   settings_u.settings.x[i] = 0;
	   settings_u.settings.a[0] = CH_A;
	   settings_u.settings.b[0] = CH_B;	
       settings_u.settings.x[0] = CH_X; 
   }
   settings_u.settings.I2C_address = I2C_ADDR;
   settings_u.settings.EEPROM_address = 0x10;
   settings_u.settings.read_index = false;
   settings_u.settings.Speed = true;
   settings_u.settings.speed_th_l = 0;
   settings_u.settings.speed_th_h = 1;
}

```


Where in **EncoderSlave.h** for the encoder on the front right wheel we have: 
```c++
#define CH_A 4
#define CH_B 5
#define CH_X 6
#define I2C_ADDR 0x03
```
for the encoder on the front left wheel:
```c++
#define CH_A 7
#define CH_B 8
#define CH_X 9
#define I2C_ADDR 0x04
```
for the encoder on the rear right wheel:
```c++
#define CH_A 4
#define CH_B 5
#define CH_X 6
#define I2C_ADDR 0x05
```
for the encoder on the rear left wheel:
```c++
#define CH_A 7
#define CH_B 8
#define CH_X 9
#define I2C_ADDR 0x06
```
