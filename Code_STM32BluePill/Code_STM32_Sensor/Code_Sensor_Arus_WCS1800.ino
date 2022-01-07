#define nilaiKoreksi 164 //mA
#define iterasi 10000
#define teganganReferensi 5000.0 //5000mv is for 5V
#define bitResolusi 12

unsigned long oldtime1 = 0, oldtime2 = 0;

const int Arus = PA4;
const int potPin = PB0;
const int bldc = PB6;

void setup()
{
  Serial.begin(115200);
  pinMode(Arus, INPUT);
  pinMode(potPin, INPUT);
  pinMode(bldc, OUTPUT);
}

void loop()
{
    int nilaiSensor, pot;
    float hasilNilaiSensor = 0;
    float voltage, current;

    if((millis() - oldtime2) >= 1000){
      pot = analogRead(potPin)/16;
      analogWrite(bldc, pot);
      oldtime2 = millis();
    }
    if((millis() - oldtime1) >= 1000){
      for(int i=0; i<= iterasi; i++)
      {   
        nilaiSensor = analogRead(Arus);
          voltage = (nilaiSensor) * (teganganReferensi / (pow(2,bitResolusi)-1)) - ( 0.5 * teganganReferensi) + nilaiKoreksi ; 
          current  = voltage / 66.0  ;
          hasilNilaiSensor += current;
      }
      oldtime1 = millis();
    }
    hasilNilaiSensor /= iterasi;
    hasilNilaiSensor += 3.64;
    if(hasilNilaiSensor < 0){
      hasilNilaiSensor = 0;
    }
    Serial.println("Arus : " + String(hasilNilaiSensor));
    //Serial.println("nilaiSensor : " + String(nilaiSensor));
    delay(1000);
}
