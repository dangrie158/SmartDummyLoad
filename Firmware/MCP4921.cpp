#include "MCP4921.h"
MCP4921::MCP4921(uint8_t csPin, bool bufferEnabled, bool gainEnabled,
                 bool shutdownEnabled)
    : mCSPin(csPin), mBufferEnabled(bufferEnabled), mGainEnabled(gainEnabled),
      mShutdownEnabled(shutdownEnabled), mValue(0) {
  SPI.begin();
  pinMode(mCSPin, OUTPUT);
  digitalWrite(mCSPin, HIGH);
}

void MCP4921::synchronizeDevice() {
  uint16_t configuration = 0x0000;

  configuration |= ((mBufferEnabled ? 0x01 : 0x00) << BUF_BIT);
  configuration |= ((mGainEnabled ? 0x00 : 0x01) << NGA_BIT);
  configuration |= ((mShutdownEnabled ? 0x00 : 0x01) << NSHDN_BIT);
  configuration |= (mValue & 0x0FFF);

  SPI.beginTransaction(SPISettings(20000000, MSBFIRST, SPI_MODE0));
  digitalWrite(mCSPin, LOW);
  SPI.transfer((configuration >> 8) & 0xFF);
  SPI.transfer(configuration & 0xFF);
  digitalWrite(mCSPin, HIGH);
  SPI.endTransaction();
}

void MCP4921::setConfiguration(bool enableBuffer, bool enableGain,
                               bool enableShutdown) {
  this->mBufferEnabled = enableBuffer;
  this->mGainEnabled = enableGain;
  this->mShutdownEnabled = enableShutdown;
  synchronizeDevice();
}

void MCP4921::setValue(uint16_t newValue) {
  this->mValue = newValue;
  synchronizeDevice();
}

void MCP4921::shutdown() {
  this->mShutdownEnabled = true;
  synchronizeDevice();
}

void MCP4921::wakeup() {
  this->mShutdownEnabled = false;
  synchronizeDevice();
}

void MCP4921::gainEnabled(bool status) {
  this->mGainEnabled = status;
  synchronizeDevice();
}

void MCP4921::bufferEnabled(bool status) {
  this->mBufferEnabled = status;
  synchronizeDevice();
}
