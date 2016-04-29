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
    this->disableAllController();
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
  tmp |= (1 << (y % PAGE_SIZE));
  this->writeData(tmp);
}
