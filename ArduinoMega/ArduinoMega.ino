#include <Wire.h>
#include "EncoderBoard.h"
#include "PWM.h"
#include "pins_arduino.h"
#include "Arduino.h"
#include "inttypes.h"

EncoderBoard Enc3(3);
EncoderBoard Enc4(4);
EncoderBoard Enc5(5);
EncoderBoard Enc6(6);

#define ISR_HOW ISR_NOBLOCK //ISR_BLOCK  ISR_NOBLOCK

#define STEERING 51  //3°bit del registro  0x04  
#define MOTOR 52  //2°bit del registro  0x02
#define MODE 53  //1°bit del registro  0x01

#define ESC 11
#define SERVO 12

#define DUTY_MODE_HIGH 2024
#define DUTY_MODE_CENTRAL 1504
#define DUTY_MODE_LOW 980

typedef struct ReceivingData {
  uint32_t pulseWidth;
  uint32_t edgeTime;
  uint16_t duty;
  byte counter;
};

volatile static uint8_t PCintLast;
volatile static ReceivingData pinData[3];
char c[100] = {'\0'};

void setup() {
  InitTimersSafe();
  Serial.begin(115200);
  configureReceiver(); // Setup receiver pins for pin change interrupts
  Serial.flush();
  SetMotori(ESC,SERVO);
}

void loop() {
  //Invio dati!
  
  EncsUpdate();
  if (Serial.read() == 'a') {
    cli();
    PrintData();
    sei();
  }
  if ((pinData[2].pulseWidth >= DUTY_MODE_CENTRAL - 50) && (pinData[2].pulseWidth <= DUTY_MODE_CENTRAL + 50))
    UpdateMotori();
  if ((pinData[2].pulseWidth >= DUTY_MODE_LOW - 50) && (pinData[2].pulseWidth <= DUTY_MODE_LOW + 50))
    Sicurezza();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////

void EncsUpdate() {
  Enc3.update();
  Enc4.update();
  Enc5.update();
  Enc6.update();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

void PrintData() { 
  Serial.print("#!\t");
  Serial.print(millis() / 1000.0, 4);

  Serial.print("\t");
  Serial.print(-Enc3.get(0), 4); //prints data from the first encoder
  Serial.print("\t");
  Serial.print(Enc4.get(0), 4); //prints data from the second encoder
  Serial.print("\t");
  Serial.print(Enc5.get(0), 4); //prints data from the second encoder
  Serial.print("\t");
  Serial.print(Enc6.get(0), 4); //prints data from the second encoder

  Serial.print("\t");
  Serial.print(-Enc3.angular_speed(0), 4);
  Serial.print("\t");
  Serial.print(-Enc4.angular_speed(0), 4);
  Serial.print("\t");
  Serial.print(-Enc5.angular_speed(0), 4);
  Serial.print("\t");
  Serial.print(Enc6.angular_speed(0), 4);

  Serial.print('\t'); 
  Serial.print(pinData[0].pulseWidth); 
  Serial.print('\t'); 
  Serial.print(pinData[1].pulseWidth); 
  Serial.print('\t'); 
  Serial.print(pinData[2].pulseWidth); 
  Serial.print('\n'); 
  pinData[0].counter = 0; 
  pinData[1].counter = 0; 
  pinData[2].counter = 0; 
} 

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


ISR(PCINT0_vect, ISR_HOW) {
  uint8_t mask;
  uint8_t cPINB; // To read the Port B Input value
  uint32_t cTime; // To keep track of the current executing time, i.e. current Time
  cPINB = PINB;  //                                     // Let's save the Port B Input value as soon as possible  // modded 14/05
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
