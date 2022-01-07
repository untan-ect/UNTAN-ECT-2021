// Deklarasi PinOut MCU untuk LED
const int ledRed = PA1;
const int ledYellow = PA2;
const int ledGreen = PA3;

//Deklarasi Variabel millis
int crossTime = 1000;
unsigned long changeTime = 0;

//Deklarasi Variabel Pendukung
int x;
int wledRed;
int wledYellow;
int wledGreen;

void setup() {
  pinMode(ledRed, OUTPUT);    // Set Pin LED Sebagai Output
  pinMode(ledYellow, OUTPUT);
  pinMode(ledGreen, OUTPUT);
  Serial.begin(9600);         // Komunikasi Serial MCU ke Laptop pada baud 9600

}
void loop() {
  if ((millis() - changeTime) > crossTime) { Jika (millis - changeTime) > crossTime
    if (x == 0) { // Jika x = 0 
      digitalWrite(ledRed, HIGH);
      digitalWrite(ledYellow, LOW);
      digitalWrite(ledGreen, LOW);
      x = x + 1; // Penjumlahan
      changeTime = millis(); // variabel changeTime menyimpan data terakhir millis
    }
    else if (x == 1) { // Jika x = 1
      digitalWrite(ledRed, LOW);
      digitalWrite(ledYellow, HIGH);
      digitalWrite(ledGreen, LOW);
      x = x + 1; // Penjumlahan
      changeTime = millis(); // variabel changeTime menyimpan data terakhir millis
    }
    else if (x == 2) { // Jika x = 2
      digitalWrite(ledRed, LOW);
      digitalWrite(ledYellow, LOW);
      digitalWrite(ledGreen, HIGH);
      x = x + 1; // Penjumlahan
      changeTime = millis(); // variabel changeTime menyimpan data terakhir millis
    }
  }

  if(x > 2) {
    x = 0; // Jika x lebih besar dari 2 maka x = 0
  }

}
