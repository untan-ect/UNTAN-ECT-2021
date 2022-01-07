#define Volt PB1
#define Arus PB0
#define Suhu PA7
#define Infra PA6
#define pedal PA0

unsigned long oldtime = 0, oldtime1 = 0, oldtime2 = 0, oldtime3 = 0, oldtime4 = 0, oldtime5 = 0, oldtime6 = 0;
unsigned long interval = 1000, interval1 = 2000, interval2 = 60000;
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
float I = 0;

//Deklarasi Variabel Daya
float P = 0;

//Deklarasi Sensor suhu
float val = 0;
float mv = 0;
float T = 0;

//Deklarasi Sensor Hall
int pi = 3.14;
int r = 0.28;
int rev = 0; //Deklarasi nilai variabel rev = 0
float rpm; //Deklarasi Variabel
float timeold = 0; //Deklarasi nilai variabel oldtime = 0      
float Speed = 0; //Deklarasi nilai variabel Speed = 0
float rotasi;
float time; //Deklarasi Variabel time
float v = 0; //Kecepatan KM/Jam

//Deklarasi Energi yang yang terpakai
float E = 0;
int waktu = 0;

String sdata1, sdata2, sdata3, sdata4, sdata5, sdata6; // energi

void isr()          //interrupt service routine
{
  rev++;
}

void setup() {
  // put your setup code here, to run once:
  pinMode(Arus, INPUT);
  pinMode(Infra, INPUT);
  Serial.begin(115200);
  attachInterrupt(Infra, isr, RISING);
  delay(10);
}

void Sensor_Tegangan(){
  if((millis() - oldtime1) >= interval){
    nilai=analogRead(Volt); // pembacaan pin
    Vstm=((nilai/4095)*3.3);
    Vreal = Vstm * (726+38.2)/38.2;
    V = Vreal;
    oldtime1 = millis();
  }
}

void Sensor_Arus(){
  int nilaiSensor;
  float voltage, current;
  
  if((millis() - oldtime2) >= interval){
    for(int i=0; i<= iterasi; i++)
      {   
        nilaiSensor = analogRead(Arus);
          voltage = (nilaiSensor) * (teganganReferensi / (pow(2,bitResolusi)-1)) - ( 0.5 * teganganReferensi) + nilaiKoreksi ; 
          current  = voltage / 66.0  ;
          I += current;
    }
    oldtime2 = millis();
  }
  I /= iterasi;
  I += 3.75;
  if(I < 0){
    I = 0;
  }
}

void Daya(){
  if((millis() - oldtime3) >= interval){
    Sensor_Tegangan();
    Sensor_Arus();
    P = I*V;
    oldtime3 = millis();
  }
}

void Sensor_Temperatur(){
  if((millis() - oldtime4) >= interval){
    //Data sensor suhu
    val = analogRead(Suhu);
    mv = ( val/4095.0)*3300;
    T = mv/10;
    oldtime4 = millis();
  }
}

void Sensor_Kecepatan(){
  if((millis() - oldtime5) >= interval1){
    detachInterrupt(Infra);
    time = millis() - timeold;
    rpm = (rev/time)*60000;
    timeold = millis();
    rotasi = 0;
    rev = 0;
    Speed = rpm*((2*pi*r)/60);
    v = Speed*3.6;
    attachInterrupt(Infra, isr, RISING);
    oldtime5 = millis();
  }
}

void Energi(){
  if((millis() - oldtime6) >= interval){
    Daya();
    waktu++;
    E = P*(waktu/3600);
    oldtime6 = millis();
  }
}

void Data() {
  Sensor_Tegangan();
  Sensor_Arus();
  Daya();
  Sensor_Temperatur();
  Sensor_Kecepatan();
  Energi();
   
  sdata1 = V;
  sdata2 = I;
  sdata3 = P;
  sdata4 = T;
  sdata5 = v;
  sdata6 = E;
  
  Serial.println("----------------------");
  Serial.print("Tegangan V = ");
  Serial.println(sdata1 );
  Serial.print("Arus I = ");
  Serial.println(sdata2 );
  Serial.print("Daya P = ");
  Serial.println(sdata3 );
  Serial.print("Temperatur T = ");
  Serial.println(sdata4 );
  Serial.print("Kecepatan v = ");
  Serial.println(sdata5 );
  Serial.print ("Energi E = ");
  Serial.println(sdata6 );
  Serial.println("----------------------");
}

void loop() {
  // put your main code here, to run repeatedly
  if((millis() - oldtime) >= interval){
    Data();
    oldtime = millis();
  }
}
