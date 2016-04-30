#include "GLCD.h"

// Theese two macros ae copied from openGLCD Lib
#define ABS_DIFF(x, y) ((x > y) ? (x - y) : (y - x))
#define SWAP(a, b)                                                             \
  do {                                                                         \
    uint8_t t;                                                                 \
    t = a;                                                                     \
    a = b;                                                                     \
    b = t;                                                                     \
  } while (0)

GLCD::GLCD(IOMapping *ioMapping) : mIo(ioMapping), mPosX(0), mPosY(0) {
  this->initialize();
}

GLCD::~GLCD() {}

void GLCD::initialize() {
  mIo->initializeIO();
  this->disableController(0);
  this->disableController(1);

  for (uint8_t i = 0; i < NUM_CHIPS; i++) {
    this->writeCommand((LCD_ON), i);
  }
  for (uint8_t i = 0; i < NUM_CHIPS; i++) {
    this->writeCommand((LCD_DISP_START), i);
  }
}

void GLCD::delay() {}
void GLCD::enableController(uint8_t controller) { mIo->setCsPin(controller); }
void GLCD::disableController(uint8_t controller) {
  mIo->clearCsPin(controller);
}

uint8_t GLCD::readStatus(uint8_t controller) {
  uint8_t status;

  mIo->prepareDataInput();
  mIo->setRwPin();
  mIo->clearDiPin();
  this->enableController(controller);
  mIo->setEnPin();
  this->delay();
  status = mIo->readData();
  mIo->clearEnPin();
  this->disableController(controller);

  return status;
}

void GLCD::writeCommand(uint8_t command, uint8_t controller) {
  // while (this->readStatus(controller) & DISPLAY_STATUS_BUSY)
  ;
  mIo->prepareDataOutput();
  mIo->clearRwPin();
  mIo->clearDiPin();
  this->enableController(controller);
  mIo->writeData(command);
  mIo->setEnPin();
  this->delay();
  mIo->clearEnPin();
  this->disableController(controller);
}

uint8_t GLCD::readData() {
  unsigned char data;
  // while (this->readStatus(this->mPosX / CHIP_WIDTH) & DISPLAY_STATUS_BUSY)
  ;
  mIo->prepareDataInput();
  mIo->setRwPin();
  mIo->setDiPin();
  this->enableController(this->mPosX / CHIP_WIDTH);
  mIo->setEnPin();
  this->delay();
  data = mIo->readData();
  mIo->clearEnPin();
  this->disableController(this->mPosX / CHIP_WIDTH);

  this->mPosX++;

  return data;
}

void GLCD::writeData(uint8_t data) {
  // while (this->readStatus(this->mPosX / CHIP_WIDTH) & DISPLAY_STATUS_BUSY)
  ;

  uint8_t yOffset = this->mPosY % PAGE_SIZE;
  if (yOffset != 0) {
    uint8_t tempData, tempX, tempY;
    // we're witing over a page boundary,
    // we need to read 2 positions and merge the
    // new data
    tempX = mPosX;
    tempY = mPosY;
    this->goTo(tempX, tempY);
    tempData = this->readData(); // dummy read
    this->goTo(tempX, tempY);
    tempData = this->readData(); // "real" read

    tempData |= data << yOffset;
    this->goTo(tempX, tempY);
    this->writeData(tempData);

    // second page
    tempY += PAGE_SIZE;
    this->goTo(tempX, tempY);
    tempData = this->readData(); // dummy read
    this->goTo(tempX, tempY);
    tempData = this->readData(); // "real" read
    this->goTo(tempX, tempY);

    tempData |= data >> (PAGE_SIZE - yOffset);
    this->writeData(tempData);
  } else {
    mIo->prepareDataOutput();
    mIo->clearRwPin();
    mIo->setDiPin();
    this->enableController(this->mPosX / CHIP_WIDTH);
    mIo->writeData(data);
    mIo->setEnPin();
    this->delay();
    mIo->clearEnPin();
    this->disableController(this->mPosX / CHIP_WIDTH);
  }

  this->mPosX++;
}

