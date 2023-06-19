/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp-now-esp32-arduino-ide/

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/
#include <esp_now.h>
#include <WiFi.h>

// Address of CONTROL
// REPLACE WITH THE MAC Address of your receiver
uint8_t broadcastAddress[] = { 0xC8, 0xF0, 0x9E, 0x4E, 0x49, 0xE0 };

// Define variables to store BME280 readings to be sent
bool act_motor;

// Define variables to store incoming readings
unsigned long passed_time;

void data2Send() {
  act_motor = true;
}

// Variable to store if sending data was successful
String success;

//Structure example to send data
//Must match the receiver structure
typedef struct struct_send_message {
  bool act_motor;
} struct_send_message;

//Structure example to receive data
//Must match the receiver structure
typedef struct struct_receive_message {
  unsigned long passed_time;
} struct_receive_message;

// Create a struct_message called BME280Readings to hold sensor readings
struct_send_message ownData;

// Create a struct_message to hold incoming sensor readings
struct_receive_message incData;

esp_now_peer_info_t peerInfo;

// Callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  if (status == 0) {
    success = "Delivery Success :)";
  } else {
    success = "Delivery Fail :(";
  }
}

// Callback when data is received
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
  memcpy(&incData, incomingData, sizeof(incData));
  Serial.print("Bytes received: ");
  Serial.println(len);
  passed_time = incData.passed_time;
}

void setup() {
  // Init Serial Monitor
  Serial.begin(115200);


  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);

  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  // Add peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
  // Register for a callback function that will be called when data is received
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  data2Send();

  // Set values to send
  ownData.act_motor = act_motor;

  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&ownData, sizeof(ownData));

  if (result == ESP_OK) {
    Serial.println("Sent with success");
  } else {
    Serial.println("Error sending the data");
  }
  updateDisplay();
  delay(2000);
}


void updateDisplay() {
  // Display Readings in Serial Monitor
  Serial.println("INCOMING READINGS");
  Serial.print("Zeit seit Start von esp_CONTROL: ");
  Serial.print(incData.passed_time);
  Serial.print(" ms");
}
