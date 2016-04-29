#ifndef NATIVEMAPPING_H_
#define NATIVEMAPPING_H_

#include "IOMapping.h"
#include <inttypes.h>

class NativeMapping : public IOMapping {
private:
  uint8_t mCs1Pin;
  uint8_t mCs2Pin;
  uint8_t mEnPin;
  uint8_t mRwPin;
  uint8_t mDiPin;
  uint8_t mD0Pin;
  uint8_t mD1Pin;
  uint8_t mD2Pin;
  uint8_t mD3Pin;
  uint8_t mD4Pin;
  uint8_t mD5Pin;
  uint8_t mD6Pin;
  uint8_t mD7Pin;

public:
  NativeMapping(uint8_t cs1Pin, uint8_t cs2Pin, uint8_t enPin, uint8_t rwPin,
                uint8_t diPin, uint8_t d0Pin, uint8_t d1Pin, uint8_t d2Pin,
                uint8_t d3Pin, uint8_t d4Pin, uint8_t d5Pin, uint8_t d6Pin,
                uint8_t d7Pin);

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
