#include "PWM.h" 
#include "pins_arduino.h" 
#include "Arduino.h" 
#include "inttypes.h" 
 
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
  Serial.begin(57600); 
  configureReceiver(); // Setup receiver pins for pin change interrupts 
  Serial.flush(); 
  SetMotori(ESC,SERVO); 
} 
 
void loop() { 
  //Invio dati! 
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
 
void PrintData() { 
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
 
