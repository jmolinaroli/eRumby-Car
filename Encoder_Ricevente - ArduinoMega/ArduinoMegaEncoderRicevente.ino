#include "inttypes.h"
#include "PWM.h"
#include "pins_arduino.h"
#include "Arduino.h"

#define ISR_HOW ISR_NOBLOCK                   //ISR_BLOCK  Abilita la ISR delle porte
#define BAUD 19200                            //Baud Rate per la comunicazione con lo Yun
/*
 *Siamo interessati ai pin  (50), (51), (52) e (53) per effettuare la lettura degli encoder
 * sono stati scelti i PCINT 0/1/2/3 <-> PinArduino 53/52/51/50  -->Porta B
 * sono stati scelti i pin  (Analog8), (Analog9), (2) per effettuare la lettura della ricevente
 * sono stati scelti i PCINT 16/17 <-> PinArduino Analog8/Analog9 -->Porta K
 *---------------------------------------------------------------------------*/

#define ASPIN 53  //1 bit del registro  0x01    
#define ADPIN 51  //3 bit del registro  0x04  
#define PSPIN 50  //4 bit del registro  0x08
#define PDPIN 52  //2°bit del registro  0x02

#define STERZO A8            //1 bit del registro
#define ACCELERATORE A9      //2 bit del registro
#define MODALITA 2           // pin a cui è associata la lettura della modalità

#define ESC 11                //pin di uscita verso la ESC
#define SERVO 12              //pin di uscita verso il servo dello sterzo

// STERZO - VALORI DI DUTY CYCLE (us, periodo in cui sta alta l'onda quadra)
#define DUTY_STERZO_DX 1052
#define DUTY_STERZO_NEUTRO 1476
#define DUTY_STERZO_SX 1890

// Proporzione dei valori precedenti in 16 bit
// Valore nominale = 6881
// Valori limite ottenuti = +/- 27% valore nominale
#define DUTY_SERVO_DX 5024//4915  valore minimo, tradotto dal comando
#define DUTY_SERVO_CENTRO 6881
#define DUTY_SERVO_SX 8738//8847 valore massimo, tradotto dal comando

// MOTORE - VALORI DI DUTY CYCLE (us, periodo in cui sta alta l'onda quadra)
#define DUTY_MOTORE_MAX 2032
#define DUTY_MOTORE_NEUTRO 1500
#define DUTY_MOTORE_NEUTRO_SAFE 1340
#define DUTY_MOTORE_MIN 1000

// Proporzione dei valori precedenti in 16 bit
// Valore nominale = 7012
// Valori limite ottenuti = +/- 20% valore nominale
#define DUTY_ESC_MAX 8412//8064
#define DUTY_ESC_MAX_SAFE 7700
#define DUTY_ESC_NEUTRO 7010
#define DUTY_ESC_MIN 5608//5960

//MODALITA - VALORI DI DUTY CYCLE (us, periodo in cui sta alta l'onda quadra)
#define DUTY_MODALITA_ALTO 2024          //us in cui l'onda sta alta per lo switch messo in alto
#define DUTY_MODALITA_CENTRO 1504        //us in cui l'onda sta alta per lo switch messo in centro
#define DUTY_MODALITA_BASSO 980          //us in cui l'onda sta alta per lo switch messo in basso

//una sola struttura dati per la lattura di tutti e 7 gli interrupt
typedef struct Dati {
  uint16_t pulseWidth;                          //contiene il dt che ha impiegato una tacca trasparente a passare
  uint16_t pulseWidthReal;                      //serve solo per la modalità per evitare che entri in sicurezza a causa di letture non corrette
  uint16_t edgeTime;                            //contine l'ultimo istante di tempo in cui l'onda è stata alta
  byte counter;                                 //numero di letture tra un invio e l'altro, per la modalità lo usiamo come sicurezza
};


volatile static uint8_t PBintLast;                  //lettura precedente della porta B
volatile static uint8_t PKintLast;                  //lettura precedente della porta K
volatile static Dati pinDataEncoder[4];             //Vettore dei dati dell'encoder, 0=AS; 1=AD; 3=PS; 4=PD
volatile static Dati pinDataRicevente [3];          //Vettore dei dati della ricevente, 0=STERZO; 1=ACCELLERATORE; 2=MODALITA
byte dataCamera[4] = {0};                     //buffer lettura dati della camera
byte dataTarget[2] = {0};                    //buffer di lettura dati per cruise control
float* enne = NULL;
char* target = new char[2];                  //variabile che contiene la velocità target
uint16_t comandoESC = DUTY_ESC_NEUTRO;          //variabile per inviare i comandi all'ESC
uint16_t comandoSERVO = DUTY_SERVO_CENTRO;      //variabile per inviare i comandi al SERVO
float wMedia = 0;
float wMediaOld = 0;
float velocitaOLD[4] = {0};
float counterOLD[4] = {0};

