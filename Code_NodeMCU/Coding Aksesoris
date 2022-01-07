#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <WiFiClient.h>
#include <Arduino_JSON.h>
#include <UbidotsESPMQTT.h>
#include <Arduino.h>
#include <PCF8574.h>
#include <SoftwareSerial.h>

SoftwareSerial NextSerial(10, 15); // RX (Receiver), TX (Transmitter) // (S3, D8)
PCF8574 pcf8574(0x20);
WiFiClient client;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

#define pinKlakson D0
#define pinWiper D5
#define pinLamUtama D7
#define pinSenKiri P0
#define pinSenKanan P1
#define t_SenKiri P2
#define t_SenKanan P3
#define t_Klakson P4
#define t_Wiper P5
#define t_LamUtama P6

char token[]           = "BBFF-lJhOBUcVPa4rnl2Hxnf5DLPHJZUnqX";
char ssid[]            = "Redmi Not 8";
char pass[]            = "indakaku";

String APIOpenWeather  = "b23308e70faf9521f7bce63d9ecc393e";
String Kota            = "Pontianak";
String KodeNegara      = "ID";

const int GMT = 25200;
String DaftarBulan[12] = {"Jan", "Feb", "Mar", "Apr", "Mei", "Jun", "Jul", "Agt", "Sep", "Okt", "Nov", "Des"};
String jsonBuffer, logo, ikon, deskripsi;
String SkrgTanggal, SkrgJam;
String Kec, Kel, Tek, TemC;
String NxSenKiri, NxSenKanan = "0";

unsigned long WTDisplay, WTGetData = 0;
int WTCloud     = -10000;
int WTCloudDua  = -8000;
int WTCuaca     = -630000;
int WTWaktu     = -500;

unsigned long oldtime[11];
unsigned long interval[7] = {630000, 10000, 1000, 500, 250, 30, 100};

String Data[7];
String PesanData;
char BacaData;
int nilaiKlakson, nilaiKanan, nilaiTengah, nilaiKiri, nilaiWiper, nilaiLampu = 0;
String x, y, z, wi, lu, kl;
int v, E = 0;
bool a = false, b = false, c = false, d = false;
float V, I, P, T, W  = 0;

WiFiEventHandler JikaWiFiTerhubung;
WiFiEventHandler JikaWiFiTerputus;
Ubidots ubidots(token);

unsigned long Time(byte ar, int br) {
  unsigned long waktu = (millis() - oldtime[ar]) >= interval[br];
  return waktu;
}

unsigned long sets(byte an) {
  oldtime[an] = millis();
  return oldtime[an];
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int j = 0; j < length; j++) {
    Serial.print((char)payload[j]);
  }
  Serial.println();
}

void setup() {
  pinMode(pinWiper, OUTPUT);
  pinMode(pinKlakson, OUTPUT);
  pinMode(pinLamUtama, OUTPUT);
  pcf8574.pinMode(pinSenKiri, OUTPUT);
  pcf8574.pinMode(pinSenKanan, OUTPUT);
  pcf8574.pinMode(t_LamUtama, INPUT);
  pcf8574.pinMode(t_SenKanan, INPUT);
  pcf8574.pinMode(t_SenKiri, INPUT);
  pcf8574.pinMode(t_Klakson, INPUT);
  pcf8574.pinMode(t_Wiper, INPUT);

  pcf8574.digitalWrite(pinSenKanan, HIGH);
  pcf8574.digitalWrite(pinSenKiri, HIGH);
  digitalWrite(pinLamUtama, HIGH);
  digitalWrite(pinKlakson, HIGH);
  digitalWrite(pinWiper, HIGH);

  WiFi.begin(ssid, pass);
  Serial.begin(115200);
  NextSerial.begin(115200);
  delay(300);

  Serial.println("\nUniversitas Tanjugpura Electric Car Team - KAPUAS 2\n");
  Serial.println("Inisialisasi IC PCF8574...");
  if (pcf8574.begin()) {
    Serial.println("Berhasil");
  }
  else {
    Serial.println("Inisialisasi Gagal");
  }
  delay(200);

  JikaWiFiTerhubung = WiFi.onStationModeGotIP(WiFi_Terhubung);
  JikaWiFiTerputus  = WiFi.onStationModeDisconnected(WiFi_Terputus);

  timeClient.begin();
  timeClient.setTimeOffset(GMT);
  ubidots.setDebug(true);
  ubidots.begin(callback);
  delay(500);
}

