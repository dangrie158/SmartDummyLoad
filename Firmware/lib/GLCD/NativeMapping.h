#ifndef NATIVEMAPPING_H_
#define NATIVEMAPPING_H_

#include "IOMapping.h"
#include <inttypes.h>

class NativeMapping : public IOMapping {
private:
public:
  NativeMapping();

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

#endif // NATIVEMAPPING_H_