//variabili per il profilo di velocità
uint32_t startTime = 0; // tiene traccia del tempo corrente

void setup() {
  InitTimersSafe();                             //inizializza i timer di sistema
  Serial.begin(BAUD);                           //inizializza la comunicazione seriale con lo Yun
  Serial1.begin(BAUD);
  configureReceiver();                          //inizializzo i pin per la lettura degli interrupt
  Serial.flush();                               //pulizia della seriale
  SetMotori();                                  //inizializza i pin dedicati al servo e alla ESC
}

void loop() {
  delay(100);
  char q = Serial.read();
  PrintDataEncoder();
  //Invio dati!
  
//  if (q == 'a') {                    //se ricevo 'a' in seriale è ora di inviare i dati!
//    cli();                                       //blocco interrupt
//    PrintDataEncoder();                          //invio dati encoder
//    PrintDataReceiver();                         //invio dati ricevente
//    sei();                                       //riavvio interrupt
//  }
//  if (q == 'g') {
//    Serial.readBytes(dataTarget, 4);
//    sprintf(target, "%c%c", dataTarget[0], dataTarget[1]);
//  }
//  if (Serial1.available()) {
//    Serial1.readBytes(dataCamera, 4);
//    enne = (float*)dataCamera;
//  }
  //controllo se sono in manuale, se si invio i comandi del telecomando ai motori

  //controllo se sono in manuale, se si invio i comandi del telecomando ai motori
  if ((pinDataRicevente[2].pulseWidthReal >= DUTY_MODALITA_CENTRO - 75) && (pinDataRicevente[2].pulseWidthReal <= DUTY_MODALITA_CENTRO + 75)) {
    //Serial.print("Secure mode!");
    Sicurezza();
  }
  //controllo se sono in safety, se si invio i comandi di neutro ai motori
  if ((pinDataRicevente[2].pulseWidthReal >= DUTY_MODALITA_BASSO - 75) && (pinDataRicevente[2].pulseWidthReal <= DUTY_MODALITA_BASSO + 75)) {
    //Serial.print("Remote control!");
    UpdateMotori();
  }
  if ((pinDataRicevente[2].pulseWidthReal >= DUTY_MODALITA_ALTO - 75) && (pinDataRicevente[2].pulseWidthReal <= DUTY_MODALITA_ALTO + 75)) {
    //Serial.flush();
//    Serial.print("Automatic control!");
//    if (q == "a") {
//    Serial.print("Straight with constant velocity");
//    sprintf(target, "%c%c", '4', '0');
//    comandoSERVO = DUTY_SERVO_CENTRO;
//    pwmWriteHR(SERVO, comandoSERVO);
//  }
//  if (q == "b") {
//    Serial.print("Straight with increasing velocity");
//    sprintf(target, "%c%c", '4', '0');
//    comandoSERVO = DUTY_SERVO_CENTRO;
//    pwmWriteHR(SERVO, comandoSERVO);
//  }
//  if (q == "c") {
//    Serial.print("Stearing with constant velocity");
//    sprintf(target, "%c%c", '3', '0');
//    comandoSERVO = DUTY_SERVO_DX;
//    pwmWriteHR(SERVO, comandoSERVO);
//  }
//  if (q == "d") {
//    Serial.print("Sweep stearing with constant velocity");
//    sprintf(target, "%c%c", '3', '0');
//    comandoSERVO = DUTY_SERVO_DX;
//    pwmWriteHR(SERVO, comandoSERVO);
//  }
    Automatico();
  }
}

