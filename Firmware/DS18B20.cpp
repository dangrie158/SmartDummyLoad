#include "DS18B20.h"

DS18B20::DS18B20(uint8_t pinNumber) : mPin(pinNumber) {
  mIO = new OneWire(mPin);
  mSensor = new DallasTemperature(mIO);

  mSensor->begin();
}

DS18B20::~DS18B20() {
  delete mSensor;
  delete mIO;
}

float DS18B20::getTemperature() {
  mSensor->requestTemperatures();
  return mSensor->getTempCByIndex(FIRST_DEVICE);
}
