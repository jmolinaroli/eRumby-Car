// STERZO - VALORI DI DUTY CYCLE (ms, periodo in cui sta alta l'onda quadra)
#define DUTY_STEERING_RIGHT 1064
#define DUTY_STEERING_NEUTRAL 1476
#define DUTY_STEERING_LEFT 1890
// Proporzione dei valori precedenti in 16 bit
// Valore nominale = 6881
// Valori limite ottenuti = +/- 20% valore nominale
#define DUTY_SERVO_LEFT 5505   //4915
#define DUTY_SERVO_NEUTRAL 6881
#define DUTY_SERVO_RIGHT 8257    //8847

// MOTORE - VALORI DI DUTY CYCLE (ms, periodo in cui sta alta l'onda quadra)
#define DUTY_MOTOR_MAX 2032
#define DUTY_MOTOR_NEUTRAL 1496
#define DUTY_MOTOR_MIN 1000
// Proporzione dei valori precedenti in 16 bit
// Valore nominale = 7012
// Valori limite ottenuti = +/- 15% valore nominale
#define DUTY_ESC_MAX 8064
#define DUTY_ESC_NEUTRAL 7010
#define DUTY_ESC_MIN 5960

// MODALITA - VALORI DI DUTY CYCLE (ms, periodo in cui sta alta l'onda quadra)
#define DUTY_MODE_HIGH 2032
#define DUTY_MODE_CENTRAL 1496
#define DUTY_MODE_LOW 1000

int val = 0;
int valS = 0;

void SetMotori(int pin_ESC, int pin_Servo) {
  SetPinFrequency(ESC, 71.4);
  SetPinFrequency(SERVO, 71.4);
}
void UpdateMotors() {
  valS = map(pinData[0].pulseWidth, DUTY_STEERING_RIGHT, DUTY_STEERING_LEFT, DUTY_SERVO_RIGHT, DUTY_SERVO_LEFT);
  
  if (pinData[1].pulseWidth <= DUTY_MOTOR_NEUTRAL - 50) {
    val = map(pinData[1].pulseWidth, DUTY_MOTOR_MIN, DUTY_MOTOR_MAX, DUTY_ESC_NEUTRAL, DUTY_ESC_MIN);
    pwmWriteHR(ESC, val);
    //pwmWriteHR(ESC, DUTY_ESC_NEUTRAL);
  }
  if (pinData[1].pulseWidth >= DUTY_MOTOR_NEUTRAL + 50) {
    val = map(pinData[1].pulseWidth, DUTY_MOTOR_MIN, DUTY_MOTOR_MAX, DUTY_ESC_NEUTRAL, DUTY_ESC_MIN);
    pwmWriteHR(ESC, val);
  }
  if ((pinData[1].pulseWidth <= DUTY_MOTOR_NEUTRAL + 50) && (pinData[1].pulseWidth >= DUTY_MOTOR_NEUTRAL - 50)) {
    pwmWriteHR(ESC, DUTY_ESC_NEUTRAL);
  }
  pwmWriteHR(SERVO, valS);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

void IdleStatus() {
    pwmWriteHR(ESC, DUTY_ESC_NEUTRAL);
    pwmWriteHR(SERVO, DUTY_SERVO_NEUTRAL);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

void AutomaticControl() {
    pwmWriteHR(ESC, DUTY_ESC_MAX/4);
    pwmWriteHR(SERVO, DUTY_SERVO_NEUTRAL);
}
