float Vstm; // tegangan terbaca pada stm32 antara 0-3.3V
float Vreal; // tegangan sesungguhnya yaitu antara 0-60V
float nilai; // nilai bit terbaca
float Vkalibrasi;
int stPin = PB1; // pin adc yg dipakai untuk membaca tegangan
int potPin = PB0;
int statePin = 0;

void setup()
{
  pinMode(stPin, INPUT);
  Serial.begin(9600);
 }
void loop()
{
      nilai=analogRead(stPin); // pembacaan pin
      statePin = analogRead(potPin);
      Vstm=((nilai/4095)*3.3);
      Vreal = Vstm * (661+37)/37;
      Vkalibrasi = Vreal/1.09922;
      Serial.print("Data : ");
      Serial.print(nilai);
      Serial.println(" bit");
      Serial.print("Volt : ");
      Serial.print(Vstm);
      Serial.println(" V (STM32)");
      Serial.print("Volt : ");
      Serial.print(Vreal);
      Serial.println(" V-real");
      Serial.print("Volt Kalibrasi : ");
      Serial.print(Vkalibrasi);
      Serial.println(" V-kalibrasi");
      Serial.print("Nilai Pot : ");
      Serial.println(statePin);
      Serial.println("\n");
   delay(statePin);
}
