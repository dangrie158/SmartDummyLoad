#include "Font.h"
#include <avr/pgmspace.h>
#include <inttypes.h>

#ifndef FONT6X8_H_
#define FONT6X8_H_

class Font6x8 : public Font {
public:
  Font6x8();
  const uint8_t *getCharacterAddress(char character) const;
  const uint8_t getNumBytesPerChar() const;
};
#endif // FONT6X8_H_
