// *******************************************************************
//  Arduino Uno code
//  for   https://github.com/mide1337/arduino-bldc-motor-control
//
//  Copyright (C) 2023 Sebastian Stegmaier <sebastian@stegm.de>
//
// *******************************************************************


/*
 ideas from:
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp-now-esp32-arduino-ide/

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/
// Complete Instructions to Get and Change ESP MAC Address: https://RandomNerdTutorials.com/get-change-esp32-esp8266-mac-address-arduino/

#ifdef ESP32
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.print("ESP Board MAC Address:  ");
  Serial.println(WiFi.macAddress());
}

void loop() {
  delay(20000);
  Serial.print("ESP Board MAC Address:  ");
  Serial.println(WiFi.macAddress());
}
