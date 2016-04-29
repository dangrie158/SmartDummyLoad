#include "Font.h"
#include <avr/pgmspace.h>
#include <inttypes.h>

#ifndef FONT12X16_H_
#define FONT12X16_H_

class Font12x16 : public Font {
public:
  Font12x16();
  const uint8_t *getCharacterAddress(char character) const;
  const uint8_t getNumBytesPerChar() const;
};
#endif // FONT12X16_H_
