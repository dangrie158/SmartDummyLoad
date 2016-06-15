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
#include "Progressbar.h"
#include "SmartDummyLoad.h"
#include "bitmaps.h"
#include "definitions.h"
#include "fonts.h"
#include "pins.h"

MCP4921 dac(CS_DAC);
Output fan1(FAN1);
Output fan2(FAN2);
Output loadEnabledIndicator(LED2);
Output thermalShutdownIndicator(LED1);

ClickEncoder encoder(ENC_A, ENC_B, ENC_BTN, STEPS_PER_NOTCH);

Button outputEnableButton(BUTTON_B, true);
Button button2(BUTTON_A, true);

AnalogInput loadVoltage(VLOAD, VLOAD_CONVERSION_FACTOR, ADC_DENOISE_SAMPLES);
AnalogInput loadCurrent(ILOAD, ILOAD_CONVERSION_FACTOR, ADC_DENOISE_SAMPLES,
                        REFERENCE_INTERNAL);

DS18B20 temperature1(TEMP_SENS1);
DS18B20 temperature2(TEMP_SENS2);

MCP23S17 ioExpander(CS_GPIO);
MCP23S17GLCDMapping mapping(&ioExpander);
GLCD lcd(&mapping);

ProgressBar voltageBar(&lcd, 2, 48, 91);
ProgressBar currentBar(&lcd, 2, 16, 91);

int16_t lastEncoderValue, encoderValue;
uint16_t currentDacValue;
uint16_t currentSetLoad;
bool outputState = OFF;
bool coarseEncoder = true;

void timerIsr() {
  encoder.service();
  outputEnableButton.update();
  button2.update();
}

void setup() {
  Serial.begin(9600);
  dac.setConfiguration(true, false, false);
  fan1.switchOff();
  fan2.switchOff();
  thermalShutdownIndicator.switchOff();
  loadEnabledIndicator.switchOff();

  Timer1.initialize(1000);
  Timer1.attachInterrupt(timerIsr);

  encoder.setAccelerationEnabled(true);

  lcd.fill(0x00);
  voltageBar.draw();
  currentBar.draw();
}

void loop() {
  setCurrent(encoderValue);

  handleTemperature();

  display();

  handleHID();
}

void setCurrent(uint16_t milliAmp) {
  if (milliAmp > MAX_LOAD) {
    encoderValue = MAX_LOAD;
    return;
  } else if (milliAmp < MIN_LOAD) {
    encoderValue = MIN_LOAD;
    return;
  }

  currentSetLoad = milliAmp;
  currentDacValue = milliAmp;
  if (outputState == true) {
    dac.setValue(currentDacValue);
  }
}

void updateOutputState(bool newState) {
  outputState = newState;
  if (newState == true) {
    thermalShutdownIndicator.switchOff();
    dac.setValue(currentDacValue);
    loadEnabledIndicator.switchOn();
  } else {
    dac.setValue(0);
    loadEnabledIndicator.switchOff();
  }
}

void emergencyShutdown() {
  updateOutputState(OFF);
  Serial.println("EMERGENCY_OFF");
  thermalShutdownIndicator.switchOn();
}

void display() {
  // print measured current
  char buffer[24];
  float current = loadCurrent.readConverted();
  sprintf(buffer, "%4dmA", (int)(current * 1000));
  lcd.goTo(2, 0);
  lcd.printString(bigFont, "I:");
  lcd.printString(bigFont, buffer);
  currentBar.setValue(((current * 1000) / MAX_LOAD) * 100);

  // print measured voltage
  float voltage = loadVoltage.readConverted();
  dtostrf(voltage, 5, 2, buffer);
  lcd.goTo(2, 32);
  lcd.printString(bigFont, "U:");
  lcd.printString(bigFont, buffer);
  lcd.printString(bigFont, "V");
  voltageBar.setValue((uint8_t)((voltage / VLOAD_MAXVALUE) * 100));

  // print set current
  lcd.goTo(2, 24);
  sprintf(buffer, "Set:%4dmA", currentDacValue);
  lcd.printString(smallFont, buffer);

  // print whether coarse control is enabled or not
  uint8_t controlTextStart = 110;
  lcd.goTo(controlTextStart, 56);
  lcd.printString(smallFont, "F C");
  uint8_t invertingOffset =
      coarseEncoder == ON ? controlTextStart + 11 : controlTextStart;
  lcd.invertRect(invertingOffset, 56, invertingOffset + 6, 63);

  // print the temperatures
  float temp1 = temperature1.getTemperature();
  float temp2 = temperature2.getTemperature();
  lcd.goTo(2, 56);
  sprintf(buffer, "T1:      T2:     ");
  dtostrf(temp1, 4, 1, buffer + 3);
  buffer[7] = 'C';
  dtostrf(temp2, 4, 1, buffer + 12);
  buffer[16] = 'C';
  buffer[17] = '\0';
  lcd.printString(smallFont, buffer);

  lcd.goTo(100, 0);
  lcd.printChar(symbolsFont, 0x21);
};
void handleTemperature() {
  temperature1.requestTemperature();
  temperature2.requestTemperature();

  float temp1 = temperature1.getTemperature();
  float temp2 = temperature2.getTemperature();

  // switch on fans to cool the MosFETs
  if (temp1 > 30 || temp2 > 30) {
    fan1.switchOn();
    fan2.switchOn();
  }

  // Hysteresis switch off the fans
  if (temp1 < 25 && temp2 < 25 && fan1.getStatus() == HIGH) {
    fan1.switchOff();
    fan2.switchOff();
  }

  // safety switch off the MosFETs when they get over 60°
  if (temp1 > 60 || temp2 > 60) {
    emergencyShutdown();
  }
}

void handleHID() {
  if (outputEnableButton.getStatus() == Clicked) {
    updateOutputState(!outputState);
  }

  int16_t encoderNewSteps = encoder.getValue();
  if (coarseEncoder == ON) {
    encoderValue += encoderNewSteps * COARSE_ADJUSTMENT_FACTOR;
  } else {
    encoderValue += encoderNewSteps;
  }

  ClickEncoder::Button buttonState = encoder.getButton();
  static bool wasHeld = false;
  switch (buttonState) {
  case ClickEncoder::Held:
    // toggle coarse control
    if (!wasHeld) {
      coarseEncoder = !coarseEncoder;
      wasHeld = true;
    }
    break;
  case ClickEncoder::Released:
    wasHeld = false;
    break;
  default:
    break;
  }

  if (encoderValue != lastEncoderValue) {
    lastEncoderValue = encoderValue;
  }
}
