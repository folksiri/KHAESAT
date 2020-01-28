#include <SPI.h>
#include <LoRa.h>

// LoRa connect config
#define NSS_PIN    10
#define NRESET_PIN 9
#define DIO0_PIN   2

String text = "Hello LoRa";

void setup() {
  Serial.begin(115200);
  while (!Serial);
  //LoRa.setTxPower(6);

  Serial.println("LoRa Node - Sender");

  LoRa.setPins(NSS_PIN, NRESET_PIN, DIO0_PIN);
  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while(1);
  }
}

void loop() {
  LoRa.beginPacket();
  LoRa.print(text);
  LoRa.endPacket();

  Serial.print("Send '");
  Serial.print(text);
  Serial.println("'");
  delay(1000);
}