void loop() {
  if (Serial.available() > 0) {
    BacaData = Serial.read();
    PesanData += BacaData;
    if (BacaData == '\n') {
      Data[0] = getValue(PesanData, ',', 0);
      Data[1] = getValue(PesanData, ',', 1);
      Data[2] = getValue(PesanData, ',', 2);
      Data[3] = getValue(PesanData, ',', 3);
      Data[4] = getValue(PesanData, ',', 4);
      Data[5] = getValue(PesanData, ',', 5);
      Data[6] = getValue(PesanData, ',', 6);

      V = Data[0].toFloat();
      I = Data[1].toFloat();
      P = Data[2].toFloat();
      T = Data[3].toFloat();
      v = Data[4].toInt();
      W = Data[5].toFloat();
      E = Data[6].toInt();

      PesanData = "";
      Serial.println("Volt    : " + Data[0] + "V      \t||\t " + "Arus   : " + Data[1] + "A");
      Serial.println("Daya    : " + Data[2] + "W      \t||\t " + "Suhu   : " + Data[3] + "C");
      Serial.println("Kecp    : " + Data[4] + "KM/Jam \t||\t " + "Energi : " + Data[5] + "Wh");
      Serial.println("Baterai : " + Data[6] + "%");
      Serial.println();
    }
  }

  if (Time(1, 5)) {
    nilaiKiri = pcf8574.digitalRead(t_SenKiri);
    nilaiKanan = pcf8574.digitalRead(t_SenKanan);
    nilaiWiper = pcf8574.digitalRead(t_Wiper);
    nilaiKlakson = pcf8574.digitalRead(t_Klakson);
    sets(1);
  }

  wiper();
  klakson();
  lampu_sen();
  lamUtama();

  if (WiFi.status() == WL_CONNECTED) {
    if (Time(2, 1)) {
      DataCloud();
      sets(2);
    }
    if (Time(3, 1)) {
      DataCloudDua();
      sets(3);
    }
    if (Time(4, 0)) {
      CuacaOpenWeather();
      sets(4);
    }
    if (Time(5, 3)) {
      Serial.println();
      WaktuNTP();
      sets(5);
    }
  }
  if (Time(6, 3)) {
    NextionDisplay();
    Serial.println("SenKiri = " + String(x) + ", SenKanan = " + String(y) + ", Hazard = " + String(z) + ", Wiper = " + String(wi) + ", Klakson = " + String(kl));
    sets(6);
  }
}

