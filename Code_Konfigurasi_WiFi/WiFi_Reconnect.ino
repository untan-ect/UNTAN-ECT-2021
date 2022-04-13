/*
 * Untuk jaga-jaga saat ESP8266 terputus jaringan WiFi saat sedang berjalan
 * 13 Agustus 2021 - Home Sweet Home - Muhammad Revaldi Frizky
 */

#include <ESP8266WiFi.h>

const char* ssid = "Valdi Honor";
const char* pass = "mymommy64";

WiFiEventHandler JikaWiFiTerhubung;
WiFiEventHandler JikaWiFiTerputus;

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

void setup() {
  Serial.begin(115200);

  JikaWiFiTerhubung = WiFi.onStationModeGotIP(WiFi_Terhubung);
  JikaWiFiTerputus  = WiFi.onStationModeDisconnected(WiFi_Terputus);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  Serial.print("Menghubungkan ke WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("Alamat IP:");
  Serial.println(WiFi.localIP());
}

void loop() {
  delay(1000);
  Serial.println("Hiya..");
}

// Coba sekali-sekali matikan WiFi, lihat respon dari Serial Monitor
