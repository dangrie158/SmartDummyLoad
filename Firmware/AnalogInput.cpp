#include "AnalogInput.h"

volatile uint8_t currentReference = REFERENCE_EXTERNAL;

AnalogInput::AnalogInput(uint8_t pinNumber, float conversionFactor,
                         uint8_t numSamples, uint8_t reference)
    : Input(pinNumber, false, false), mConversionFactor(conversionFactor),
      mNumSamples(numSamples), mReference(reference) {
  pinMode(this->mPin, INPUT);
}

uint16_t AnalogInput::read() {
  if (currentReference != this->mReference) {
    this->switchToOwnReference();
    currentReference = this->mReference;
  }

  uint32_t sum = 0;

  for (uint8_t i = 0; i < this->mNumSamples; ++i) {
    sum += analogRead(this->mPin);
  }

  return (uint16_t)(sum / this->mNumSamples);
}

void AnalogInput::switchToOwnReference() {
  if (this->mReference == REFERENCE_EXTERNAL) {
    analogReference(DEFAULT);
  } else if (this->mReference == REFERENCE_INTERNAL) {
    analogReference(INTERNAL);
  }
  for (uint8_t i = 0; i < VREF_PROBING_COUNT; i++) {
    analogRead(this->mPin);
  }
}

float AnalogInput::readConverted() {
  uint16_t value = this->read();
  float convertedValue = value * this->mConversionFactor;
  return convertedValue;
}
