// Nathan Achinger
// 10/07/25
// AD7768 ADC Arduino Driver 
// AD7768.hpp

#ifndef AD7768_HPP
#define AD7768_HPP

#include <Arduino.h>
#include <SPI.h>

#define VENDOR_ID_REG_L 0x0C
#define ADC_CONV_DATA_REG 0x2C
#define SPI_READ_BM 0x40

class AD7768 {
public:
  AD7768(SPIClass &spiPort);
  bool init();
  int32_t read();
  uint8_t readRegister(uint8_t reg);
  void writeRegister(uint8_t reg, uint8_t data);
private:
  bool dataReady();
  void select();
  void deselect();

  SPIClass* spi;
  static constexpr uint8_t CS = 10;
  static constexpr uint8_t DRDY = 2;
  static constexpr uint8_t RST = 7;
};

#endif