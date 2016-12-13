#include <Wire.h>
#include "EncoderBoard.h"

EncoderBoard Enc3(3);
EncoderBoard Enc4(4);
EncoderBoard Enc5(5);
EncoderBoard Enc6(6);

//////////////////////////////////////////////////////////////////////////////////////////////////////

void setup() {
  Serial.begin(115200);  // start serial for output
  Serial.println();
  Serial.print("#!\t");
  Serial.print("Time");
  Serial.print("\t");
  Serial.print("Enc. 1");
  Serial.print("\t");
  Serial.print("Enc. 2");
  Serial.print("\t");
  Serial.print("Enc. 3");
  Serial.print("\t");
  Serial.print("Enc. 4");
  Serial.print("\t");
  Serial.print("Vel. 1");
  Serial.print("\t");
  Serial.print("Vel. 2");
  Serial.print("\t");
  Serial.print("Vel.3");
  Serial.print("\t");
  Serial.println("Vel. 4");
  }

//////////////////////////////////////////////////////////////////////////////////////////////////////

void loop() {
  Enc3.update();
  Enc4.update();
  Enc5.update();
  Enc6.update();
  
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
  Serial.println(Enc6.angular_speed(0), 4);

   delay(10);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////


