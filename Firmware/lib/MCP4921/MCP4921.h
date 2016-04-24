#include <Arduino.h>
#include <SPI.h>
#include <inttypes.h>
#include <stdbool.h>

#ifndef MCP4921_H_
#define MCP4921_H_

// bit number of the buffer enable bit
#define BUF_BIT 14
// bit number of the inverted gain enable bit
#define NGA_BIT 13
// bit number of the inverted shutdown bit
#define NSHDN_BIT 12

class MCP4921 {
private:
  uint8_t mCSPin;
  bool mBufferEnabled;
  bool mGainEnabled;
  bool mShutdownEnabled;
  uint16_t mValue;

  void synchronizeDevice();

public:
  MCP4921(uint8_t csPin, bool bufferEnabled = true, bool gainEnabled = false,
          bool shutdownEnabled = false);

  void setConfiguration(bool enableBuffer, bool enableGain,
                        bool enableShutdown);
  void setValue(uint16_t newValue);
  void shutdown();
  void wakeup();
  void enableGain();
  void gainEnabled(bool status);
  void bufferEnabled(bool status);
};

#endif // MCP4921_H_
