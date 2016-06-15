/*
 *  read rfid and or 433 remote to unlock door
 *  rfid reader 125khz
 *  data from variable
 */

#include <RCSwitch.h>                     //library for remote control
RCSwitch mySwitch = RCSwitch();           // mySwitch object  dari class RCSwitch
//for rfid
int i;
char c;                                   //data sementara tag rfid
unsigned long previousMillis = 0;  

String newtag = "";
String tag1 = "38002558EAAF";           //data kartu tersimpan
String tag2 = "380039F49A6F";

unsigned long rfremote = 2254946;   // data remote tersimpan
unsigned long rfremotenewtag;

int pinbuzzer = A5 ;
int pinled = 4;
int pinreject = 6;
int pinmotorcw = A0;
int pinmotorccw = 13;

void setup() {
  Serial.begin(9600);
  mySwitch.enableReceive(0);                                    // Receiver on interrupt 0 => that is pin #2
  pinMode(pinbuzzer, OUTPUT);   digitalWrite(pinbuzzer, LOW);   //buzzer -
  pinMode(pinled, OUTPUT);      digitalWrite(pinled, LOW);      //indikator live/baca rfid/diterima
  pinMode(pinreject, OUTPUT);   digitalWrite(pinreject, LOW);   //ditolak
  pinMode(pinmotorcw, OUTPUT);  digitalWrite(pinmotorcw, LOW);  //MOTOR DC  - CW
  pinMode(pinmotorccw, OUTPUT); digitalWrite(pinmotorccw, LOW); //MOTOR DC  - CCW
}

void loop() {
  unsigned long currentMillis = millis();
  while (Serial.available() > 0) {              // baca RFID
    c = Serial.read();
    newtag += c;
    delay(5);
  }


  if (mySwitch.available()) {                   // baca remote rf 4333 MHZ
    rfremotenewtag = mySwitch.getReceivedValue();
    mySwitch.resetAvailable();
    Serial.println("Remote : " + String(rfremotenewtag));
  }
  

  newtag = newtag.substring(1, 13); //22005F5CFBDA
  delay(10);
  //Serial.println("Hasil Swap RFID: " + newtag);

if(currentMillis - previousMillis <= 500) {
  if ((newtag.length() == 12) or (rfremotenewtag > 0)) {
    digitalWrite(pinbuzzer, HIGH);  digitalWrite(pinled, HIGH);   delay(100);
    digitalWrite(pinbuzzer, LOW);   digitalWrite(pinled, LOW);    delay(100);
    digitalWrite(pinbuzzer, HIGH);  digitalWrite(pinled, HIGH);   delay(100);
    digitalWrite(pinbuzzer, LOW);   digitalWrite(pinled, LOW);    delay(100);

    if ((newtag != tag1) and (newtag != tag2) and (rfremotenewtag != rfremote)) {
      digitalWrite(pinbuzzer, HIGH);
      digitalWrite(pinreject, HIGH); delay(100);
      digitalWrite(pinbuzzer, LOW);
      digitalWrite(pinreject, LOW); delay(100);
      digitalWrite(pinbuzzer, HIGH);
      digitalWrite(pinreject, HIGH); delay(100);
      digitalWrite(pinbuzzer, LOW);
      digitalWrite(pinreject, LOW); delay(100);
      digitalWrite(pinbuzzer, HIGH);
      digitalWrite(pinreject, HIGH); delay(100);
      digitalWrite(pinbuzzer, LOW);
      digitalWrite(pinreject, LOW); delay(100);
      digitalWrite(pinbuzzer, HIGH);
      digitalWrite(pinreject, HIGH); delay(100);
      digitalWrite(pinbuzzer, LOW);
      digitalWrite(pinreject, LOW); delay(100);
      Serial.println(String(rfremotenewtag) + " Ditolak");
      Serial.println(newtag + " DITOLAK");
      newtag = "";
      rfremotenewtag = 0;
    }
    else if ((newtag == tag1) or (newtag == tag2) or (rfremotenewtag == rfremote)) {
      Serial.println(String(rfremotenewtag) + " Diterima");
      Serial.println(newtag + " DITERIMA");
      Serial.println("membuka pintu");

      digitalWrite(pinmotorcw, HIGH);  digitalWrite(pinmotorccw, LOW);      //putar CW / buka
      delay (50);
      digitalWrite(pinbuzzer, HIGH);
      delay(600);
      digitalWrite(pinmotorccw, LOW); digitalWrite(pinmotorcw, LOW); delay (50);
      digitalWrite(pinbuzzer, LOW);
      digitalWrite(pinled, HIGH);
      delay (8000);
      digitalWrite(pinled, LOW);
      delay (50);
      // after 5 second kunci otomatis//
      Serial.println("mengunci pintu");
      digitalWrite(pinmotorcw, LOW);  digitalWrite(pinmotorccw, HIGH);      //putar ccw / tutup
      delay (50);
      digitalWrite(pinbuzzer, HIGH);
      delay(600);
      digitalWrite(pinmotorccw, LOW); digitalWrite(pinmotorcw, LOW);
      delay (50);
      digitalWrite(pinbuzzer, LOW);
      newtag = "";
      rfremotenewtag = 0;
//      scanenable =0 ;
    }

  }
}
  newtag = "";
  rfremotenewtag = 0;
  delay(200);
//  Serial.println(String(currentMillis - previousMillis));
  previousMillis = currentMillis;
  
}


