#include "Arduino.h"
#include "PCF8574.h"

PCF8574 pcf8574(0x20);

void setup(){
  Serial.begin(115200);
  delay(1000);

  pcf8574.pinMode(P0, OUTPUT);
  pcf8574.pinMode(P1, OUTPUT);
  pcf8574.pinMode(P2, OUTPUT);
  pcf8574.pinMode(P3, OUTPUT);
  pcf8574.pinMode(P4, INPUT);
  pcf8574.pinMode(P5, INPUT);
  pcf8574.pinMode(P6, INPUT);
  pcf8574.pinMode(P7, INPUT);
  
  Serial.print("Inisialisasi PCF8574...");
    if (pcf8574.begin()){
	Serial.println("Berhasil");
    }else{
	Serial.println("Inisialisasi Gagal");
    }
}

void loop(){
    if (pcf8574.digitalRead(P4) == HIGH){
      pcf8574.digitalWrite(P0, HIGH);
      delay(50);
    }
    if (pcf8574.digitalRead(P5) == HIGH){
      pcf8574.digitalWrite(P1, HIGH);
      delay(50);
    }
    if (pcf8574.digitalRead(P6) == HIGH){
      pcf8574.digitalWrite(P2, HIGH);
      delay(50);
    }
    if (pcf8574.digitalRead(P7) == HIGH){
      pcf8574.digitalWrite(P3, HIGH);
      delay(50);
    }
}

/*
  Referensi : https://create.arduino.cc/projecthub/tarantula3/pcf8574-gpio-extender-with-arduino-and-nodemcu-a8cb00
  Library   : https://github.com/xreef/PCF8574_library
*/
