void Sensor_Tegangan() {
  if (Time(3, 1)) {
    for (int j = 1; j <= 500; j++) {
      nilai = analogRead(Volt); // pembacaan pin
      Vstm = (nilai / 4095) * 3.24;
      Vreal = Vstm * (680 + 39) / 39;
      V += Vreal;
    }
    V /= 500;
    sets(3);
  }
}

void Sensor_Arus() {
  if (Time(4, 1)) {
    for (int i = 1; i <= iterasi; i++)
    {
      nilaiSensor = analogRead(Arus);
      voltage = (nilaiSensor) * (teganganReferensi / (pow(2, bitResolusi) - 1)) - (0.5 * teganganReferensi) + nilaiKoreksi ;
      current  = voltage / 66.0  ;
      I += current;
    }

    I /= iterasi;
    I = I - 15.65;
    if (I < 0) {
      I = 0;
    }
    sets(4);
  }
  //Serial.println("Arus : " + String(I));
  //Serial.println("nilaiSensor : " + String(nilaiSensor));
}

void Sensor_Temperatur() {
  if (Time(5, 1)) {
    //Data sensor suhu
    val = analogRead(Suhu);
    mv = ( val / 4095) * 3300;
    T = mv / 10;
    sets(5);
  }
}

void Sensor_Kecepatan() {
  if (Time(6, 3)) {
    detachInterrupt(digitalPinToInterrupt(Speed));
    time = millis() - timeold;
    rpm = (rev / time) * 60000;
    timeold = millis();
    KEC = (rpm * 2 * 3.14 * 0.28) / 60;
    v = KEC * 3.6;
    rotasi = 0;
    rev = 0;
    attachInterrupt(Speed, isr, RISING);
    sets(6);
  }
}
