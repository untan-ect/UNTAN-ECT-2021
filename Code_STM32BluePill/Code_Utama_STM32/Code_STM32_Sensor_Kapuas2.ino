#include <SPI.h>
#include <Wire.h>
#include <SD.h>

#define Volt PB1
#define Arus PB0
#define Suhu PA1
#define pedal PA0
#define Speed PB4
#define BLDC PB6
#define indikator PB5
#define CS_PIN PA4 //Medenefisikan variabel CS_PIN pada pin PA4 di STM32

File dataFile;

unsigned long oldtime[11];
unsigned long interval[5] = {30, 500, 1000, 2000, 60000};
String myString;

//Deklarasi Sensor tegangan
float Vstm; // tegangan terbaca pada stm32 antara 0-3.3V
float Vreal; // tegangan sesungguhnya yaitu antara 0-60V
float nilai; // nilai bit terbaca
float V;

//Deklarasi Sensor Arus
#define nilaiKoreksi 164 //mA
#define iterasi 10000
#define teganganReferensi 5000.0 //5000mv is for 5V
#define bitResolusi 12
int nilaiSensor;
float voltage, current;
float I = 0;

//Deklarasi Variabel Daya
float P = 0;

//Deklarasi Sensor suhu
float val = 0;
float mv = 0;
float T = 0;

//Deklarasi Sensor Kecepatan
int rev = 0; //Deklarasi nilai variabel rev = 0
float rpm; //Deklarasi Variabel
float timeold = 0; //Deklarasi nilai variabel oldtime = 0
float KEC = 0; //Deklarasi nilai variabel Speed = 0
float rotasi;
float time; //Deklarasi Variabel time
float v = 0; //Kecepatan KM/Jam

//Deklarasi Kapasitas_Arus_Terpakai yang yang terpakai
float calt;
float E = 0;
float W;
float t = 0;

String Data[7]; // energi
String Kirim_Data;
int NilaiADC, NilaiPWM;
bool a = true;
String ch;
int k;

void isr()
{
  rev++;
}

unsigned long Time(byte ar, int br) {
  unsigned long waktu = (millis() - oldtime[ar]) >= interval[br];
  return waktu;
}

unsigned long sets(byte an) {
  oldtime[an] = millis();
  return oldtime[an];
}

void setup() {
  pinMode(Speed, INPUT);
  pinMode(BLDC, OUTPUT);
  Serial.begin(115200);   //Serial komunikasi untuk dihubungkan ke laptop/PC dengan baud rate 115200
  Serial3.begin(115200);  //Serial komunikasi HardwareSerial dari Serial3 untuk dihubungkan ke mikrokontroler NodeMCU lain pada baud rate 115200
  delay(1000);

  Serial.println("Inisialisasi kartu.");
  if (!SD.begin(CS_PIN)) {
    Serial.println("Gagal, cek apakah kartu SD terhubung.");
    return;
  }
  Serial.println();
  Serial.println("Inisialisasi kartu SD berhasil.\n");

  dataFile = SD.open("Kapuas_2.xls", FILE_WRITE); //Menjadikan variabel dataFile sebagai alat untuk membuat file dan membuka file di SD card
  if (!dataFile) {
    Serial.println("Gagal membuka file untuk ditulis");
    return;
  }

  if (dataFile) {
      Serial.println("Berhasil membuka file untuk ditulis");
      dataFile.print("Tegangan"); 
      dataFile.print("\t\t"); //Tab 2 kali
      dataFile.print("Arus"); 
      dataFile.print("\t\t"); //Tab 2 kali
      dataFile.print("Daya"); 
      dataFile.print("\t\t"); //Tab 2 kali
      dataFile.print("Temperature"); 
      dataFile.print("\t\t"); //Tab 2 kali
      dataFile.print("Kecepatan"); 
      dataFile.print("\t\t"); //Tab 2 kali
      dataFile.print("Energi"); 
      dataFile.println("");
    //menutup file setelah menggunakannya
    dataFile.close();
  }
  // jika file tidak dapat dibuka, data tidak akan ditulis.
  else {
    Serial.println("Gagal membuka file Kapuas_2.xls"); //Menampilkan kata "Gagal membuka  file NilaiPot.xls" diserial monitor laptop
  }

  digitalWrite(indikator, HIGH);
  delay(3000);
  digitalWrite(indikator, LOW);
  digitalWrite(Speed, HIGH);// Instead of using a pull up resistor
  attachInterrupt(Speed, isr, RISING);
  delay(10);
}


void loop() {
  NilaiADC = analogRead(pedal);
  NilaiPWM = map(NilaiPWM, 515, 2566, 0, 255);
  if (NilaiPWM <= 0) {
    NilaiPWM = 0;
  }
  if (NilaiPWM >= 255) {
    NilaiPWM = 255;
  }

  if (Time(0, 0)) {
    analogWrite(BLDC, NilaiPWM);
    sets(0);
  }

  if (Time(1, 1)) {
    serialData();
    sets(1);
  }

  SD_Card();

  if (T >= 60 || E >= 20) {
    if (Time(2, 2)) {
      if (a == true) {
        digitalWrite(indikator, HIGH);
        a = false;
      }
      else if (a == false) {
        digitalWrite(indikator, LOW);
        a = true;
      }
      sets(2);
    }
  }
  else {
    digitalWrite(indikator, LOW);
  }

}

