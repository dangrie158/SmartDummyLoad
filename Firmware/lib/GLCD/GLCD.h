#include <inttypes.h>

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
#define CLEAR_PATTERN 0x00

#define CHIP0 0
#define CHIP1 1

class GLCD {
private:
  IOMapping *mIo;
  uint8_t mPosX;
  uint8_t mPosY;

  void delay();
  void enableController(uint8_t controller);
  void disableAllController();

  void writeCommand(uint8_t command, uint8_t controller);
  uint8_t readStatus(uint8_t controller);
  uint8_t readData();
  void writeData(uint8_t data);

public:
  GLCD(IOMapping *ioMapping);
  ~GLCD();

  void initialize();
  void goTo(uint8_t x, uint8_t y);
  void fill(uint8_t pattern);
  void clearScreen();
  void setPixel(uint8_t x, uint8_t y, uint8_t color);
};

#endif // GLCD_H_
