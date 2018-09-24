#include <avr/pgmspace.h>
#include <inttypes.h>

#ifndef BITMAP_H_
#define BITMAP_H_

class Bitmap {
protected:
  const uint8_t *mBitmapData;
  uint8_t mWidth;
  uint8_t mHeight;

public:
  explicit Bitmap(const uint8_t *bitmapData, uint8_t width, uint8_t height)
      : mBitmapData(bitmapData), mWidth(width), mHeight(height){};

  inline const uint8_t getHeight() const { return mHeight; }
  inline const uint8_t getWidth() const { return mWidth; }
  inline const uint8_t getPageBlock(uint8_t x, uint8_t page) const {
    return pgm_read_byte(mBitmapData + (x + (page * this->getWidth())));
  }
};
#endif // BITMAP_H_
