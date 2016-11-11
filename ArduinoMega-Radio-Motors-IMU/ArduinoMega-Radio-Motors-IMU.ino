#include <EEPROM.h>
#include "PWM.h"
#include "pins_arduino.h"
#include "Arduino.h"
#include "inttypes.h"

#define ISR_HOW ISR_NOBLOCK //ISR_BLOCK  ISR_NOBLOCK

#define STEERING 51   //3° register bit 0x04  
#define MOTOR 52      //2° register bit 0x02
#define MODE 53       //1° register bit 0x01

#define ESC 11
#define SERVO 12

#define MAX_ENCS 4
#define I2C_address 0x02

#define DUTY_MODE_HIGH 2024
#define DUTY_MODE_CENTRAL 1504
#define DUTY_MODE_LOW 980

typedef struct ReceivingData {
  uint32_t pulseWidth;
  uint32_t edgeTime;
  uint16_t duty;
  byte counter;
};

struct data_t{
  long TO_DEG(roll)[MAX_ENCS];
  long TO_DEG(pitch)[MAX_ENCS];
  long TO_DEG(yaw)[MAX_ENCS];
};


volatile static uint8_t PCintLast;
volatile static ReceivingData pinData[3];
char c[100] = {'\0'};

void PrintDataIMU();
void IMUupdate();

//////////////////////////////////////////////////////////////////////////////////////////////////////

void setup() {
  InitTimersSafe();
  Serial.begin(115200);
  configureReceiver(); // Setup receiver pins for pin change interrupts
  Serial.flush();
  SetMotors(ESC,SERVO);
  I2C_Init();
  Accel_Init();
  Magn_Init();
  Gyro_Init();
  delay(20);  // Give sensors enough time to collect data
  reset_sensor_fusion();
  Wire.begin(I2C_address);
  Wire.onRequest(requestEvent);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

void loop() {
  //Invio dati!
  //if (Serial.read() == 'a') {
    cli();
    PrintData();
    PrintDataIMU();
    sei();
  //}
  
  IMUupdate();
  if ((pinData[2].pulseWidth >= DUTY_MODE_CENTRAL - 50) && (pinData[2].pulseWidth <= DUTY_MODE_CENTRAL + 50))
    UpdateMotors();
  if ((pinData[2].pulseWidth >= DUTY_MODE_LOW - 50) && (pinData[2].pulseWidth <= DUTY_MODE_LOW + 50))
    Sicurezza();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

void PrintData() {
  Serial.print(pinData[0].pulseWidth);
  Serial.print('\t');
  Serial.print(pinData[1].pulseWidth);
  Serial.print('\t');
  Serial.print(pinData[2].pulseWidth);
  Serial.print('\n');
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

void configureReceiver() {
  cli();
  PCMSK0 = 0;
  pinMode(STEERING, INPUT_PULLUP);
  pinMode(MOTOR, INPUT_PULLUP);
  pinMode(MODE, INPUT_PULLUP);
  PCMSK0 = 0b00000111;
  PCICR = 0b00000001; // PCINT enabled only for the B port.
  sei();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

void requestEvent() { 
  Wire.write(data_t, sizeof(data_t)); //the data are sent to the master
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

ISR(PCINT0_vect, ISR_HOW) {
  uint8_t mask;
  uint8_t cPINB; // To read the Port B Input value
  uint32_t cTime; // To keep track of the current executing time, i.e. current Time
  cPINB = PINB;                                         // Let's save the Port B Input value as soon as possible  // modded 14/05
  cTime = micros();                                     // Reads the number of microseconds elapsed since the start of the code, with a resolution of 4 us
  mask = cPINB ^ PCintLast;                             // XORing the Port D Input with the last value saved we get mask for the changed bits
  PCintLast = cPINB;    // we save the Port B Input value for the next interrupt occurence
  if (mask & 0x04)
  { // Let's check the mask for the pin STEERING
    pinData[0].counter++;
    if (!(cPINB & 0x04)) // True if the STEERING is LOW
      pinData[0].pulseWidth = cTime - pinData[0].edgeTime; // Save the pulse width
    else // If instead the STEERING is HIGH, let's keep track of the rise time
      pinData[0].edgeTime = cTime; // Updates the edgeTime for the pin with the cTime value
  }
  if (mask & 0x02)
  { // Let's check the mask for the pin MOTOR
    pinData[1].counter++;
    if (!(cPINB & 0x02)) // True if the MOTOR is LOW
      pinData[1].pulseWidth = cTime - pinData[1].edgeTime; // Save the pulse width
    else // If instead the MOTOR is HIGH, let's keep track of the rise time
      pinData[1].edgeTime = cTime; // Updates the edgeTime for the pin with the cTime value
  }
  if (mask & 0x01) // Let's check the mask for the pin MODE
  {
    pinData[2].counter++;
    if (!(cPINB & 0x01))  // True if the MODE is LOW
      pinData[2].pulseWidth = cTime - pinData[2].edgeTime; // Save the pulse width
    else // If instead the MODE is HIGH, let's keep track of the rise time
      pinData[2].edgeTime = cTime; // Updates the edgeTime for the pin with the cTime value
  }
}
