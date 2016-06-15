#include "DS18B20.h"

DS18B20::DS18B20(uint8_t pinNumber)
    : mPin(pinNumber), mAddress(new uint8_t[8]) {
  mIO = new OneWire(mPin);
  mSensor = new DallasTemperature(mIO);

  mSensor->begin();
  mSensor->setWaitForConversion(false);
  mSensor->getAddress(mAddress, FIRST_DEVICE);
}

DS18B20::~DS18B20() {
  delete mSensor;
  delete mIO;
  delete mAddress;
}

float DS18B20::getTemperature() {
  return mSensor->getTempCByIndex(FIRST_DEVICE);
}

void DS18B20::requestTemperature() {
  mSensor->requestTemperaturesByAddress(mAddress);
}
bool DS18B20::isConversionReady() {
  return mSensor->isConversionAvailable(mAddress);
}
