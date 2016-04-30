#include "ProgressBar.h"

ProgressBar::ProgressBar(GLCD *glcd, uint8_t x, uint8_t y, uint8_t width)
    : mGlcd(glcd), mXPos(x), mYPos(y), mWidth(width), mValue(0) {}

void ProgressBar::draw() {
  uint8_t fillWidth =
      ((int)(mWidth - (2 * PROGRESSBAR_BORDER)) * 100) / this->mValue;

  mGlcd->drawLine(mXPos, mYPos, mXPos + mWidth, mYPos);
  mGlcd->drawLine(mXPos, mYPos, mXPos, mYPos + PROGRESSBAR_HEIGHT);
  mGlcd->drawLine(mXPos, mYPos + PROGRESSBAR_HEIGHT, mXPos + mWidth,
                  mYPos + PROGRESSBAR_HEIGHT);
  mGlcd->drawLine(mXPos + mWidth, mYPos, mXPos + mWidth,
                  mYPos + PROGRESSBAR_HEIGHT);

  mGlcd->fillRect(mXPos + PROGRESSBAR_BORDER, mYPos + PROGRESSBAR_BORDER,
                  mXPos + fillWidth,
                  mYPos + PROGRESSBAR_HEIGHT - PROGRESSBAR_BORDER);
}

void ProgressBar::setValue(uint8_t value) {
  uint8_t oldFillWidth =
      ((int)(mWidth - (2 * PROGRESSBAR_BORDER)) * 100) / this->mValue;

  uint8_t fillWidth = ((int)(mWidth - (2 * PROGRESSBAR_BORDER)) * 100) / value;

  this->mValue = value;

  if (oldFillWidth > fillWidth) {
    // mGlcd->fillRect(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2,
    // COLOR_OFF);
  } else {
    // mGlcd->fillRect(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2,
    // COLOR_ON);
  }

  this->draw();
}
