#include "AD7768.hpp"
#include <SPI.h>

#define SCALE (1L << 23)
#define VREF 4.096

AD7768 adc(SPI);

void setup() {
  Serial.begin(9600);
  delay(1000);
  SPI.begin();

  if (adc.init()) {
    Serial.println("ADC connected");
  }
  else {
    Serial.println("ADC failed to connect");
  }
}

void loop() {
  int32_t adc_val = adc.read();
  Serial.print("ADC Value: ");
  Serial.println(adc_val);

  float voltage = ((float)adc_val/SCALE) * VREF;
  Serial.print("Voltage: ");
  Serial.println(voltage);
  Serial.print('\n');

  delay(1000);
}
