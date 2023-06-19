// ****** Pin analog DrehzahlSignal Ausgabe 0 BIS 5V*******
/* Uno pins 5 and 6: 980 Hz */
byte PIN_DrehzahlSignal = 5;

// 0,1 - 5V Analog
// grünes Kabel
float Volt_Drehzahl = 2.0;
bool RPMswitch = true;
bool wait5s = true;

// ****** Pin analog Drehrichtung Ausgabe 0 ODER 5V *******
// blaues Kabel
// Z / F
byte PIN_DrehRichtung = 4;

// Linksdrehung TRUE (5V), Rechtsdrehung FALSE (0V)
bool Drehrichtung = true;


// Drehzahlmessung
byte PIN_DrehzahlMessung = A0;
int coil_measure_new = 0;
int coil_measure_old = 0;
byte coil_passed = 0;
unsigned long last_turn = 0;
unsigned long last_turn_time = 0;
int RPM;

// PWM wird von Volt Drehzahl gemappt
long PWM_Drehzahl = 0;

// Textausgabe
char AktuelleWerte[50];

// Zeitfunktionen
unsigned long old_time;
unsigned long time_point;

void setup() {
  Serial.begin(9600);
  pinMode(PIN_DrehRichtung, OUTPUT);
}

void loop() {
  if (millis() - 1000 >= old_time) {
    old_time = millis();
    // 0 bis 5 auf 0 bis 255
    // PWM_Drehzahl = map(Volt_Drehzahl, 0, 5, 0, 255);
    // Mit Dreisatz
    PWM_Drehzahl = (Volt_Drehzahl / 5) * 255;

    // Werte ausgeben
    //sprintf(AktuelleWerte, "Volt_Drehzahl ist %d, PWM_Drehzahl ist %d.", 2, PWM_Drehzahl);
    //Serial.println(AktuelleWerte);
    /*Serial.print("Drehzahl in Volt: ");
    Serial.print(Volt_Drehzahl);
    Serial.print("  PWM Scale: ");
    Serial.println(PWM_Drehzahl);*/
  }
  setRPM();
  handleRPM();

  // Signal für Drehzahl ausgeben
  analogWrite(PIN_DrehzahlSignal, PWM_Drehzahl);

  // Signal für Drehrichtung ausgeben
  digitalWrite(PIN_DrehRichtung, Drehrichtung);
}

void setRPM() {
  // Nur anschalten wenn letzte Umdrehung 5s her ist
  if (millis() - time_point >= 5000) {
    Volt_Drehzahl = 5;
    // if (RPMswitch) {
    //   RPMswitch = false;
    //   Volt_Drehzahl = 5;
    // } else {
    //   RPMswitch = true;
    //   Volt_Drehzahl = 5;
    // }
  }
}

void handleRPM() {
  measureRPM();

  if (coil_passed == 27) {
    last_turn_time = millis() - last_turn;
    last_turn = millis();
    coil_passed = 0;
    //Serial.println("coil_passed/81");
    Volt_Drehzahl = 0;

    // 5s warten bis Volt wieder auf 5V geschalten werden
    time_point = millis();
  }
}

void measureRPM() {
  coil_measure_old = analogRead(PIN_DrehzahlMessung);
  //Serial.println(coil_measure_old);

  // positive Flanke -> drei Hall Sensoren über Spule gefahren (1/27 Umdrehung)
  if (coil_measure_old < 1000 && coil_measure_new > 1000) {
    //Serial.println("negative Flanke");
    coil_passed++;
  }
  coil_measure_new = analogRead(PIN_DrehzahlMessung);
  //Serial.println(coil_measure_new);
}

void displayRPM() {
  if (millis() - 1000 >= old_time) {
    RPM = (60 / (last_turn_time * pow(10, -3)));
    Serial.print(RPM);
    Serial.println(" RPM");
  }
}