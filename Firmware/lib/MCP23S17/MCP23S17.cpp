#include "MCP23S17.h"

#define OPCODEW (0b01000000)     // Opcode for write (0)
#define OPCODER (0b01000001)     // Opcode for read (1)
#define ADDR_ENABLE (0b00001000) // set the hardware adressing bit

#define IODIRA (0x00) // MCP23x17 I/O Direction Register
#define IODIRB (0x01) // 1 = Input (default), 0 = Output

#define IOCON (0x0A) // MCP23S17 Configuration Register

#define GPPUA (0x0C) // MCP23S17 Weak Pull-Up Resistor Register
#define GPPUB (0x0D) // INPUT ONLY: 0 = no 100k pull-up, 1 = pullups enabled

#define GPIOA (0x12) // MCP23S17 GPIO Port Register
#define GPIOB (0x13) // Value on the Port

#define OLATA (0x14) // MCP23S17 Output Latch Register
#define OLATB (0x15) // 1 = Latch High, 0 = Latch Low (default)

MCP23S17::MCP23S17(uint8_t csPin, uint8_t address)
    : mCSPin(csPin), mAddress(address), mModeCache(0x0000),
      mPullupCache(0x0000), mOutputCache(0x0000) {
  SPI.begin();
  pinMode(mCSPin, OUTPUT);
  digitalWrite(mCSPin, HIGH);

  byteWrite(IOCON, ADDR_ENABLE);
}

void MCP23S17::byteWrite(uint8_t reg, uint8_t value) {

  ::digitalWrite(mCSPin, LOW);
  SPI.transfer(OPCODEW | (mAddress << 1));
  // Send the register we want to write
  SPI.transfer(reg);
  // Send the byte
  SPI.transfer(value);
  // Take slave-select high
  ::digitalWrite(mCSPin, HIGH);
}

// GENERIC WORD WRITE - will write a word to a register pair, LSB to first
// register, MSB to next higher value register

void MCP23S17::wordWrite(uint8_t reg, uint16_t word) {
  ::digitalWrite(mCSPin, LOW);

  // Send the MCP23S17 opcode, chip address, and write bit
  SPI.transfer(OPCODEW | (mAddress << 1));
  // Send the register we want to write
  SPI.transfer(reg);
  // Send the low byte
  SPI.transfer((uint8_t)(word));
  // Send the high byte
  SPI.transfer((uint8_t)(word >> 8));
  // Take slave-select high
  ::digitalWrite(mCSPin, HIGH);
}

uint16_t MCP23S17::wordRead(uint8_t reg) {
  uint16_t value = 0;
  // Take slave-select low
  ::digitalWrite(mCSPin, LOW);
  // Send the MCP23S17 opcode, chip address, and read bit
  SPI.transfer(OPCODER | (mAddress << 1));
  // Send the register we want to read
  SPI.transfer(reg);
  // send empty bytes to clock in the data from the chip
  value = SPI.transfer(0x00);
  value |= (SPI.transfer(0x00) << 8);
  // Take slave-select high
  ::digitalWrite(mCSPin, HIGH);
  // Return the constructed word, the format is 0x(portB)(portA)
  return value;
}

void MCP23S17::digitalWrite(uint8_t pin, bool status) {
  if (status == HIGH) {
    mOutputCache |= (1 << pin);
  } else if (status == LOW) {
    mOutputCache &= ~(1 << pin);
  }

  wordWrite(GPIOA, mOutputCache);
}
bool MCP23S17::digitalRead(uint8_t pin) {
  uint8_t port = (pin < 8) ? GPIOA : GPIOB;
  uint8_t portStatus = this->portRead(port);

  uint8_t offset = (port == PORT_A) ? 0 : 8;

  return (portStatus >> (pin - offset)) & 1;
}
void MCP23S17::portWrite(uint8_t port, uint8_t value) {
  if (port != PORT_A && port != PORT_B) {
    return;
  }

  ((uint8_t *)(&mOutputCache))[port] = value;

  wordWrite(GPIOA, mOutputCache);
}
uint8_t MCP23S17::portRead(uint8_t port) {
  if (port != PORT_A && port != PORT_B) {
    return 0x00;
  }

  uint8_t reg = (port == PORT_A) ? GPIOA : GPIOB;

  ::digitalWrite(mCSPin, LOW);
  SPI.transfer(OPCODER | (mAddress << 1));
  SPI.transfer(reg);

  uint8_t value = SPI.transfer(0x00);

  ::digitalWrite(mCSPin, HIGH);

  return value;
}

void MCP23S17::pinMode(uint8_t pin, uint8_t mode) {
  if (mode == INPUT) {
    mModeCache |= (1 << pin);
    mPullupCache &= ~(1 << pin);
  } else if (mode == OUTPUT) {
    mModeCache &= ~(1 << pin);
    mPullupCache &= ~(1 << pin);
  } else if (mode == INPUT_PULLUP) {
    mModeCache &= ~(1 << pin);
    mPullupCache |= (1 << pin);
  }
  wordWrite(IODIRA, mModeCache);
  wordWrite(GPPUA, mPullupCache);
}
void MCP23S17::portMode(uint8_t port, uint8_t mode) {
  if (port != PORT_A && port != PORT_B) {
    return;
  }

  uint8_t portModeValue = 0x00;
  uint8_t portPullupValue = 0x00;

  if (mode == INPUT) {
    portModeValue = 0xFF;
  } else if (mode == INPUT_PULLUP) {
    portPullupValue = 0xFF;
    portModeValue = 0xFF;
  }

  ((uint8_t *)(&mModeCache))[port] = portModeValue;
  ((uint8_t *)(&mPullupCache))[port] = portPullupValue;

  wordWrite(IODIRA, mModeCache);
  wordWrite(GPPUA, mPullupCache);
}