void Daya_Baterai() {
  if (Time(7, 1)) {
    P = I * V;
    sets(7);
  }
}

void Energi_Terpakai() {
  if (Time(8, 1)) {
    t++;
    W = (P * t) / 3600;
    sets(8);
  }
}

void Persen_Baterai() {
  if (Time(9, 1)) {
    for (int n = 1; n <= 500; n++) {
      calt = (V - 48.00) / 0.048;
      E += calt;
    }
    E /= 500;
    if (E <= 0) {
      E = 0;
    }
    else if (E >= 100) {
      E = 100;
    }
    sets(9);
  }
}

void SD_Card() {
  dataFile = SD.open("Kapuas_2.xls");
  if (!dataFile) {
    Serial.println("Gagal membuka file untuk ditambahkan");
    return;
  }
  if (dataFile) {
    if (Time(10, 4)) {
      Serial.println("File Kapuas_2.xls Berhasil Dibuka.");
      dataFile.print(V);
      dataFile.print("\t"); //Tab 1 kali
      dataFile.print(I);
      dataFile.print("\t"); //Tab 1 kali
      dataFile.print(P);
      dataFile.print("\t"); //Tab 1 kali
      dataFile.print(T);
      dataFile.print("\t"); //Tab 1 kali
      dataFile.print(v);
      dataFile.print("\t"); //Tab 1 kali
      dataFile.print(E);
      dataFile.println("");
      sets(10);
    }
    dataFile.close();
  }
  else {
    if (Time(10, 4)) {
      Serial.println("Gagal membuka file Kapuas_2.xls"); //Menampilkan kata "Gagal membuka  file NilaiPot.xls" diserial monitor laptop
      sets(10);
    }
  }
}

void serialData() {
  Sensor_Tegangan();
  Sensor_Arus();
  Daya_Baterai();
  Sensor_Temperatur();
  Sensor_Kecepatan();
  Energi_Terpakai();
  Persen_Baterai();

  Data[0] = V;
  Data[1] = I;
  Data[2] = P;
  Data[3] = T;
  Data[4] = v;
  Data[5] = W;
  Data[6] = E;
  Kirim_Data = Data[0] + "," + Data[1] + "," + Data[2] + ","  + Data[3] + "," + Data[4] + "," + Data[5] + "," + Data[6];

  Serial.println("----------------------------------");
  Serial.println("Tegangan V        = " + Data[0] + " V");
  Serial.println("Arus I            = " + Data[1] + " A");
  Serial.println("Daya P            = " + Data[2] + " W");
  Serial.println("Temperature T     = " + Data[3] + " °C");
  Serial.println("Kecepatan v       = " + Data[4] + " Km/Jam");
  Serial.println("Energi Terpakai E = " + Data[5] + " Wh");
  Serial.println("Persen Baterai W  = " + Data[6] + " %");
  Serial.println("----------------------------------");
  Serial3.println(Kirim_Data); //Mengirmkan Data Ke NodeMCU
}

void Sensor_Tegangan() {
  if (Time(3, 1)) {
    for (int j = 1; j <= 500; j++) {
      nilai = analogRead(Volt); // pembacaan pin
      Vstm = (nilai / 4095) * 3.24;
      Vreal = Vstm * (680 + 39) / 39;
      V += Vreal;
    }
    V /= 500;
    sets(3);
  }
}

void Sensor_Arus() {
  if (Time(4, 1)) {
    for (int i = 1; i <= iterasi; i++)
    {
      nilaiSensor = analogRead(Arus);
      voltage = (nilaiSensor) * (teganganReferensi / (pow(2, bitResolusi) - 1)) - (0.5 * teganganReferensi) + nilaiKoreksi ;
      current  = voltage / 66.0  ;
      I += current;
    }

    I /= iterasi;
    I = I - 15.65;
    if (I < 0) {
      I = 0;
    }
    sets(4);
  }
  //Serial.println("Arus : " + String(I));
  //Serial.println("nilaiSensor : " + String(nilaiSensor));
}

void Sensor_Temperatur() {
  if (Time(5, 1)) {
    //Data sensor suhu
    val = analogRead(Suhu);
    mv = ( val / 4095) * 3300;
    T = mv / 10;
    sets(5);
  }
}

void Sensor_Kecepatan() {
  if (Time(6, 3)) {
    detachInterrupt(digitalPinToInterrupt(Speed));
    time = millis() - timeold;
    rpm = (rev / time) * 60000;
    timeold = millis();
    KEC = (rpm * 2 * 3.14 * 0.28) / 60;
    v = KEC * 3.6;
    rotasi = 0;
    rev = 0;
    attachInterrupt(Speed, isr, RISING);
    sets(6);
  }
}
