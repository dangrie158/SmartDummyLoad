#ifndef PROGRESSBAR_H_
#define PROGRESSBAR_H_

#include "GLCD.h"
#include <inttypes.h>

#define PROGRESSBAR_BORDER 2
#define BORDER_PATTERN 0xFF
#define EMPTY_PATTERN 0x81
#define FULL_PATTERN 0xBD

class ProgressBar {
private:
  GLCD *mGlcd;
  uint8_t mXPos;
  uint8_t mYPos;
  uint8_t mWidth;
  uint8_t mValue;

public:
  ProgressBar(GLCD *glcd, uint8_t x, uint8_t y, uint8_t width);

  void draw();
  void setValue(uint8_t value);
};

#endif // PROGRESSBAR_H_
