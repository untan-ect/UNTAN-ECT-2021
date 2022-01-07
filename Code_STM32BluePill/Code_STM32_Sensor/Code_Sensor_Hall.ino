float rev = 0; //Deklarasi nilai variabel rev = 0
float rpm; //Deklarasi Variabel  
float time; //Deklarasi Variabel time
float Speed = 0; //Deklarasi nilai variabel Speed = 0
float Kilometer = 0;
unsigned long oldtime = 0; //Deklarasi nilai variabel oldtime = 0    

void isr()          //interrupt service routine
{
  rev++; //Penambahan nilai variabel setiap interupsi
  int poll = rev/24; // 24 adalah jumlah magnet dalam satu putaran
}

void setup()
{
  pinMode(PA11 ,INPUT); // Mendeklarasikan PIN PA11 sebagai  INPUT
  pinMode(PA7, INPUT);
  pinMode(PA6, INPUT);
  attachInterrupt(digitalPinToInterrupt(PA11),isr,RISING);  
  attachInterrupt(digitalPinToInterrupt(PA7),isr,RISING);  
  attachInterrupt(digitalPinToInterrupt(PA6),isr,RISING);  
  Serial.begin(9600); //Untuk menampilkan diserial monitor

}

void loop()
{
  delay(2000);// Penundaan 2 detik
  detachInterrupt(digitalPinToInterrupt(PA11)); 
  detachInterrupt(digitalPinToInterrupt(PA7)); 
  detachInterrupt(digitalPinToInterrupt(PA6)); 

  time = millis() - oldtime; // Waktu sekarang dikurang waktu sebelumnya
  rpm = (poll/time)*60000;   // Penjumlahan
  oldtime = millis();        //Menyimpan waktu saat ini
  rev = 0;
  poll = 0;
  Speed = (rpm*2*3.14*0.3)/60;
  Kilometer = Speed*3,6;

  Serial.println("-------------"); //Menampilkan ----- diserial monitor 
  Serial.print("RPM : "); //Menampilkan RPM diserial monitor
  Serial.println(rpm); //Menampilkan nilai rpm di serial monitor
  Serial.println("-------------"); //Menampilkan ----- diserial monitor 
  Serial.print("Speed : "); //Menampilkan Speed diserial monitor
  Serial.print(Speed); //Menampilkan nilai rpm di serial monitor
  Serial.println(" m/s"); //Menampilkan Speed diserial monitor
  Serial.print("Kilometer : "); //Menampilkan Speed diserial monitor
  Serial.print(Kilometer); //Menampilkan nilai Speed diserial monitor
  Serial.println(" Km/jam"); //Menampilkan Speed diserial monitor
  Serial.println("\n\n"); //Menampilkan Spasi 2x ke bawah diserial monitor

  attachInterrupt(PA11,isr,RISING); //interrupt akan dieksekusi jika terjadi perubahan nilai pembacaan dari LOW ke HIGH( dieksekusi 1 kali pas terjadi perubahan )
  attachInterrupt(PA7,isr1,RISING);
  attachInterrupt(PA6,isr1,RISING);
}
