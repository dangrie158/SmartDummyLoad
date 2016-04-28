#include "GLCD.h"

GLCD::GLCD(IOMapping *ioMapping) : mIo(ioMapping), mPosX(0), mPosY(0) {
  this->initialize();
}

GLCD::~GLCD() {}

void GLCD::initialize() {
  mIo->initializeIO();
  this->disableAllController();

  for (uint8_t i = 0; i < NUM_CHIPS; i++) {
    this->writeCommand((LCD_ON), i);
  }
  for (uint8_t i = 0; i < NUM_CHIPS; i++) {
    this->writeCommand((LCD_DISP_START), i);
  }
}

void GLCD::delay() {}
void GLCD::enableController(uint8_t controller) { mIo->setCsPin(controller); }
void GLCD::disableAllController() {
  mIo->clearCsPin(0);
  mIo->clearCsPin(1);
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
  this->disableAllController();

  return status;
}

void GLCD::writeCommand(uint8_t command, uint8_t controller) {
  while (this->readStatus(controller) & DISPLAY_STATUS_BUSY)
    ;
  mIo->prepareDataOutput();
  mIo->clearRwPin();
  mIo->clearDiPin();
  this->enableController(controller);
  mIo->writeData(command);
  mIo->setEnPin();
  this->delay();
  mIo->clearEnPin();
  this->disableAllController();
}

uint8_t GLCD::readData() {
  unsigned char data;
  while (this->readStatus(this->mPosX / CHIP_WIDTH) & DISPLAY_STATUS_BUSY)
    ;
  mIo->prepareDataInput();
  mIo->setRwPin();
  mIo->setDiPin();
  this->enableController(this->mPosX / CHIP_WIDTH);
  mIo->setEnPin();
  this->delay();
  data = mIo->readData();
  mIo->clearEnPin();
  this->disableAllController();

  this->mPosX++;

  return data;
}

void GLCD::writeData(uint8_t data) {
  while (this->readStatus(this->mPosX / CHIP_WIDTH) & DISPLAY_STATUS_BUSY)
    ;
  mIo->prepareDataOutput();
  mIo->clearRwPin();
  mIo->setDiPin();
  mIo->writeData(data);
  this->enableController(this->mPosX / CHIP_WIDTH);
  mIo->setEnPin();
  this->delay();
  this->disableAllController();

  this->mPosX++;
}

void GLCD::goTo(uint8_t x, uint8_t y) {
  this->mPosX = x;
  this->mPosY = y;

  for (uint8_t i = 0; i < SCREEN_WIDTH / CHIP_WIDTH; i++) {
    this->writeCommand(DISPLAY_SET_Y | 0, i);
    this->writeCommand(DISPLAY_SET_X | y, i);
    this->writeCommand(DISPLAY_START_LINE | 0, i);
  }

  this->writeCommand(DISPLAY_SET_Y | (x % CHIP_HEIGHT), (x / CHIP_HEIGHT));
  this->writeCommand(DISPLAY_SET_X | y, (x / CHIP_WIDTH));
}

void GLCD::fill(uint8_t pattern) {
  for (uint8_t j = 0; j < SCREEN_HEIGHT / 8; j++) {
    this->goTo(0, j);
    for (uint8_t i = 0; i < SCREEN_WIDTH; i++)
      this->writeData(pattern);
  }
}

void GLCD::clearScreen() { this->fill(CLEAR_PATTERN); }

void GLCD::setPixel(uint8_t x, uint8_t y, uint8_t color) {
  unsigned char tmp;
  this->goTo(x, (y / 8));
  tmp = this->readData();
  this->goTo(x, (y / 8));
  tmp = this->readData();
  this->goTo(x, (y / 8));
  tmp |= (1 << (y % 8));
  this->writeData(tmp);
}
