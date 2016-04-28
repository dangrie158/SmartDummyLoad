#include "NativeMapping.h"
#include "Arduino.h"

NativeMapping::NativeMapping() {}

void NativeMapping::initializeIO() {
  pinMode(2, OUTPUT); // D1
  pinMode(3, OUTPUT); // D2
  pinMode(4, OUTPUT); // D3
  pinMode(5, OUTPUT); // D4
  pinMode(6, OUTPUT); // D5
  pinMode(7, OUTPUT); // D6
  pinMode(8, OUTPUT); // D7
  pinMode(9, OUTPUT); // D8

  pinMode(10, OUTPUT); // CS1
  pinMode(11, OUTPUT); // CS2

  pinMode(A0, OUTPUT); // DI
  pinMode(A1, OUTPUT); // RW
  pinMode(A2, OUTPUT); // EN
}

// set all data pins as output
void NativeMapping::prepareDataOutput() {
  pinMode(2, OUTPUT); // D1
  pinMode(3, OUTPUT); // D2
  pinMode(4, OUTPUT); // D3
  pinMode(5, OUTPUT); // D4
  pinMode(6, OUTPUT); // D5
  pinMode(7, OUTPUT); // D6
  pinMode(8, OUTPUT); // D7
  pinMode(9, OUTPUT); // D8
}
void NativeMapping::prepareDataInput() {
  pinMode(2, INPUT); // D1
  pinMode(3, INPUT); // D2
  pinMode(4, INPUT); // D3
  pinMode(5, INPUT); // D4
  pinMode(6, INPUT); // D5
  pinMode(7, INPUT); // D6
  pinMode(8, INPUT); // D7
  pinMode(9, INPUT); // D8
}
void NativeMapping::writeData(uint8_t data) {
  digitalWrite(2, (data >> 0) & 1); // D1
  digitalWrite(3, (data >> 1) & 1); // D2
  digitalWrite(4, (data >> 2) & 1); // D3
  digitalWrite(5, (data >> 3) & 1); // D4
  digitalWrite(6, (data >> 4) & 1); // D5
  digitalWrite(7, (data >> 5) & 1); // D6
  digitalWrite(8, (data >> 6) & 1); // D7
  digitalWrite(9, (data >> 7) & 1); // D8
}
uint8_t NativeMapping::readData() {
  uint8_t data = 0;
  data |= (digitalRead(2) << 0);
  data |= (digitalRead(3) << 1);
  data |= (digitalRead(4) << 2);
  data |= (digitalRead(5) << 3);
  data |= (digitalRead(6) << 4);
  data |= (digitalRead(7) << 5);
  data |= (digitalRead(8) << 6);
  data |= (digitalRead(9) << 7);
  return data;
}

void NativeMapping::setRwPin() { digitalWrite(A1, HIGH); }
void NativeMapping::clearRwPin() { digitalWrite(A1, LOW); }
void NativeMapping::setDiPin() { digitalWrite(A0, HIGH); }
void NativeMapping::clearDiPin() { digitalWrite(A0, LOW); }
void NativeMapping::setEnPin() { digitalWrite(A2, HIGH); }
void NativeMapping::clearEnPin() { digitalWrite(A2, LOW); }
void NativeMapping::setCsPin(uint8_t controller) {
  switch (controller) {
  case 0:
    digitalWrite(10, HIGH);
    break;
  case 1:
    digitalWrite(11, HIGH);
    break;
  }
}
void NativeMapping::clearCsPin(uint8_t controller) {
  switch (controller) {
  case 0:
    digitalWrite(10, LOW);
    break;
  case 1:
    digitalWrite(11, LOW);
    break;
  }
}
