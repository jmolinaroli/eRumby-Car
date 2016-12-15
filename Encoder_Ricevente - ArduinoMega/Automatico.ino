uint32_t inizioTime; // tiene traccia del tempo corrente
float outputPIPrev = 0;
float errorPrev = 0;
float wMediaPrec = 0;
float targetFilteredPrec = 0;
void AutomaticoCamera() {
  //leggo la distanza dal cantro della linea dalla camera e la vado a mappare sullo stezo (comandi al contrario)
  comandoSERVO = map((int)(*enne), -48, +48, DUTY_SERVO_DX, DUTY_SERVO_SX);
  //comandoSERVO = map((int)(*enne), -80, +80, DUTY_SERVO_DX, DUTY_SERVO_SX);
  comandoESC = PIVelocityControl();
  pwmWriteHR(SERVO, comandoSERVO);
  pwmWriteHR(ESC, comandoESC);
}
//codice per far seguire alla macchina un profilo di trottle nel tempo
void AutomaticoVelocita() {
  inizioTime = millis();
  /*  if (inizioTime - startTime < 2000) {
      comandoESC = DUTY_ESC_NEUTRO;
      comandoSERVO = DUTY_SERVO_CENTRO;
      pwmWriteHR(SERVO, comandoSERVO);
      pwmWriteHR(ESC, comandoESC);
    }
    if ((inizioTime - startTime >= 2000) && (inizioTime - startTime <= 11000)) {

      comandoESC = DUTY_ESC_NEUTRO - (((inizioTime - startTime - 2000) / 250) * (DUTY_ESC_NEUTRO - DUTY_ESC_MIN) / 40);
      if (comandoESC < DUTY_ESC_MIN) {
        comandoESC = DUTY_ESC_MIN;
      }
      comandoSERVO = DUTY_SERVO_CENTRO;
      pwmWriteHR(SERVO, comandoSERVO);
      pwmWriteHR(ESC, comandoESC);
    }
    if (inizioTime - startTime > 11000) {
      comandoESC = ;    //variabile per inviare i comandi all'ESC
      comandoSERVO = DUTY_SERVO_CENTRO;
      pwmWriteHR(SERVO, comandoSERVO);
      pwmWriteHR(ESC, comandoESC);
    }*/
  if (inizioTime - startTime < 1000) {
    //comandoESC = DUTY_ESC_NEUTRO;
    comandoESC = PIVelocityControl();
    comandoSERVO = DUTY_SERVO_CENTRO;
    pwmWriteHR(SERVO, comandoSERVO);
    pwmWriteHR(ESC, comandoESC);
  }
  if ((inizioTime - startTime >= 1000) && (inizioTime - startTime <= 9000)) {
    comandoESC = PIVelocityControl();
    comandoSERVO = DUTY_SERVO_CENTRO;
    pwmWriteHR(SERVO, comandoSERVO);
    pwmWriteHR(ESC, comandoESC);
  }
  if (inizioTime - startTime > 9000) {
    comandoESC = PIVelocityControl();    //variabile per inviare i comandi all'ESC
    comandoSERVO = DUTY_SERVO_CENTRO;
    pwmWriteHR(SERVO, comandoSERVO);
    pwmWriteHR(ESC, comandoESC);
  }
}

uint16_t PIVelocityControl() {
  uint16_t uscita;
  int targetINT = atoi(target);  //target trasformato in intero
  float Ts = 0.017;  //tempo di campionamento [sec]
  float tau = 0.709082;  //costante di tempo del PI
  float Fs = 8 / tau;    //frequenza di taglio del filtro per l'ingresso
  float Kp = 4;          //costante proporzionale del PI
  float targetFiltered = (float)targetINT;//(targetFilteredPrec + targetINT * Ts * Fs) / (1 + Ts * Fs);       //filtraggio velocità di riferimento
  if (wMedia > targetINT * 1.75)          //check per outlier
    wMedia = wMediaPrec;
  float error = targetFiltered - wMedia ;        //errore in ingresso al PI
  float outputPI = outputPIPrev + Kp * (error - errorPrev) + Kp * (Ts / tau) * error; //valore in uscitadel PI
  float appo = (outputPI - 71.56) / 35.11;                         //valore in uscita dal PI
  float throttle = (-38.97) * appo * appo + (-311.1) * appo + (6352); //valore compreso tra 0 e 1
  uscita = (int)throttle;        //valore da inviare all'ESC
  if (uscita >= DUTY_ESC_NEUTRO)                            //saturo il valore di controllo tra 0 e 1
    uscita = DUTY_ESC_NEUTRO;
  if (uscita <= DUTY_ESC_MIN)
    uscita = DUTY_ESC_MIN;
  //salvetaggio valori precedenti
  errorPrev = error;
  outputPIPrev = outputPI;
  wMediaPrec = wMedia;
  targetFilteredPrec = targetFiltered;
  return uscita;
}

