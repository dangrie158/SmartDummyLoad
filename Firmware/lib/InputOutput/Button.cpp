#include "Button.h"

// ----------------------------------------------------------------------------
// Button configuration (values for 1ms timer service calls)
//
#define ENC_BUTTONINTERVAL 10   // debounce time
#define ENC_DOUBLECLICKTIME 600 // second click within 600ms
#define ENC_HOLDTIME 1200       // report held button after 1.2s

Button::Button(uint8_t pinNumber, bool internalPullupEnabled, bool activeLow)
    : Input(pinNumber, activeLow, internalPullupEnabled), mCurrentStatus(Open) {
}

status_t Button::getStatus() {
  status_t status = mCurrentStatus;
  // only fire click event once
  if (status == Clicked) {
    mCurrentStatus = Open;
  }
  return status;
}

void Button::update() {
  unsigned long now = millis();

  static uint16_t keyDownTicks = 0;
  static unsigned long lastButtonCheck = 0;

  if ((now - lastButtonCheck) >=
      ENC_BUTTONINTERVAL) // checking button is sufficient every 10-30ms
  {
    lastButtonCheck = now;

    if (this->read()) { // key is down
      keyDownTicks++;
      if (keyDownTicks > (ENC_HOLDTIME / ENC_BUTTONINTERVAL)) {
        this->mCurrentStatus = Held;
      } else {
        this->mCurrentStatus = Closed;
      }
    } else { // key is now up
      if (keyDownTicks) {
        if (this->mCurrentStatus == Held) {
          this->mCurrentStatus = Open;
        } else {
          this->mCurrentStatus = Clicked;
        }
        keyDownTicks = 0;
      }
    }
  }
}
