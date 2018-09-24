#include <DallasTemperature.h>
#include <OneWire.h>

#include <inttypes.h>

#ifndef DS18B20_H_
#define DS18B20_H_

#define FIRST_DEVICE 0

class DS18B20 {
private:
  uint8_t mPin;
  OneWire *mIO;
  DallasTemperature *mSensor;
  uint8_t *mAddress;

public:
  DS18B20(uint8_t pinNumber);
  ~DS18B20();

  float getTemperature();
  void requestTemperature();
  bool isConversionReady();
};

#endif // DS18B20_H_
