#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <Arduino_JSON.h>
#include <UbidotsESPMQTT.h>
#include <Arduino.h>
#include <SoftwareSerial.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h> 

SoftwareSerial mySerial(10, 15); // RX (Receiver), TX (Transmitter) // (S3, D8)
WiFiClient client;

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Deklarasi untuk SSD1306 display terhubung ke I2C (pin SDA dan SCL)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

char token[]           = "BBFF-lJhOBUcVPa4rnl2Hxnf5DLPHJZUnqX";
char ssid[]            = "Redmi Not 8";
char pass[]            = "indakaku";

unsigned long WTDisplay, WTGetData = 0;
int WTCloud     = -10000;
int WTCloudDua  = -8000;

unsigned long oldtime[11];
unsigned long interval[7] = {630000, 10000, 1000, 500, 250, 30, 100};

String Data[7];
String PesanData;
char BacaData;
int v, E = 0;
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
  WiFi.begin(ssid, pass);
  Serial.begin(115200);
  mySerial.begin(115200);
  delay(300);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Clear the buffer
  display.clearDisplay();
  display.display();

  JikaWiFiTerhubung = WiFi.onStationModeGotIP(WiFi_Terhubung);
  JikaWiFiTerputus  = WiFi.onStationModeDisconnected(WiFi_Terputus);

  ubidots.setDebug(true);
  ubidots.begin(callback);
  delay(500);
}

void loop() {
  if (mySerial.available() > 0) {
    BacaData = mySerial.read();
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

  if (WiFi.status() == WL_CONNECTED) {
    if (Time(0, 1)) {
      DataCloud();
      sets(0);
    }
    if (Time(2, 1)) {
      DataCloudDua();
      sets(2);
    }
  }
  if (Time(3, 3)) {
    displayOLED();
    sets(3);
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

void displayOLED() {
  display.clearDisplay();
  display.display();

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("   Data Nilai Mobil"); //Menampilkan kata pada baris 1
  display.println("---------------------"); //Menampilkan kata pada baris 2

  //Kolom 1 baris 3
  //Menampilkan nilai variabel V di LCD Oled
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 16);
  display.println("V");
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(8, 16);
  display.println(":");
  display.setCursor(16, 16);
  display.setTextSize(1);
  display.print(V, 1);

  //Kolom 1 Baris 4
  //Menampilkan nilai variabel I di LCD Oled
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 28);
  display.println("I");
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(8, 28);
  display.println(":");
  display.setCursor(16, 28);
  display.setTextSize(1);
  display.print(I, 1);

  //Kolom 1 Baris 5
  //Menampilkan nilai variabel P di LCD Oled
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 40);
  display.println("P");
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(8, 40);
  display.println(":");
  display.setCursor(16, 40);
  display.setTextSize(1);
  display.print(P, 1);

  //Kolom 2 Baris 3
  //Menampilkan nilai variabel T di LCD Oled
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(70, 16);
  display.println("T");
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(78, 16);
  display.println(":");
  display.setCursor(86, 16);
  display.setTextSize(1);
  display.print(T, 1);

  //Kolom 2 Baris 4
  //Menampilkan nilai variabel v di LCD Oled
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(70, 28);
  display.println("v");
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(78, 28);
  display.println(":");
  display.setCursor(86, 28);
  display.setTextSize(1);
  display.print(v, 1);

  //Kolom 2 Baris 5
  //Menampilkan nilai variabel E di LCD Oled
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(70, 40);
  display.println("E");
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(78, 40);
  display.println(":");
  display.setCursor(86, 40);
  display.setTextSize(1);
  display.print(E, 1);
  display.display();
  delay(100);
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
