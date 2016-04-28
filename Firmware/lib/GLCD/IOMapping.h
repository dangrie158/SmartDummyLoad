#ifndef IOMAPPING_H_
#define IOMAPPING_H_

#include <inttypes.h>

class IOMapping {
private:
public:
  // set all IO as Input/Output
  virtual void initializeIO() = 0;

  // set all data pins as output
  virtual void prepareDataOutput() = 0;
  virtual void prepareDataInput() = 0;
  virtual void writeData(uint8_t data) = 0;
  virtual uint8_t readData() = 0;

  virtual void setRwPin() = 0;
  virtual void clearRwPin() = 0;
  virtual void setDiPin() = 0;
  virtual void clearDiPin() = 0;
  virtual void setEnPin() = 0;
  virtual void clearEnPin() = 0;
  virtual void setCsPin(uint8_t controller) = 0;
  virtual void clearCsPin(uint8_t controller) = 0;
};

#endif // IOMAPPING_H_
