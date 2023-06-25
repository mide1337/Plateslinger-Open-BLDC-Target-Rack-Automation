// *******************************************************************
//  ESP32 code
//  for   https://github.com/mide1337/arduino-bldc-motor-control
//
//  Copyright (C) 2023 Sebastian Stegmaier <sebastian@stegm.de>
//
// *******************************************************************


byte PIN_DrehzahlSignal = 33;
// 0,1 - 5V Analog
float Volt_Drehzahl = 0;
// PWM wird von Volt Drehzahl gemappt
long PWM_Drehzahl = 0;

// ****** Pin analog Drehrichtung Ausgabe 0 ODER 5V *******
byte PIN_DrehRichtung = 4;

// Linksdrehung TRUE (5V), Rechtsdrehung FALSE (0V)
bool Drehrichtung = true;

// ***** PWM *****
// the number of the LED pin
const int ledPin = 26;  // 16 corresponds to GPIO16

// setting PWM properties
const int freq = 1000;
const int ledChannel = 0;
const int resolution = 8;

void setup() {
  Serial.begin(115200);
  pinMode(PIN_DrehRichtung, OUTPUT);

  // configure LED PWM functionalitites
  ledcSetup(ledChannel, freq, resolution);

  // attach the channel to the GPIO to be controlled
  ledcAttachPin(ledPin, ledChannel);
}

void loop() {
  // 0 bis 5 auf 0 bis 255
  // PWM_Drehzahl = map(Volt_Drehzahl, 0, 5, 0, 255);
  // Mit Dreisatz
  PWM_Drehzahl = (Volt_Drehzahl / 5) * 255;

  // changing the LED brightness with PWM
  ledcWrite(ledChannel, 240);

  // Werte ausgeben
  //sprintf(AktuelleWerte, "Volt_Drehzahl ist %d, PWM_Drehzahl ist %d.", Volt_Drehzahl, PWM_Drehzahl);
  //Serial.println(AktuelleWerte);
  Serial.print(Volt_Drehzahl);
  Serial.println(PWM_Drehzahl);
}
