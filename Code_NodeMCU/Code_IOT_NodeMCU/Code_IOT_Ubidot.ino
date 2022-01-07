#include "UbidotsESPMQTT.h"

#define token "BBFF-lJhOBUcVPa4rnl2Hxnf5DLPHJZUnqX"     // Token ubidots yang didapat setelah membuat akun
#define ssid "Valdi Honor"                              // SSID WiFi yang digunakan
#define pass "mymommy64"                                // Password WiFi yang digunakan

int a = 2, b, c, d, e, f;
int WS, WSsatu = 0;

Ubidots ubidots(token);

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void setup() {
  Serial.begin(115200);
  ubidots.setDebug(true); 
  ubidots.wifiConnection(ssid, pass);
  ubidots.begin(callback);
}

void loop() {
  if (!ubidots.connected()) {
    ubidots.reconnect();
  }
  if ( millis() - WS >= 5000 ) {
// Publish nilai ke 2 sumber data yang berbeda
  a = a + 1;
  b = a + 1;
  c = b + 1; 
  d = c + 1;
  e = d + 1;
  f = f + 1;
  WS = millis();

// Masukkan variabel label dan nilainya yang akan dikirim/diperbarui - Maksimal hanya 5 variabel dalam 1 kali pengiriman
  ubidots.add("nilai a", a); 
  ubidots.add("nilai b", b);
  ubidots.add("nilai c", c);
  ubidots.ubidotsPublish("sumberData1");
  ubidots.add("nilai d", d);
  ubidots.add("nilai e", e);
  ubidots.add("nilai f", f);
  ubidots.ubidotsPublish("sumberData2");
  ubidots.loop(); 
  }
}


/*
  Referensi   : https://help.ubidots.com/en/articles/513324-connect-a-nodemcu-esp8266-to-ubidots-over-mqtt
  Library     : https://github.com/ubidots/ubidots-mqtt-esp
*/
