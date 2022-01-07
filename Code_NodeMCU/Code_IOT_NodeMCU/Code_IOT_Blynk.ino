//Mendeklarasikan library atau perpustakaan
#include <BlynkSimpleEsp8266.h>
#include <ESP8266WiFi.h>
#define BLYNK_PRINT Serial

const int pot = A0;
int data = 0;

// Hostpot dan Konfigurasi Blynk
const char auth[] = "JkBH5KigBFGw0cpfckcvgbGUGQazQiM0";
const char ssid[] = "vivo 1807";
const char pass[] = "alfineokee";

WiFiClient client;

void setup() {
  Serial.begin(115200);
  delay(10);

  //Status sambungan ke wifi
  Serial.print("Tersambung ke Wifi: "); 
  Serial.println(ssid);
  //WiFi.begin(ssid); digunakan saat wifi tanpa password
  WiFi.begin(ssid, pass);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("\nWiFi connected\n"); 
  Blynk.begin(auth, ssid, pass);

}

void loop() {
   Blynk.run();
   data = analogRead(pot);
   Blynk.virtualWrite(V0, data); // Set pin virtual Blynk pada V0
}
