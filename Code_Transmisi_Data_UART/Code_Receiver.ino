String myString; // complete message from arduino, which consistors of snesors data
char rdata; // menerima karakter

//Deklarasi Millis
unsigned long oldtime = 0;
unsigned long interval = 5000;

//Dekalrasi Variabel
float V, I, P, T, v, E;

void setup() {
  Serial.begin(115200);
  delay(100);
}

void loop() {
  if (Serial.available() > 0)
  {
    rdata = Serial.read(); // Ambil data yang dibaca
    myString += rdata;
    if (rdata == '\n') {
      String l = getValue(myString, ',', 0);
      String m = getValue(myString, ',', 1);
      String n = getValue(myString, ',', 2);
      String o = getValue(myString, ',', 3);
      String p = getValue(myString, ',', 4);
      String q = getValue(myString, ',', 5);

      V = l.toFloat();
      I = m.toFloat();
      P = n.toFloat();
      T = o.toFloat();
      v = p.toFloat();
      E = q.toFloat();

      myString = "";
      Serial.println("-----------------------");

      Serial.println("Data Mobil Listrik KMHE");
      Serial.println("-----------------------");
      Serial.print("Data V : ");
      Serial.println(V);
      Serial.print("Data I : ");
      Serial.println(I);
      Serial.print("Data P : ");
      Serial.println(P);
      Serial.print("Data T : ");
      Serial.println(T);
      Serial.print("Data v : ");
      Serial.println(v);
      Serial.print("Data E : ");
      Serial.println(E);
      Serial.println("-----------------------");
      // end new code
      Serial.println();
    }
  }
}

String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
