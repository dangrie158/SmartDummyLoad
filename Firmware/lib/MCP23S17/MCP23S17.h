#include <Arduino.h>
#include <SPI.h>
#include <inttypes.h>
#include <stdbool.h>

#ifndef MCP23S17_H_
#define MCP23S17_H_

#define PORT_A 0
#define PORT_B 1

class MCP23S17 {
private:
  uint8_t mCSPin;
  uint8_t mAddress;

  // caching variables to store the state the device is in
  uint16_t mModeCache;
  uint16_t mPullupCache;
  uint16_t mOutputCache;

  void byteWrite(uint8_t reg, uint8_t value);
  void wordWrite(uint8_t reg, uint16_t word);
  uint16_t wordRead(uint8_t reg);

public:
  MCP23S17(uint8_t csPin, uint8_t address = 0x00);

  void digitalWrite(uint8_t pin, bool status);
  bool digitalRead(uint8_t pin);
  void portWrite(uint8_t port, uint8_t value);
  uint8_t portRead(uint8_t port);

  void pinMode(uint8_t pin, uint8_t mode);
  void portMode(uint8_t port, uint8_t mode);
};

#endif // MCP23S17_H_
