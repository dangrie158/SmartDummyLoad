#include "MCP23S17GLCDMapping.h"
#include "GLCD.h"

MCP23S17GLCDMapping::MCP23S17GLCDMapping(MCP23S17 *ioDevice)
    : mIoDevice(ioDevice), mControlPortStatus(0x00) {}

void MCP23S17GLCDMapping::initializeIO() {
  mIoDevice->portMode(CTRL_PORT, OUTPUT);
  mIoDevice->digitalWrite(CS1_PIN, HIGH);
  mIoDevice->digitalWrite(CS2_PIN, HIGH);
}

// set all data pins as output
void MCP23S17GLCDMapping::prepareDataOutput() {
  mIoDevice->portMode(DATA_PORT, OUTPUT);
}
void MCP23S17GLCDMapping::prepareDataInput() {
  mIoDevice->portMode(DATA_PORT, INPUT);
}
void MCP23S17GLCDMapping::writeData(uint8_t data) {
  mIoDevice->portWrite(DATA_PORT, data);
}
uint8_t MCP23S17GLCDMapping::readData() {
  return mIoDevice->portRead(DATA_PORT);
}

void MCP23S17GLCDMapping::setRwPin() { mControlPortStatus |= (1 << RW_PIN); }
void MCP23S17GLCDMapping::clearRwPin() { mControlPortStatus &= ~(1 << RW_PIN); }
void MCP23S17GLCDMapping::setDiPin() { mControlPortStatus |= (1 << DI_PIN); }
void MCP23S17GLCDMapping::clearDiPin() { mControlPortStatus &= ~(1 << DI_PIN); }
void MCP23S17GLCDMapping::setEnPin() {
  mControlPortStatus |= (1 << EN_PIN);
  // this call happens after all other pins
  // are set, do the real IO now

  mIoDevice->portWrite(PORT_A, mControlPortStatus);
}
void MCP23S17GLCDMapping::clearEnPin() { mControlPortStatus &= ~(1 << EN_PIN); }
void MCP23S17GLCDMapping::setCsPin(uint8_t controller) {
  switch (controller) {
  case CHIP0:
    mControlPortStatus |= (1 << CS1_PIN);
    break;
  case CHIP1:
    mControlPortStatus |= (1 << CS2_PIN);
    break;
  }
}
void MCP23S17GLCDMapping::clearCsPin(uint8_t controller) {
  switch (controller) {
  case CHIP0:
    mControlPortStatus &= ~(1 << CS1_PIN);
    break;
  case CHIP1:
    mControlPortStatus &= ~(1 << CS2_PIN);
    break;
  }

  // this call happens after after the control port is set up
  // do the IO now
  mIoDevice->portWrite(PORT_A, mControlPortStatus);
}