void PrintDataEncoder() {
  //dai usec ricavo le velocità angolari e invio quelle
  float velocita[4];
  velocita[0] = (pinDataEncoder[0].counter > 1) ? (6.28 / 200.0) / ((float)(pinDataEncoder[0].pulseWidth) / 1000000) : 0;
  velocita[1] = (pinDataEncoder[1].counter > 1) ? (6.28 / 200.0) / ((float)(pinDataEncoder[1].pulseWidth) / 1000000) : 0;
  velocita[2] = (pinDataEncoder[2].counter > 1) ? (6.28 / 200.0) / ((float)(pinDataEncoder[2].pulseWidth) / 1000000) : 0;
  velocita[3] = (pinDataEncoder[3].counter > 1) ? (6.28 / 200.0) / ((float)(pinDataEncoder[3].pulseWidth) / 1000000) : 0;
  wMedia = (velocita[2] + velocita[3])/ 2.0;
  
  counterOLD[0] = counterOLD[0] + pinDataEncoder[0].counter;
  counterOLD[1] = counterOLD[1] + pinDataEncoder[1].counter;
  counterOLD[2] = counterOLD[2] + pinDataEncoder[2].counter;
  counterOLD[3] = counterOLD[3] + pinDataEncoder[3].counter;
  
  //Serial.print(counterOLD[0]/200.);
  //Serial.print("\t");
  //Serial.print(counterOLD[1]/200.);
  //Serial.print("\t");
  //Serial.print(counterOLD[2]/200.);
  //Serial.print("\t");
  //Serial.print(counterOLD[3]/200.);
  //Serial.print("\t");
   

  //
  
  
//  if (abs(wMedia - wMediaOld) >= 30){
//      wMedia = wMediaOld;
//      velocita[0]=velocitaOLD[0];
//      velocita[1]=velocitaOLD[1];
//      velocita[2]=velocitaOLD[2];
//      velocita[3]=velocitaOLD[3];
//    }

  if (wMedia >= 100) {
        wMedia = wMediaOld;
        velocita[0]=velocitaOLD[0];
        velocita[1]=velocitaOLD[1];
        velocita[2]=velocitaOLD[2];
        velocita[3]=velocitaOLD[3];
  }
    
  byte* b = (byte*)velocita;
  //Serial.write(b, 16);
  b = (byte*)&wMedia;
  //Serial.write(b, 4);

  //Serial.print(pinDataRicevente[1].pulseWidth);
  //Serial.print("\t");
  Serial.print(target);
  Serial.print("\t");
  Serial.print(velocita[0]);
  Serial.print("\t");
  Serial.print(velocita[1]);
  Serial.print("\t");
  Serial.print(velocita[2]);
  Serial.print("\t");
  Serial.print(velocita[3]);
  Serial.print("\t");
  Serial.print(wMedia);
  Serial.print("\n");

  pinDataEncoder[0].counter = 0;
  pinDataEncoder[1].counter = 0;
  pinDataEncoder[2].counter = 0;
  pinDataEncoder[3].counter = 0;
  velocitaOLD[0]=velocita[0];
  velocitaOLD[1]=velocita[1];
  velocitaOLD[2]=velocita[2];
  velocitaOLD[3]=velocita[3];
  wMediaOld = wMedia;
}

void PrintDataReceiver() {
  //invio i dati  della durata dello stato alto dei segnali della ricevente  <--- sostituibile con un numero 0-1-2 che indica la modalità
  byte* b = (byte*)&pinDataRicevente[0].pulseWidth;
  Serial.write(b, 2);
  b = (byte*)&pinDataRicevente[1].pulseWidth;
  Serial.write(b, 2);
  b = (byte*)&pinDataRicevente[2].pulseWidthReal;
  Serial.write(b, 2);
  b = (byte*)&comandoSERVO;
  Serial.write(b, 2);
  b = (byte*)&comandoESC;
  Serial.write(b, 2);
  if ((pinDataRicevente[2].pulseWidthReal >= DUTY_MODALITA_ALTO - 75) && (pinDataRicevente[2].pulseWidthReal <= DUTY_MODALITA_ALTO + 75)) {
    Automatico();
  }
}

void configureReceiver() {
  cli();
  //Pin per gli encoder
  PCMSK0 = 0;
  pinMode(ASPIN, INPUT_PULLUP);          //attivo l resistenze di pullup
  pinMode(ADPIN, INPUT_PULLUP);
  pinMode(PSPIN, INPUT_PULLUP);
  pinMode(PDPIN, INPUT_PULLUP);
  PCMSK0 = 0b00001111;                    //setto la maschera del registro per la lettura degli interrupt della porta B

  //Pin per la ricevente
  PCMSK2 = 0;
  pinMode(A8, INPUT_PULLUP);
  pinMode(A9, INPUT_PULLUP);
  PCMSK2 = 0b00000011;                    //setto la maschera del registro per la lettura degli interrupt della porta K
  PCICR = 0b00000101;                       //Abilita gli interrupt relativi ai CHANGE sulle porte B e K

  attachInterrupt(0, ReadModalita, CHANGE);      //utilizza un interrupt normale per la lettura della ricevente
  sei();
}