void GLCD::goTo(uint8_t x, uint8_t y) {
  this->mPosX = x;
  this->mPosY = y;

  for (uint8_t i = 0; i < SCREEN_WIDTH / CHIP_WIDTH; i++) {
    this->writeCommand(LCD_SET_ADD | 0, i);
    this->writeCommand(LCD_SET_PAGE | (y / PAGE_SIZE), i);
    this->writeCommand(DISPLAY_START_LINE | 0, i);
  }

  this->writeCommand(LCD_SET_ADD | (x % CHIP_HEIGHT), (x / CHIP_HEIGHT));
  this->writeCommand(LCD_SET_PAGE | (y / PAGE_SIZE), (x / CHIP_WIDTH));
}

void GLCD::fill(uint8_t pattern) {
  for (uint8_t j = 0; j < SCREEN_HEIGHT / PAGE_SIZE; j++) {
    this->goTo(0, j * PAGE_SIZE);
    for (uint8_t i = 0; i < SCREEN_WIDTH; i++)
      this->writeData(pattern);
  }
}

void GLCD::clearScreen() { this->fill(CLEAR_PATTERN); }

void GLCD::setPixel(uint8_t x, uint8_t y, uint8_t color) {
  unsigned char tmp;
  this->goTo(x, ((y / 8) * 8));
  tmp = this->readData();
  this->goTo(x, ((y / 8) * 8));
  tmp = this->readData();
  this->goTo(x, ((y / 8) * 8));
  if (color) {
    tmp |= (1 << (y % PAGE_SIZE));
  } else {
    tmp &= ~(1 << (y % PAGE_SIZE));
  }
  this->writeData(tmp);
}

void GLCD::printChar(const Font &font, char c) {
  uint8_t pages, tmp, i, j;

  if (c == '\n') {
    this->printNewLine(font.getHeight(), 0);
    return;
  }

  if (!font.isPrintable(c)) {
    // ignore characters that are not in the font
    return;
  }

  uint8_t xPos = this->mPosX; // save old coordinates
  uint8_t yPos = this->mPosY;

  pages = font.getHeight() / PAGE_SIZE; // calculate pages
  if (font.getHeight() % 8 != 0)
    pages++;

  for (j = 0; j < pages; j++) {
    for (i = j; i < font.getWidth() * pages; i += pages) {
      tmp = pgm_read_byte(font.getCharacterAddress(c) + i);
      this->writeData(tmp);
    }
    // go to next page
    this->goTo(xPos, this->mPosY + PAGE_SIZE);
  }
  // go to the upper right corner
  this->goTo(this->mPosX + font.getWidth(), yPos);
}

void GLCD::printNewLine(uint8_t fontHeight, uint8_t offset) {
  if (this->mPosY + fontHeight < 64)
    this->goTo(offset, this->mPosY + fontHeight);
  else
    this->goTo(offset, 0);
}

void GLCD::printString(const Font &font, const char *string) {
  while (*string) {
    this->printChar(font, *string);
    string++;
  }
}

void GLCD::drawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2,
                    uint8_t color) {
  uint8_t width = ABS_DIFF(x1, x2);
  uint8_t height = ABS_DIFF(y1, y2);

  // check if the line is either horizontal or vertical to simplify things
  if (width == 1) {
    return this->drawVLine(x1, y1, height, color);
  } else if (height == 1) {
    return this->drawHLine(x1, y1, width, color);
  }

  uint8_t steep = height > width;

  if (steep) {
    SWAP(x1, y1);
    SWAP(x2, y2);
  }

  if (x1 > x2) {
    SWAP(x1, x2);
    SWAP(y1, y2);
  }

  uint8_t deltaX = x2 - x1;
  uint8_t deltaY = ABS_DIFF(y2, y1);
  int8_t error = deltaX / 2;

  uint8_t yStep;
  if (y1 < y2) {
    yStep = 1;
  } else {
    yStep = -1;
  }

  uint8_t y = y1;
  for (uint8_t x = x1; x <= x2; x++) {
    if (steep) {
      this->setPixel(y, x, color);
    } else {
      this->setPixel(x, y, color);
    }
    error = error - deltaY;
    if (error < 0) {
      y += yStep;
      error += deltaX;
    }
  }
}

