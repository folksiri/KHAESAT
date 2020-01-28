#include <TinyGPS++.h>
#include <SoftwareSerial.h>
static const int RXPin = 4, TXPin = 3;
static const uint32_t GPSBaud = 9600;
TinyGPSPlus gps;
SoftwareSerial ss(RXPin, TXPin);
void setup(){
  Serial.begin(9600);
  ss.begin(GPSBaud);
}
void loop(){
  /*while (ss.available() > 0){
    gps.encode(ss.read());
    if (gps.location.isUpdated()){*/
      //Serial.print("Latitude= "); 
      Serial.print(gps.location.lat(), 6);      
      //Serial.print(" Longitude= "); 
      Serial.println(gps.location.lng(), 6); 
     //Serial.print("Raw date DDMMYY = ");
      Serial.println(gps.date.value());
     // Serial.print("Raw time in HHMMSSCC = "); 
      Serial.println(gps.time.value()); 
     // Serial.print("Speed in m/s = ");
      Serial.println(gps.speed.mps()); 
      //Serial.print("Raw altitude in centimeters = "); 
      Serial.println(gps.altitude.value()); 
     // Serial.print("Altitude in meters = "); 
      Serial.println(gps.altitude.meters());  
   // }
  //}
}
