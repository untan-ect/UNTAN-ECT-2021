#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <WiFiClient.h>
#include <Arduino_JSON.h>
#include <SoftwareSerial.h>

SoftwareSerial NextSerial(13, 15);                  //RX, TX (Pin D7 dan D8)

const char *ssid     = "Nama_WiFi";
const char *password = "Sandi_WiFi";

String openWeatherMapApiKey = "b23308e70faf9521f7bce63d9ecc393e";              // Didapat setelah login ke openWeather
String city                 = "Pontianak";
String countryCode          = "ID";

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");
const int GMT = 25200;

String DaftarHari[7]={"Minggu", "Senin", "Selasa", "Rabu", "Kamis", "Jum'at", "Sabtu"};
String DaftarBulan[12]={"Jan", "Feb", "Mar", "Apr", "Mei", "Jun", "Jul", "Agu", "Sep", "Okt", "Nov", "Des"};

unsigned long WTCuaca, WTWaktu, WTDisplay = 0;
unsigned long JedaCuaca                   = 630000;                                    
unsigned long JedaWaktu                   = 500;
unsigned long JedaDisplay                 = 1000;                                                           

String jsonBuffer, logo, icon, deskripsi;
String SkrgTanggal, SkrgJam;
String Kec, Kel, Tek, TemC;

void NextionDisplay(){
      NextionWrite();
      String NxSekarang = "Tanggal.txt=\""+ SkrgTanggal+"\"";
      NextSerial.print(NxSekarang);    
      NextionWrite();
      String NxJam = "Jam.txt=\""+SkrgJam+"\"";
      NextSerial.print(NxJam);
      NextionWrite();
      String NxSuhuDepan = "page1.DerajatSuhu.val="+ TemC;
      NextSerial.print(NxSuhuDepan);
      NextionWrite();
      String NxSuhuDalam ="page6.TemCu.txt=\""+TemC+" C"+"\""; //°
      NextSerial.print(NxSuhuDalam);
      NextionWrite();
      String NxTek = "page6.TekCu.txt=\""+Tek+" mBar"+"\"";
      NextSerial.print(NxTek);
      NextionWrite();
      String NxKel = "page6.KelCu.txt=\""+Kel+" %"+"\"";
      NextSerial.print(NxKel);
      NextionWrite();
      String NxKec = "page6.KecCu.txt=\""+Kec+" m/s"+"\"";
      NextSerial.print(NxKec);
      NextionWrite();
      
      String NxDeskripsiCuaca = "page6.Cuaca.txt=\""+deskripsi+"\"";;
      NextSerial.print(NxDeskripsiCuaca);
      NextionWrite();
      String NxLogoCuaca = "page1.n0.val="+logo;
      NextSerial.print(NxLogoCuaca);
      NextionWrite();
      Serial.println();
}

void CuacaOpenWeather(){
      String serverPath = "http://api.openweathermap.org/data/2.5/weather?q=" + city + "," + countryCode + "&APPID=" + openWeatherMapApiKey;
      jsonBuffer = httpGETRequest(serverPath.c_str());
      JSONVar myObject = JSON.parse(jsonBuffer);  
      if (JSON.typeof(myObject) == "undefined") {
        Serial.println("Parsing input failed!");
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
      icon = myObject["weather"][0]["icon"];
       if(icon == "01d"){
          logo = "0";
          deskripsi = "CERAH";
          }
       if(icon == "01n"){
          logo = "4";
          deskripsi = "CERAH";
          }
       if(icon == "02d" || icon == "02n" || icon == "03d" || icon == "03n" || icon == "04d" || icon == "04n"){
          logo = "1";
          deskripsi = "BERAWAN";
          }
       if(icon == "09d" || icon == "09n" || icon == "10d" || icon == "10n" || icon == "11d" || icon == "11n" || icon == "13d" || icon == "13n"){
          logo = "2";
          deskripsi = "HUJAN";
          }
       if(icon == "50d" || icon == "50n"){
          logo = "3"; 
          deskripsi = "KABUT";
          }
}

void WaktuNTP(){
  timeClient.update();
  unsigned long epochTime = timeClient.getEpochTime();
  struct tm *ptm = gmtime ((time_t *)&epochTime); 
  int Tanggal = ptm->tm_mday;
  int Bulan_ke = ptm->tm_mon+1;
  String Bulan = DaftarBulan[Bulan_ke-1];
  int Tahun = ptm->tm_year+1900;
  
  SkrgJam = timeClient.getFormattedTime();
  SkrgTanggal = String(Tanggal) + " " + String(Bulan) + " " + String(Tahun);
}

void NextionWrite(){
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
  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  http.end();
  return payload;
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid,password );
  Serial.print("Menghubungkan.");
  while(WiFi.status()!= WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Terhubung ke WiFi dengan alamat IP: ");
  Serial.println(WiFi.localIP());

  NextSerial.begin(115200);

  timeClient.begin();
  timeClient.setTimeOffset(GMT);

  WaktuNTP();
  CuacaOpenWeather();
}

void loop() {
  if ((millis() - WTCuaca) > JedaCuaca) {
      CuacaOpenWeather();
      WTCuaca = millis();
  }
  if ((millis() - WTWaktu) > JedaWaktu) {
      WaktuNTP();
      WTWaktu = millis();
  }
  if ((millis() - WTDisplay) > JedaDisplay) {
      NextionDisplay();
      WTDisplay = millis();
  }
}