void GLCD::walkRect(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2,
                    uint8_t color, uint8_t invert) {
  uint8_t height = ABS_DIFF(y1, y2) + 1;
  uint8_t width = ABS_DIFF(x1, x2) + 1;

  uint8_t pageOffset = y1 % PAGE_SIZE;
  y1 -= pageOffset;
  uint8_t mask = FILL_PATTERN;

  uint8_t currentStartHeight;

  if (height < PAGE_SIZE - pageOffset) {
    mask >>= (PAGE_SIZE - height);
    currentStartHeight = height;
  } else {
    currentStartHeight = PAGE_SIZE - pageOffset;
  }

  mask <<= pageOffset;
  this->drawPageLine(x1, y1, width, mask, color, invert);

  while (currentStartHeight + PAGE_SIZE <= height) {
    currentStartHeight += PAGE_SIZE;
    y1 += PAGE_SIZE;

    this->goTo(x1, y1);

    this->drawPageLine(x1, y1, width, FILL_PATTERN, color, invert);
  }

  if (currentStartHeight < height) {
    mask = ~(FILL_PATTERN << (height - currentStartHeight));
    y1 += PAGE_SIZE;

    this->drawPageLine(x1, y1, width, mask, color, invert);
  }
}

void GLCD::drawPageLine(uint8_t x, uint8_t y, uint8_t width, uint8_t mask,
                        uint8_t color, uint8_t invert) {
  for (uint8_t i = 0; i < width; i++) {
    uint8_t data = 0x00;
    // check if we write over the whole page
    if (mask != FILL_PATTERN || invert == INVERT) {
      this->goTo(x + i, y);
      data = this->readData();
      this->goTo(x + i, y);
      data = this->readData();
      this->goTo(x + i, y);
    }

    if (invert == INVERT) {
      // xor the invertion with the mask
      data = (data & ~mask) | (~data & mask);
    } else if (color == COLOR_ON) {
      data |= mask;
    } else if (color == COLOR_OFF) {
      data &= ~mask;
    }

    this->writeData(data);
  }
}

void GLCD::drawHLine(uint8_t x, uint8_t y, uint8_t width, uint8_t color) {
  this->fillRect(x, y, x + width, y, color);
}

void GLCD::drawVLine(uint8_t x, uint8_t y, uint8_t height, uint8_t color) {
  this->fillRect(x, y, x, y + height, color);
}

void GLCD::drawRect(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2,
                    uint8_t color) {
  uint8_t width = ABS_DIFF(x1, x2);
  uint8_t height = ABS_DIFF(y1, y2);

  drawHLine(x1, y1, width, color);
  drawHLine(x1, y2, width, color);
  drawVLine(x1, y1, height, color);
  drawVLine(x2, y1, height, color);
}

void GLCD::fillRect(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2,
                    uint8_t color) {
  this->walkRect(x1, y1, x2, y2, color, NO_INVERT);
}

void GLCD::invertRect(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2) {
  this->walkRect(x1, y1, x2, y2, COLOR_OFF, INVERT);
}

void GLCD::drawBitmap(const Bitmap &bitmap, uint8_t x, uint8_t y) {
  for (uint8_t j = 0; j < (bitmap.getHeight() / PAGE_SIZE); j++) {
    this->goTo(x, y + (j * PAGE_SIZE));
    for (uint8_t i = 0; i < bitmap.getWidth(); i++) {
      this->writeData(bitmap.getPageBlock(i, j));
    }
  }
}
