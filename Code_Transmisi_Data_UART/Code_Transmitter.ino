unsigned long oldtime = 0;
unsigned long interval = 500;

float V = 0;
float I = 0;
float P = 0;
float T = 0;
float v = 0;
float E = 0;
float W = 0;

float sdata1 = 0; // tegangan
float sdata2 = 0; // arus
float sdata3 = 0; // daya
float sdata4 = 0; // suhu
float sdata5 = 0; // kecepatan
float sdata6 = 0; // energi
float sdata7 = 0; // energi

String cdata;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200); //Serial komunikasi untuk dihubungkan ke laptop/PC dengan baud rate 115200
  Serial2.begin(115200); //Serial komunikasi HardwareSerial dari Serial3 untuk dihubungkan ke mikrokontroler lain pada baud rate 115200
  delay(100);
}

void loop() {
  // put your main code here, to run repeatedly:
  V = random(2, 300);
  I = 111;
  P = 123;
  T = 45;
  v = random(6, 90);
  W = random(1, 60);
  E = 222;
  
  sdata1 = V;
  sdata2 = I;
  sdata3 = P;
  sdata4 = T;
  sdata5 = v;
  sdata6 = W;
  sdata7 = E;

   if((millis() - oldtime) >= interval){
    cdata = String(sdata1) + "," + String(sdata2) + "," + String(sdata3) + ","  + String(sdata4) + "," + String(sdata5) + "," + String(sdata6) + "," + String(sdata7); 
     Serial.println(cdata); //Mengirimkan data ke laptop/PC untuk ditampilkan pada serial monitor
     Serial2.println(cdata); //Mengirimkan data ke mikrokontroler lain melalui Serial Komunikasi HardwareSerial dari Serial3
     oldtime = millis();
   }
   cdata = "";
}
