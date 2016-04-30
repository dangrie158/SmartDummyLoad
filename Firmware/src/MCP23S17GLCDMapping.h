#ifndef MCPGLCDMAPPING_H_
#define MCPGLCDMAPPING_H_

#include "IOMapping.h"
#include "MCP23S17.h"
#include <inttypes.h>

#define DATA_PORT PORT_B
#define CTRL_PORT PORT_A
#define CS1_PIN 1
#define CS2_PIN 0
#define RW_PIN 3
#define DI_PIN 4
#define EN_PIN 2

class MCP23S17GLCDMapping : public IOMapping {
private:
  MCP23S17 *mIoDevice;

  uint8_t mControlPortStatus;

public:
  MCP23S17GLCDMapping(MCP23S17 *ioDevice);

  void initializeIO();

  // set all data pins as output
  void prepareDataOutput();
  void prepareDataInput();
  void writeData(uint8_t data);
  uint8_t readData();

  void setRwPin();
  void clearRwPin();
  void setDiPin();
  void clearDiPin();
  void setEnPin();
  void clearEnPin();
  void setCsPin(uint8_t controller);
  void clearCsPin(uint8_t controller);
};

#endif // MCPGLCDMAPPING_H_
