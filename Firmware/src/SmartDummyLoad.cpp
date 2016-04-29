#include <ClickEncoder.h>
#include <DallasTemperature.h>
#include <OneWire.h>
#include <SPI.h>
#include <TimerOne.h>

#include "AnalogInput.h"
#include "Button.h"
#include "DS18B20.h"
#include "GLCD.h"
#include "MCP23S17.h"
#include "MCP23S17GLCDMapping.h"
#include "MCP4921.h"
#include "NativeMapping.h"
#include "Output.h"
#include "bitmaps.h"
#include "definitions.h"
#include "fonts.h"
#include "pins.h"

/*MCP4921 dac(CS_DAC);
Output fan1(FAN1);
Output fan2(FAN2);
Output loadEnabledIndicator(LED1);
Output thermalShutdownIndicator(LED2);

ClickEncoder encoder(ENC_A, ENC_B, ENC_BTN, STEPS_PER_NOTCH);

Button button1(BUTTON_A, true);
Button button2(BUTTON_B, true);

AnalogInput loadVoltage(VLOAD, VLOAD_CONVERSION_FACTOR, ADC_DENOISE_SAMPLES);
AnalogInput loadCurrent(ILOAD, ILOAD_CONVERSION_FACTOR, ADC_DENOISE_SAMPLES,
                        REFERENCE_INTERNAL);

DS18B20 temperature1(TEMP_SENS1);
DS18B20 temperature2(TEMP_SENS2);
*/
MCP23S17 ioExpander(CS_GPIO);

MCP23S17GLCDMapping mapping(&ioExpander);

GLCD lcd(&mapping);
/*
int16_t last, value;

void timerIsr() {
  encoder.service();
  button1.update();
  button2.update();
}
*/
void setup() {
  Serial.begin(9600);
  /*dac.setConfiguration(true, false, false);
  fan1.switchOff();
  fan2.switchOff();

  loadEnabledIndicator.switchOn();
  thermalShutdownIndicator.switchOn();



  Timer1.initialize(1000);
  Timer1.attachInterrupt(timerIsr);

  encoder.setAccelerationEnabled(true);*/
}

void loop() {
  // dac.setValue(2000);

  /*Serial.print("Temp 1 : ");
  Serial.println(temperature1.getTemperature());
  Serial.print("Temp 2 : ");
  Serial.println(temperature2.getTemperature());

  value += encoder.getValue();

  Serial.println(button1.getStatus());
  if (value != last) {

    Serial.print("Encoder Value: ");
    Serial.println(value);
    last = value;
}
  Serial.print("I: ");
  Serial.println(loadCurrent.readConverted());
  Serial.print("U: ");
  Serial.println(loadVoltage.readConverted());
  delay(100);
  static uint8_t i = 0x77;
  ioExpander.portWrite(PORT_B, i++);*/
  Serial.println("done");
  lcd.fill(0x00);
  // lcd.fill(0xff);
  // lcd.fill(0xf0);
  // lcd.clearScreen();
  for (int i = 0; i < 6; i++) {
    lcd.drawBitmap(nupoLogo, 32, 16);

    /*lcd.goTo(0, 0);
    lcd.printChar(bigFont, '0' + i / 10);
    lcd.printChar(bigFont, '0' + i % 10);
    lcd.printString(bigFont, "mA\n 00mV\n");
    lcd.printString(smallFont, "This is testing Fonts");*/
    // lcd.drawLine(0, 32, 127, i * 10);
    // lcd.drawRect(1, 1, 126, 62);
    // lcd.drawRect(1, 1, 126, 62, COLOR_OFF);
    lcd.goTo(0, 0);
    lcd.printString(smallFont, "This is testing Fonts");
    lcd.invertRect(0, 0, 10, 10);
    delay(100);
  }

  /*ioExpander.portMode(PORT_B, INPUT);
  if (ioExpander.portRead(PORT_B)) {
    Serial.println("null");
  } else {
    Serial.println("non null");
}*/
  // Serial.println("asdasdkjhlkjhlkjhlkjhl");
}
