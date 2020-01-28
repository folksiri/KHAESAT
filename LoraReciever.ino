#include <SPI.h>
#include <LoRa.h>

// LoRa connect config
#define NSS_PIN    10
#define NRESET_PIN 9
#define DIO0_PIN   2

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("LoRa Node - Receiver");
  
  LoRa.setPins(NSS_PIN, NRESET_PIN, DIO0_PIN);
  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while(1);
  }
  //LoRa.setTimeout(100);
  LoRa.setSpreadingFactor(7);
  LoRa.setSignalBandwidth(125E3);
  LoRa.setCodingRate4(4);
}

void loop() {
  if (LoRa.parsePacket() > 0) {
    String text = LoRa.readString();
    Serial.print("Receiver '");
    Serial.print(text);
    Serial.print("' RSSI is ");
    Serial.println(LoRa.packetRssi());
  }
  delay(1);
}
