#include "inttypes.h"
void updateOutputState(bool newState);
void setCurrent(uint16_t milliAmp);
void emergencyShutdown();
void display();
void handleTemperature();
void handleHID();
void handleSerial();
void sendFloatValue(const char *key, float value);
void sendIntValue(const char *key, int value);
