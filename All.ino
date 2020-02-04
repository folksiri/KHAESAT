#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <BME280I2C.h>
#include <Wire.h>
#include <SPI.h>
#include <LoRa.h>
#define NSS_PIN    10
#define NRESET_PIN 4
#define DIO0_PIN   5

//gps
static const int RXPin = 3, TXPin = 4;
static const uint32_t GPSBaud = 9600;
TinyGPSPlus gps;
SoftwareSerial ss(RXPin, TXPin);
//gy-521
const int MPU_ADDR = 0x68; 
int16_t ax, ay, az; 
int16_t gx, gy, gz; 
int16_t tempgy; 
char tmp_str[7]; 
char* convert_int16_to_str(int16_t i) { 
  sprintf(tmp_str, "%6d", i);
  return tmp_str;
}
//bme280
BME280I2C::Settings settings(
   BME280::OSR_X1,
   BME280::OSR_X1,
   BME280::OSR_X1,
   BME280::Mode_Forced,
   BME280::StandbyTime_1000ms,
   BME280::Filter_Off,
   BME280::SpiEnable_False,
   BME280I2C::I2CAddr_0x76 // I2C address. I2C specific.
);
BME280I2C bme(settings);
float temp(NAN), hum(NAN), pres(NAN);
void setup()
  {
    Serial.begin(9600);
    //gps
    ss.begin(GPSBaud);
    while(!Serial) {}
    Wire.begin();
    //bme280
    while(!bme.begin())
    {
    Serial.println("Could not find BME280I2C sensor!");
    delay(1000);
    }
    switch(bme.chipModel())
    {
      case BME280::ChipModel_BME280:
       Serial.println("Found BME280 sensor! Success.");
       break;
      case BME280::ChipModel_BMP280:
       Serial.println("Found BMP280 sensor! No Humidity available.");
       break;
      default:
       Serial.println("Found UNKNOWN sensor! Error!");
    }
      settings.tempOSR = BME280::OSR_X4;
      bme.setSettings(settings);
    //gy-521
    Wire.beginTransmission(MPU_ADDR); 
    Wire.write(0x6B); 
    Wire.write(0); 
    Wire.endTransmission(true);
    //Lora
    LoRa.setPins(NSS_PIN, NRESET_PIN, DIO0_PIN);
  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
    LoRa.setTxPower(15);
    LoRa.setSpreadingFactor(7);
    LoRa.setSignalBandwidth(125E3);
    LoRa.setCodingRate4(4);
  }

void loop()
  {
    Serial.print("@459"); 
    Serial.print(","); 
    printBME280Data(&Serial);
    gySensor();
    gpsSensor();
    LoRa.beginPacket();
    LoRa.print("@459");
    LoRa.print(",");
    LoRa.print(ax);
    LoRa.print(",");
    LoRa.print(ay);
    LoRa.print(",");
    LoRa.print(az);
    LoRa.print(",");
    LoRa.print(tempgy/340.00+36.53);
    LoRa.print(",");
    LoRa.print(gx);
    LoRa.print(",");
    LoRa.print(gy);
    LoRa.print(",");
    LoRa.print(gz);
    LoRa.print(",");
    LoRa.print(gps.location.lat(),6);
    LoRa.print(",");
    LoRa.print(gps.location.lng(),6);
    LoRa.print(",");
    LoRa.print(gps.date.value());
    LoRa.print(",");
    LoRa.print(gps.time.value());
    LoRa.print(",");
    LoRa.print(gps.speed.mps());
    LoRa.print(",");
    LoRa.print(gps.altitude.meters());
    LoRa.print(",");
    LoRa.print(temp);
    LoRa.print(",");
    LoRa.print(pres);
    LoRa.print(",");
    LoRa.print((101325-pres)/12.1202);
    LoRa.print(",");  
    LoRa.endPacket(); 
    /*Serial.print(","); Serial.print((ax));
    Serial.print(","); Serial.print((ay));
    Serial.print(","); Serial.print((az));
    Serial.print(","); Serial.print(tempgy/340.00+36.53);
    Serial.print(","); Serial.print((gx)/1000);
    Serial.print(","); Serial.print((gy)/1000);
    Serial.print(","); Serial.print((gz)/1000);
    Serial.print(","); Serial.print(gps.location.lat(), 6);      
    Serial.print(","); Serial.println(gps.location.lng(), 6); 
    Serial.print(","); Serial.println(gps.date.value());
    Serial.print(","); Serial.println(gps.time.value()); 
    Serial.print(","); Serial.println(gps.speed.mps());
    Serial.print(","); Serial.print(temp);
    Serial.print(","); Serial.print(pres);
    Serial.print(","); Serial.print((101325-pres)/12.1202);*/
    Serial.print("\n");
    delay(500);
  }
void gySensor() {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B); 
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR, 7*2, true); 
  ax= Wire.read()<<8 | Wire.read(); 
  ay= Wire.read()<<8 | Wire.read(); 
  az= Wire.read()<<8 | Wire.read();
  tempgy = Wire.read()<<8 | Wire.read(); 
  gx= Wire.read()<<8 | Wire.read(); 
  gy= Wire.read()<<8 | Wire.read(); 
  gz= Wire.read()<<8 | Wire.read(); 
  Serial.print(""); Serial.print((ax));
  Serial.print(","); Serial.print((ay));
  Serial.print(","); Serial.print((az));
  Serial.print(","); Serial.print(tempgy/340.00+36.53);
  Serial.print(","); Serial.print((gx)/1000);
  Serial.print(","); Serial.print((gy)/1000);
  Serial.print(","); Serial.print((gz)/1000);
  Serial.print(",");
  //delay(1000);
} 

void gpsSensor()
      {
        while (ss.available() > 0){
    gps.encode(ss.read());
    if (gps.location.isUpdated()){
      Serial.print(gps.location.lat(), 6);
      Serial.print(",");      
      Serial.println(gps.location.lng(), 6);
      Serial.print(","); 
      Serial.println(gps.date.value());
      Serial.print(",");
      Serial.println(gps.time.value());
      Serial.print(","); 
      Serial.println(gps.speed.mps()); 
      }
        }}

void printBME280Data
(
   Stream* client
)
{
   
   BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
   BME280::PresUnit presUnit(BME280::PresUnit_Pa);
   bme.read(pres, temp, hum, tempUnit, presUnit);
   client->print(temp);
   client->print(",");
   client->print(pres);
   client->print(",");
   client->print((101325-pres)/12.1202);
   client->print(",");
}
