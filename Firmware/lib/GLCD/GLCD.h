#include <inttypes.h>

#include "Bitmap.h"
#include "Font.h"
#include "IOMapping.h"

#ifndef GLCD_H_
#define GLCD_H_

#define DISPLAY_START_LINE 0xC0
#define DISPLAY_STATUS_BUSY 0x80
#define LCD_SET_ADD 0x40
#define LCD_SET_PAGE 0xB8

#define CHIP_WIDTH 64
#define CHIP_HEIGHT 64
#define PAGE_SIZE 8

#define LCD_ON 0x3F
#define LCD_DISP_START 0xC0

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define NUM_CHIPS ((SCREEN_WIDTH) / (CHIP_WIDTH))

#define CHIP0 0
#define CHIP1 1

#define COLOR_ON 0x01
#define COLOR_OFF 0x00
#define CLEAR_PATTERN 0x00
#define FILL_PATTERN 0xFF

#define NO_INVERT 0x00
#define INVERT 0x01

class GLCD {
private:
  IOMapping *mIo;
  uint8_t mPosX;
  uint8_t mPosY;

  void delay();
  void enableController(uint8_t controller);
  void disableController(uint8_t controller);

  void writeCommand(uint8_t command, uint8_t controller);
  uint8_t readStatus(uint8_t controller);
  uint8_t readData();

  void drawPageLine(uint8_t x, uint8_t y, uint8_t width, uint8_t mask,
                    uint8_t color, uint8_t invert);
  void walkRect(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2,
                uint8_t color = COLOR_ON, uint8_t invert = NO_INVERT);

  void drawHLine(uint8_t x, uint8_t y, uint8_t width, uint8_t color = COLOR_ON);
  void drawVLine(uint8_t x, uint8_t y, uint8_t height,
                 uint8_t color = COLOR_ON);

public:
  GLCD(IOMapping *ioMapping);
  ~GLCD();

  void initialize();
  void goTo(uint8_t x, uint8_t y);
  void writeData(uint8_t data);
  void fill(uint8_t pattern);
  void clearScreen();
  void setPixel(uint8_t x, uint8_t y, uint8_t color = COLOR_ON);
  void printChar(const Font &font, char c);
  void printString(const Font &font, const char *string);
  void printNewLine(uint8_t fontHeight, uint8_t offset);
  void drawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2,
                uint8_t color = COLOR_ON);
  void drawRect(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2,
                uint8_t color = COLOR_ON);
  void fillRect(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2,
                uint8_t color = COLOR_ON);
  void invertRect(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);
  void drawBitmap(const Bitmap &bitmap, uint8_t x, uint8_t y);
};

#endif // GLCD_H_