String getValue(String data, char separator, int index) {
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length() - 1;
  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void wiper() {
  if (nilaiWiper == 0 && Time(7, 5)) {
    wi = "1";
    digitalWrite(pinWiper, LOW);
    sets(7);
  }
  else if (nilaiWiper == 1 && Time(7, 5)) {
    wi = "0";
    digitalWrite(pinWiper, HIGH);
    sets(7);
  }
}

void lampu_sen() {
  if (nilaiKanan == 1 && nilaiKiri == 0 && Time(8, 3)) {      // Kondisi Lampu Sen Kanan Mati dan Sen Kiri Blink
    pcf8574.digitalWrite(pinSenKanan, HIGH);                  // Lampu Sen Kanan Mati
    if (b == false) {
      pcf8574.digitalWrite(pinSenKiri, LOW);                  // Lampu Sen Kiri Hidup
      b = true;
      NxSenKiri = "1";
      NxSenKanan = "0";
    }
    else if (b == true) {
      pcf8574.digitalWrite(pinSenKiri, HIGH);                 // Lampu Sen Kiri Mati
      b = false;
      NxSenKiri = "0";
      NxSenKanan = "0";
    }
    x = "1"; y = "0"; z = "0";
    sets(8);
  }
  else if (nilaiKanan == 0 && nilaiKiri == 1 && Time(8, 3)) { // Kondisi Lampu Sen Kanan Blink dan Sen Kiri Mati
    pcf8574.digitalWrite(pinSenKiri, HIGH);                   // Lampu Sen Kiri Mati
    if (c == false) {
      pcf8574.digitalWrite(pinSenKanan, LOW);                 // Lampu Sen Kanan Hidup
      c = true;
      NxSenKiri = "0";
      NxSenKanan = "1";
    }
    else if (c == true) {
      pcf8574.digitalWrite(pinSenKanan, HIGH);                // Lampu Sen Kiri Mati
      c = false;
      NxSenKiri = "0";
      NxSenKanan = "0";
    }
    x = "0"; y = "1"; z = "0";
    sets(8);
  }
  else if (nilaiKanan == 0 && nilaiKiri == 0 && Time(8, 3)) { // Kondisi Lampu Sen Kanan Blink dan Sen Kiri Blink
    if (d == false) {
      pcf8574.digitalWrite(pinSenKanan, LOW);                 // Lampu Sen Kanan Hidup
      pcf8574.digitalWrite(pinSenKiri, LOW);                  // Lampu Sen Kiri Hidup
      d = true;
      NxSenKiri = "1";
      NxSenKanan = "1";
    }
    else if (d == true) {
      pcf8574.digitalWrite(pinSenKanan, HIGH);                // Lampu Sen Kanan Mati
      pcf8574.digitalWrite(pinSenKiri, HIGH);                 // Lampu Sen Kiri Mati
      d = false;
      NxSenKiri = "0";
      NxSenKanan = "0";
    }
    x = "0"; y = "0"; z = "1";
    sets(8);
  }
  else if (nilaiKanan == 1 && nilaiKiri == 1 && Time(8, 3)) { // Kondisi Lampu Sen Kanan Mati dan Sen Kiri Mati
    pcf8574.digitalWrite(pinSenKanan, HIGH);                  // Lampu Sen Kanan Mati
    pcf8574.digitalWrite(pinSenKiri, HIGH);                   // Lampu Sen Kiri Mati
    NxSenKiri = "0";
    NxSenKanan = "0";
    x = "0"; y = "0"; z = "0";
    sets(8);
  }
}


void klakson() {
  if (nilaiKlakson == 0 && Time(9, 6)) {                       // Kondisi Klakson Aktif
    digitalWrite(pinKlakson, LOW);
    kl = "1";
    sets(9);
  }
  else if (nilaiKlakson == 1 && Time(9, 6)) {                  // Kondisi Klakson Nonaktif
    digitalWrite(pinKlakson, HIGH);
    kl = "0";
    sets(9);
  }
}

void lamUtama() {
  if (nilaiLampu == 0 && Time(10, 6)) {
    digitalWrite(pinLamUtama, LOW);
    lu = "1";
    sets(10);
  }
  else if (nilaiLampu == 1 && Time(10, 6)) {
    digitalWrite(pinLamUtama, HIGH);
    lu = "0";
    sets(10);
  }
}

void NextionDisplay() {
  if (WiFi.status() == WL_CONNECTED) {
    NextionWrite();
    String NxSekarang = "page1.Tanggal.txt=\"" + SkrgTanggal + "\"";
    NextSerial.print(NxSekarang);
    NextionWrite();
    String NxJam = "page1.Jam.txt=\"" + SkrgJam + "\"";
    NextSerial.print(NxJam);
    NextionWrite();
    String NxSuhuDepan = "page1.DerajatSuhu.val=" + TemC;
    NextSerial.print(NxSuhuDepan);
    NextionWrite();
    String NxLogoCuaca = "page1.n0.val=" + logo;
    NextSerial.print(NxLogoCuaca);
    NextionWrite();

    String NxSuhuDalam = "page6.TemCu.txt=\"" + TemC + " C" + "\""; //°
    NextSerial.print(NxSuhuDalam);
    NextionWrite();
    String NxTek = "page6.TekCu.txt=\"" + Tek + " mBar" + "\"";
    NextSerial.print(NxTek);
    NextionWrite();
    String NxKel = "page6.KelCu.txt=\"" + Kel + " %" + "\"";
    NextSerial.print(NxKel);
    NextionWrite();
    String NxKec = "page6.KecCu.txt=\"" + Kec + " m/s" + "\"";
    NextSerial.print(NxKec);
    NextionWrite();
    String NxDeskripsiCuaca = "page6.Cuaca.txt=\"" + deskripsi + "\"";;
    NextSerial.print(NxDeskripsiCuaca);
    NextionWrite();
  }

  NextionWrite();
  String NxTeBat = "page5.TeBat.txt=\"" + String(V) + " V" + "\"";
  NextSerial.print(NxTeBat);
  NextionWrite();
  String NxArusBat = "page5.ArusBat.txt=\"" + String(I) + " A" + "\"";
  NextSerial.print(NxArusBat);
  NextionWrite();
  String NxDayBat = "page5.DayBat.txt=\"" + String(P) + " W" + "\"";
  NextSerial.print(NxDayBat);
  NextionWrite();
  String NxEnBat = "page5.EnBat.txt=\"" + String(W) + " Wh" + "\"";
  NextSerial.print(NxEnBat);
  NextionWrite();
  String NxPersenBaterai = "page1.PersenBaterai.val=" + String(E);
  NextSerial.print(NxPersenBaterai);
  NextionWrite();

  String NxSuhuMesin = "page4.SuhuMesin.txt=\"" + String(T) + " C" + "\"";
  NextSerial.print(NxSuhuMesin);
  NextionWrite();

  String NxKecepatan = "page2.SpeedVal.val=" + String(v);
  NextSerial.print(NxKecepatan);
  NextionWrite();
  String NxKananSen = "page2.KananSen.val=" + NxSenKanan;
  NextSerial.print(NxKananSen);
  NextionWrite();
  String NxKiriSen = "page2.KiriSen.val=" + NxSenKiri;
  NextSerial.print(NxKiriSen);
  NextionWrite();
  NextSerial.println();
}

void DataCloud() {
  ubidots.add("Tegangan_Baterai", V);
  ubidots.add("Arus_Baterai", I);
  ubidots.add("Daya_Baterai", P);
  ubidots.add("Suhu_Mesin", T);
  ubidots.add("Kacepatan_Mobil", v);
  ubidots.ubidotsPublish("Kapuas_2_Data1");
  ubidots.loop();
}

void DataCloudDua() {
  ubidots.add("Energi_Terpakai", W);
  ubidots.add("Persen_Baterai", E);
  ubidots.ubidotsPublish("Kapuas_2_Data1");
  ubidots.loop();
}

void CuacaOpenWeather() {
  String serverPath = "http://api.openweathermap.org/data/2.5/weather?q=" + Kota + "," + KodeNegara + "&APPID=" + APIOpenWeather;
  jsonBuffer = httpGETRequest(serverPath.c_str());
  JSONVar myObject = JSON.parse(jsonBuffer);
  if (JSON.typeof(myObject) == "undefined") {
    Serial.println("Gagal memasukkan parse!");
    return;
  }
  int KonversiC = int(myObject["main"]["temp"]) - 273;
  TemC = String(KonversiC);
  int Teka = int(myObject["main"]["pressure"]);
  Tek = String(Teka);
  int Kele = int(myObject["main"]["humidity"]);
  Kel = String(Kele);
  int Kece = int(myObject["wind"]["speed"]);
  Kec = String(Kece);
  ikon = JSON.stringify(myObject["weather"][0]["icon"]);
  if (ikon == "01d") {
    logo = "0";
    deskripsi = "CERAH";
  }
  if (ikon == "01n") {
    logo = "4";
    deskripsi = "CERAH";
  }
  if (ikon == "02d" || ikon == "02n" || ikon == "03d" || ikon == "03n" || ikon == "04d" || ikon == "04n") {
    logo = "1";
    deskripsi = "BERAWAN";
  }
  if (ikon == "09d" || ikon == "09n" || ikon == "10d" || ikon == "10n" || ikon == "11d" || ikon == "11n" || ikon == "13d" || ikon == "13n") {
    logo = "2";
    deskripsi = "HUJAN";
  }
  if (ikon == "50d" || ikon == "50n") {
    logo = "3";
    deskripsi = "KABUT";
  }
}

void WaktuNTP() {
  timeClient.update();
  unsigned long epochTime = timeClient.getEpochTime();
  struct tm *ptm = gmtime ((time_t *)&epochTime);
  int Tanggal = ptm->tm_mday;
  int Bulan_ke = ptm->tm_mon + 1;
  String Bulan = DaftarBulan[Bulan_ke - 1];
  int Tahun = ptm->tm_year + 1900;

  SkrgJam = timeClient.getFormattedTime();
  SkrgTanggal = String(Tanggal) + " " + String(Bulan) + " " + String(Tahun);
}

void NextionWrite() {
  NextSerial.write(0xff);
  NextSerial.write(0xff);
  NextSerial.write(0xff);
}

String httpGETRequest(const char* serverName) {
  WiFiClient client;
  HTTPClient http;
  http.begin(client, serverName);
  int httpResponseCode = http.GET();
  String payload = "{}";
  if (httpResponseCode > 0) {
    Serial.print("Terhubung dengan kode HTTP Response: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    Serial.print("Kode Error: ");
    Serial.println(httpResponseCode);
  }
  http.end();
  return payload;
}

void WiFi_Terhubung(const WiFiEventStationModeGotIP& event) {
  Serial.println("Berhasil terhubung ke WiFi.");
  Serial.print("Alamat IP: ");
  Serial.println(WiFi.localIP());
}

void WiFi_Terputus(const WiFiEventStationModeDisconnected& event) {
  Serial.println("Koneksi WiFi terputus, menghubungkan kembali...");
  WiFi.disconnect();
  WiFi.begin(ssid, pass);
}
