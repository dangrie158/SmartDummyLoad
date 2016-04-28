#include "MCP23S17GLCDMapping.h"
#include "GLCD.h"

MCP23S17GLCDMapping::MCP23S17GLCDMapping(MCP23S17 *ioDevice)
    : mIoDevice(ioDevice) {}

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

void MCP23S17GLCDMapping::setRwPin() { mIoDevice->digitalWrite(RW_PIN, HIGH); }
void MCP23S17GLCDMapping::clearRwPin() { mIoDevice->digitalWrite(RW_PIN, LOW); }
void MCP23S17GLCDMapping::setDiPin() { mIoDevice->digitalWrite(DI_PIN, HIGH); }
void MCP23S17GLCDMapping::clearDiPin() { mIoDevice->digitalWrite(DI_PIN, LOW); }
void MCP23S17GLCDMapping::setEnPin() { mIoDevice->digitalWrite(EN_PIN, HIGH); }
void MCP23S17GLCDMapping::clearEnPin() { mIoDevice->digitalWrite(EN_PIN, LOW); }
void MCP23S17GLCDMapping::setCsPin(uint8_t controller) {
  switch (controller) {
  case CHIP0:
    mIoDevice->digitalWrite(CS1_PIN, HIGH);
    break;
  case CHIP1:
    mIoDevice->digitalWrite(CS2_PIN, HIGH);
    break;
  }
}
void MCP23S17GLCDMapping::clearCsPin(uint8_t controller) {
  switch (controller) {
  case CHIP0:
    mIoDevice->digitalWrite(CS1_PIN, LOW);
    break;
  case CHIP1:
    mIoDevice->digitalWrite(CS2_PIN, LOW);
    break;
  }
}
