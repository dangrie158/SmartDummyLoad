#include "NativeMapping.h"
#include "Arduino.h"

NativeMapping::NativeMapping(uint8_t cs1Pin, uint8_t cs2Pin, uint8_t enPin,
                             uint8_t rwPin, uint8_t diPin, uint8_t d0Pin,
                             uint8_t d1Pin, uint8_t d2Pin, uint8_t d3Pin,
                             uint8_t d4Pin, uint8_t d5Pin, uint8_t d6Pin,
                             uint8_t d7Pin)
    : mCs1Pin(cs1Pin), mCs2Pin(cs2Pin), mEnPin(enPin), mRwPin(rwPin),
      mDiPin(diPin), mD0Pin(d0Pin), mD1Pin(d1Pin), mD2Pin(d2Pin), mD3Pin(d3Pin),
      mD4Pin(d4Pin), mD5Pin(d5Pin), mD6Pin(d6Pin), mD7Pin(d7Pin) {}

void NativeMapping::initializeIO() {
  pinMode(mD0Pin, OUTPUT);
  pinMode(mD1Pin, OUTPUT);
  pinMode(mD2Pin, OUTPUT);
  pinMode(mD3Pin, OUTPUT);
  pinMode(mD4Pin, OUTPUT);
  pinMode(mD5Pin, OUTPUT);
  pinMode(mD6Pin, OUTPUT);
  pinMode(mD7Pin, OUTPUT);

  pinMode(mCs1Pin, OUTPUT); // CS1
  pinMode(mCs2Pin, OUTPUT); // CS2

  pinMode(mDiPin, OUTPUT); // DI
  pinMode(mRwPin, OUTPUT); // RW
  pinMode(mEnPin, OUTPUT); // EN
}

// set all data pins as output
void NativeMapping::prepareDataOutput() {
  pinMode(mD0Pin, OUTPUT);
  pinMode(mD1Pin, OUTPUT);
  pinMode(mD2Pin, OUTPUT);
  pinMode(mD3Pin, OUTPUT);
  pinMode(mD4Pin, OUTPUT);
  pinMode(mD5Pin, OUTPUT);
  pinMode(mD6Pin, OUTPUT);
  pinMode(mD7Pin, OUTPUT);
}
void NativeMapping::prepareDataInput() {
  pinMode(mD0Pin, INPUT);
  pinMode(mD1Pin, INPUT);
  pinMode(mD2Pin, INPUT);
  pinMode(mD3Pin, INPUT);
  pinMode(mD4Pin, INPUT);
  pinMode(mD5Pin, INPUT);
  pinMode(mD6Pin, INPUT);
  pinMode(mD7Pin, INPUT);
}
void NativeMapping::writeData(uint8_t data) {
  digitalWrite(mD0Pin, (data >> 0) & 1);
  digitalWrite(mD1Pin, (data >> 1) & 1);
  digitalWrite(mD2Pin, (data >> 2) & 1);
  digitalWrite(mD3Pin, (data >> 3) & 1);
  digitalWrite(mD4Pin, (data >> 4) & 1);
  digitalWrite(mD5Pin, (data >> 5) & 1);
  digitalWrite(mD6Pin, (data >> 6) & 1);
  digitalWrite(mD7Pin, (data >> 7) & 1);
}
uint8_t NativeMapping::readData() {
  uint8_t data = 0;
  data |= (digitalRead(mD0Pin) << 0);
  data |= (digitalRead(mD1Pin) << 1);
  data |= (digitalRead(mD2Pin) << 2);
  data |= (digitalRead(mD3Pin) << 3);
  data |= (digitalRead(mD4Pin) << 4);
  data |= (digitalRead(mD5Pin) << 5);
  data |= (digitalRead(mD6Pin) << 6);
  data |= (digitalRead(mD7Pin) << 7);
  return data;
}

void NativeMapping::setRwPin() { digitalWrite(mRwPin, HIGH); }
void NativeMapping::clearRwPin() { digitalWrite(mRwPin, LOW); }
void NativeMapping::setDiPin() { digitalWrite(mDiPin, HIGH); }
void NativeMapping::clearDiPin() { digitalWrite(mDiPin, LOW); }
void NativeMapping::setEnPin() { digitalWrite(mEnPin, HIGH); }
void NativeMapping::clearEnPin() { digitalWrite(mEnPin, LOW); }
void NativeMapping::setCsPin(uint8_t controller) {
  switch (controller) {
  case 0:
    digitalWrite(mCs1Pin, HIGH);
    break;
  case 1:
    digitalWrite(mCs2Pin, HIGH);
    break;
  }
}
void NativeMapping::clearCsPin(uint8_t controller) {
  switch (controller) {
  case 0:
    digitalWrite(mCs1Pin, LOW);
    break;
  case 1:
    digitalWrite(mCs2Pin, LOW);
    break;
  }
}