ISR(PCINT0_vect, ISR_HOW) {
  uint8_t mask;
  uint8_t cPINB; // Per leggere i valori in ingresso alla porta B
  uint16_t cTime; // tiene traccia del tempo corrente
  cPINB = PINB;                                     // Salvo il valore del registro
  cTime = micros();                                 // legge il tempo attuale
  mask = cPINB ^ PBintLast;                         // XOR tra il valore attuale e quello precedente per vedere se sono avvenuti cambiamenti
  PBintLast = cPINB;    // salva il valore attuale della porta B per le successive invocazioni
  // Controllo la maschera per il pin PSPIN
  if (mask & 0x08)
  {
    pinDataEncoder[2].counter++;
    if (!(cPINB & 0x08)) // Vero se il pin PSPIN è LOW
      pinDataEncoder[2].pulseWidth = cTime - pinDataEncoder[2].edgeTime; // salvo l'ampiezza dell'impulso (tempo attuale-istante di RISING)
    else
      pinDataEncoder[2].edgeTime = cTime; // in caso sia HIGH, salvo l'istante temporale
  }
  // Controllo la maschera per il pin ADPIN
  if (mask & 0x04)
  {
    pinDataEncoder[1].counter++;
    if (!(cPINB & 0x04))
      pinDataEncoder[1].pulseWidth = cTime - pinDataEncoder[1].edgeTime;
    else
      pinDataEncoder[1].edgeTime = cTime;
  }
  // Controllo la maschera per il pin PDPIN
  if (mask & 0x02)
  {
    pinDataEncoder[3].counter++;
    if (!(cPINB & 0x02))
      pinDataEncoder[3].pulseWidth = cTime - pinDataEncoder[3].edgeTime;
    else
      pinDataEncoder[3].edgeTime = cTime;
  }
  // Controllo la maschera per il pin ASPIN
  if (mask & 0x01)
  {
    pinDataEncoder[0].counter++;
    if (!(cPINB & 0x01))
      pinDataEncoder[0].pulseWidth = cTime - pinDataEncoder[0].edgeTime;
    else
      pinDataEncoder[0].edgeTime = cTime;
  }
}
//Algoritmo è IDENTICO al precedente, cambia solo la porta che passa dalla B alla K per leggere la ricevente
ISR(PCINT2_vect, ISR_HOW) {
  uint8_t mask;
  uint8_t cPINK;
  uint16_t cTime;
  cPINK = PINK;
  cTime = micros();
  mask = cPINK ^ PKintLast;
  PKintLast = cPINK;
  // ACCELERATORE
  if (mask & 0x02)
  {
    if (!(cPINK & 0x02))
      pinDataRicevente[1].pulseWidth = cTime - pinDataRicevente[1].edgeTime;
    else
      pinDataRicevente[1].edgeTime = cTime;
  }
  // STERZO
  if (mask & 0x01)
  {
    if (!(cPINK & 0x01))
      pinDataRicevente[0].pulseWidth = cTime - pinDataRicevente[0].edgeTime;
    else
      pinDataRicevente[0].edgeTime = cTime;
  }
}

//Algoritmo IDENTICO ai precedenti due per la lettura della durata dello stato alto.
void ReadModalita() {
  uint16_t cTime = micros();
  if (digitalRead(MODALITA) == LOW)
    pinDataRicevente[2].pulseWidth = cTime - pinDataRicevente[2].edgeTime;
  else
    pinDataRicevente[2].edgeTime = cTime;
  //mi convinco che sono in una modalità, solo dopo che leggo 10 volte consecutive lo stesso valore di pulseWidth,
  //in caso contrario resetto il contatore
  //(prevengo errori di errata lettura della modalità)
  if (abs(pinDataRicevente[2].pulseWidthReal - pinDataRicevente[2].pulseWidth) > 500)
    pinDataRicevente[2].counter++;
  else
    pinDataRicevente[2].counter = 0;
  if (pinDataRicevente[2].counter == 10) {
    pinDataRicevente[2].pulseWidthReal = pinDataRicevente[2].pulseWidth;
    pinDataRicevente[2].counter = 0;
  }
}
