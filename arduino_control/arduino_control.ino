// ****** Pin analog roundsSignal Ausgabe 0 BIS 5V*******
/* Uno pins 5 and 6: 980 Hz */
byte PIN_roundsSignal = 5;

// 0,1 - 5V Analog
// green Kabel
float Volt_rounds = 0;
// Motor action!
byte PIN_go = 12;
bool RPMswitch = true;

// ****** Pin analog turn_direction Ausgabe 0 ODER 5V *******
// blaues Kabel
// Z / F
byte PIN_turn_direction = 4;

// Linksdrehung TRUE (5V), Rechtsdrehung FALSE (0V)
bool turn_direction = false;


// rounds_measure
byte PIN_rounds_measure = 8;
int coil_measure_new = 0;
int coil_measure_old = 0;
int coil_passed = 0;
unsigned long last_turn = 0;
unsigned long last_turn_time = 0;
int RPM;

// PWM wird von Volt rounds gemappt
long PWM_rounds = 0;

// Tests with text output over serial (with sprint function)
char AktuelleWerte[50];

// for time functions
unsigned long old_time;
unsigned long time_point;
unsigned long time_point_2;


void setup() {
  Serial.begin(9600);
  pinMode(PIN_turn_direction, OUTPUT);
  pinMode(PIN_rounds_measure, INPUT);

  // ON off Motor
  pinMode(PIN_go, INPUT);
}

void loop() {
  if (millis() - 1000 >= old_time) {
    old_time = millis();
    // 0 bis 5 auf 0 bis 255
    // PWM_rounds = map(Volt_rounds, 0, 5, 0, 255);
    // Mit Dreisatz
    PWM_rounds = (Volt_rounds / 5) * 255;

    // Werte ausgeben
    //sprintf(AktuelleWerte, "Volt_rounds ist %d, PWM_rounds ist %d.", 2, PWM_rounds);
    //Serial.println(AktuelleWerte);
    /*Serial.print("rounds in Volt: ");
    Serial.print(Volt_rounds);
    Serial.print("  PWM Scale: ");
    Serial.println(PWM_rounds);*/
  }
  setRPM();
  handleRPM();

  // Signal für rounds ausgeben
  analogWrite(PIN_roundsSignal, PWM_rounds);

  // Signal für turn_direction ausgeben
  digitalWrite(PIN_turn_direction, turn_direction);
}

void setRPM() {
  // Nur anschalten wenn letzte Umdrehung 5s her ist
  //Serial.println(digitalRead(PIN_go));
  //Serial.println(digitalRead(Volt_rounds));

  if (digitalRead(PIN_go) == 1 || 1) {
    Volt_rounds = 0.6;
  } else {
    Volt_rounds = 0;
  }
  if (millis() - time_point >= 5000) {
    //Volt_rounds = 1;
    // if (RPMswitch) {
    //   RPMswitch = false;
    //   Volt_rounds = 5;
    // } else {
    //   RPMswitch = true;
    //   Volt_rounds = 5;
    // }
  }
}

void handleRPM() {
  measureRPM();

  /*
  if (coil_passed == 90) {
    last_turn_time = millis() - last_turn;
    last_turn = millis();
    c = 0;
    //Serial.println("coil_passed/81");
    Volt_rounds = 0;

    // 5s warten bis Volt wieder auf 5V geschalten werden
    time_point = millis();
  }
  */
}

void measureRPM() {
  // Messung nur innerhalb eines Zeitfensters ausführen
  if (millis() < (time_point_2 + 1000)) {

    coil_measure_old = digitalRead(PIN_rounds_measure);
    // Serial.println(coil_measure_old);

    // positive Flanke -> drei Hall Sensoren über Spule gefahren (1/27 Umdrehung)
    if ((coil_measure_old == 0 && coil_measure_new == 1) || (coil_measure_old == 1 && coil_measure_new == 0)) {
      //Serial.println("negative Flanke");
      coil_passed++;
    }

    coil_measure_new = digitalRead(PIN_rounds_measure);
    //Serial.println(coil_measure_new);
  } else {
    time_point_2 = millis();
    displayRPM();
    coil_passed = 0;
  }
}
void displayRPM() {

  //RPM = (60 / (last_turn_time * pow(10, -3)));
  Serial.print("coil_passed: ");
  Serial.print(coil_passed);
  Serial.print("   coil_passed / 27: ");
  Serial.println((float)(coil_passed / 60.0));
  RPM = (coil_passed)*600;
  //Serial.print((coil_passed / 90));
  //Serial.println(" RPM");
}