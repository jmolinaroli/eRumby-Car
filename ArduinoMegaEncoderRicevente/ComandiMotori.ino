void SetMotori()
{
  SetPinFrequency(ESC, 71);               //imposto la frequenza d'uscita del segnale di PWM per i pin associati ai motori
  SetPinFrequency(SERVO, 71);
  pinDataRicevente[2].counter = 0;          //azzero il conteggio per la lettura della corretta modalità
  pinDataRicevente[2].pulseWidthReal = 0;
}

void UpdateMotori()
{
  //leggo la durata del comando per il servo e vado a mappare il valore letto tra i possibili per essere mandati in uscita.
  comandoSERVO = map(pinDataRicevente[0].pulseWidth, DUTY_STERZO_DX, DUTY_STERZO_SX, DUTY_SERVO_SX, DUTY_SERVO_DX);
  pwmWriteHR(SERVO, comandoSERVO);

  //Aumento del range di NEUTRO per l'accelleratore
  //se il valore in input è compreso tra 0 e DUTY_MOTORE_NEUTRO+50, allora invio il comando di neutro al motore
  if (pinDataRicevente[1].pulseWidth <= DUTY_MOTORE_NEUTRO - 50)
  {
    comandoESC = DUTY_ESC_NEUTRO;
    pwmWriteHR(ESC, comandoESC);
  }
  if ((pinDataRicevente[1].pulseWidth <= DUTY_MOTORE_NEUTRO + 50) && (pinDataRicevente[1].pulseWidth >= DUTY_MOTORE_NEUTRO - 50))
  {
    comandoESC = DUTY_ESC_NEUTRO;
    pwmWriteHR(ESC, comandoESC);
  }
  //in caso contrario mappo il segnale in ingresso tra i possibili valori in uscita
  if (pinDataRicevente[1].pulseWidth >= DUTY_MOTORE_NEUTRO + 50)
  {
    comandoESC = map(pinDataRicevente[1].pulseWidth, DUTY_MOTORE_MIN, DUTY_MOTORE_MAX, DUTY_ESC_NEUTRO, DUTY_ESC_MIN);
    pwmWriteHR(ESC, comandoESC);
  }
}

void Sicurezza()
{
  //in sicurezzainvio i comandi di neutro ai due motori, trascurado i segnali di input che arrivano dal telecomando
  comandoESC = DUTY_ESC_NEUTRO;
  comandoSERVO = DUTY_SERVO_CENTRO;
  pwmWriteHR(ESC, comandoESC);
  pwmWriteHR(SERVO, comandoSERVO);
  startTime = 0;
  (*enne) = 0;
}
//wrapper per decidere quali tipo di modalità automatica seguire
void Automatico() {
  //AutomaticoCamera();
  if (startTime == 0)
    startTime = millis();
  AutomaticoVelocita();
}
