#include "Wire.h" 
const int MPU_ADDR = 0x68; 
int16_t accelerometer_x, accelerometer_y, accelerometer_z; 
int16_t gyro_x, gyro_y, gyro_z; 
int16_t temperature; 
char tmp_str[7]; 
char* convert_int16_to_str(int16_t i) { 
  sprintf(tmp_str, "%6d", i);
  return tmp_str;
}
void setup() {
  Serial.begin(9600);
  Wire.begin();
  Wire.beginTransmission(MPU_ADDR); 
  Wire.write(0x6B); 
  Wire.write(0); 
  Wire.endTransmission(true);
}
void loop() {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B); 
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR, 7*2, true); 
  accelerometer_x = Wire.read()<<8 | Wire.read(); 
  accelerometer_y = Wire.read()<<8 | Wire.read(); 
  accelerometer_z = Wire.read()<<8 | Wire.read();
  temperature = Wire.read()<<8 | Wire.read(); 
  gyro_x = Wire.read()<<8 | Wire.read(); 
  gyro_y = Wire.read()<<8 | Wire.read(); 
  gyro_z = Wire.read()<<8 | Wire.read(); 
  Serial.print(""); Serial.print((accelerometer_x));
  Serial.print(","); Serial.print((accelerometer_y));
  Serial.print(","); Serial.print((accelerometer_z));
  Serial.print(","); Serial.print(temperature/340.00+36.53);
  Serial.print(","); Serial.print((gyro_x)/1000);
  Serial.print(","); Serial.print((gyro_y)/1000);
  Serial.print(","); Serial.print((gyro_z)/1000);
  Serial.println();
  delay(1000);
}
