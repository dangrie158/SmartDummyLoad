#include <avr/pgmspace.h>
#include <inttypes.h>

#ifndef FONT_H_
#define FONT_H_

class Font {
protected:
  const uint8_t *mFontData;
  uint8_t mWidth;
  uint8_t mHeight;
  uint8_t mOffset;
  uint8_t mMaxChar;

public:
  explicit Font(const uint8_t *fontData, uint8_t width, uint8_t height,
                uint8_t offset, uint8_t maxChar)
      : mFontData(fontData), mWidth(width), mHeight(height), mOffset(offset),
        mMaxChar(maxChar){};

  inline const uint8_t getHeight() const { return mHeight; }
  inline const uint8_t getWidth() const { return mWidth; }
  inline const uint8_t getOffset() const { return mOffset; }
  inline const uint8_t getMaxChar() const { return mMaxChar; }
  inline const uint8_t isPrintable(const char c) const {
    return (c >= this->mOffset) && (c <= this->mMaxChar);
  }

  inline const uint8_t *getCharacterAddress(char character) const {
    return mFontData +
           (this->getNumBytesPerChar() * (character - this->getOffset()));
  }

  inline const uint8_t getNumBytesPerChar() const {
    return (this->getWidth() * this->getHeight()) / 8;
  }
};
#endif // FONT_H_
