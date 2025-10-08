// Nathan Achinger
// 10/07/25
// AD7768 ADC Arduino Driver 
// AD7768.cpp

#include "AD7768.hpp"

AD7768::AD7768(SPIClass &spiPort) {
    spi = &spiPort;
}

bool AD7768::init() {
    pinMode(CS, OUTPUT);
    pinMode(DRDY, INPUT);
    pinMode(RST, OUTPUT);

    digitalWrite(CS, 1);
    digitalWrite(RST, 1);
    delay(20);

    digitalWrite(RST, 0);
    delay(2);
    digitalWrite(RST, 1);
    delay(200);

    spi->beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE3));

    uint8_t vendor_id_l = readRegister(VENDOR_ID_REG_L);
    uint8_t vendor_id_h = readRegister(VENDOR_ID_REG_L + 1);

    uint16_t vendor_id = (uint16_t)(vendor_id_h << 8) | (uint16_t)(vendor_id_l);
    if (vendor_id != 0x0456) {
        return 0;
    }
    return 1;
}

int32_t AD7768::read() {
    while(!dataReady());
  
    uint8_t instr = SPI_WRITE_BM | (ADC_CONV_DATA_REG & 0x3F);
    
    select();
    spi->transfer(instr);
    uint8_t byte2 = spi->transfer(0);
    uint8_t byte1 = spi->transfer(0);
    uint8_t byte0 = spi->transfer(0);
    deselect();

    // assemble 24-bit adc value
    int32_t adc_val = (int32_t)((uint32_t)byte2 << 16 | (uint32_t)byte1 << 8 | byte0);
    return (adc_val << 8) >> 8; // sign extension to 32 bit
}

uint8_t AD7768::readRegister(uint8_t reg) {
    uint8_t instr = SPI_READ_BM | (reg & 0x3F);
    select();
    spi->transfer(instr);
    uint8_t value = spi->transfer(0);
    deselect();
    return value;
}

void AD7768::writeRegister(uint8_t reg, uint8_t data) {
    uint8_t instr = SPI_WRITE_BM | (reg & 0x3F);
    select();
    spi->transfer(instr);
    spi->transfer(data);
    deselect();
}

bool AD7768::dataReady() {
    return (!(digitalRead(DRDY)));
}

void AD7768::select() {
    digitalWrite(CS, 0); 
}

void AD7768::deselect() {
    digitalWrite(CS, 1);
}