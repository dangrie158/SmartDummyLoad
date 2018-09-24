#include "ProgressBar.h"
#include "Arduino.h"

ProgressBar::ProgressBar(GLCD *glcd, uint8_t x, uint8_t y, uint8_t width)
    : mGlcd(glcd), mXPos(x), mYPos(y), mWidth(width), mValue(0) {}

void ProgressBar::draw() {
  mGlcd->goTo(mXPos, mYPos);
  mGlcd->writeData(BORDER_PATTERN);
  mGlcd->writeData(EMPTY_PATTERN);

  for (uint8_t i = 0; i < mWidth; i++) {
    mGlcd->writeData(EMPTY_PATTERN);
  }

  mGlcd->writeData(EMPTY_PATTERN);
  mGlcd->writeData(BORDER_PATTERN);
}

void ProgressBar::setValue(uint8_t value) {
  if (value > 100) {
    value = 100;
  }
  uint8_t oldFillWidth = ((int32_t)(mWidth) * this->mValue) / 100;

  uint8_t fillWidth = ((int32_t)(mWidth)*value) / 100;

  this->mValue = value;

  if (oldFillWidth > fillWidth) {
    mGlcd->goTo(fillWidth + mXPos + PROGRESSBAR_BORDER, mYPos);
    for (uint8_t i = 0; i < (oldFillWidth - fillWidth); i++) {
      mGlcd->writeData(EMPTY_PATTERN);
    }
  } else {
    mGlcd->goTo(oldFillWidth + mXPos + PROGRESSBAR_BORDER, mYPos);
    for (uint8_t i = 0; i < (fillWidth - oldFillWidth); i++) {
      mGlcd->writeData(FULL_PATTERN);
    }
  }
}
