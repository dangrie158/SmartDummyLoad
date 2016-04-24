#include <ClickEncoder.h>
#include <DallasTemperature.h>
#include <OneWire.h>
#include <SPI.h>
#include <TimerOne.h>

#include "AnalogInput.h"
#include "Button.h"
#include "DS18B20.h"
#include "MCP23S17.h"
#include "MCP4921.h"
#include "Output.h"
#include "definitions.h"
#include "pins.h"

MCP4921 dac(CS_DAC);
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

MCP23S17 ioExpander(CS_GPIO);

int16_t last, value;

void timerIsr() {
  encoder.service();
  button1.update();
  button2.update();
}

void setup() {
  dac.setConfiguration(true, false, false);
  fan1.switchOff();
  fan2.switchOff();

  loadEnabledIndicator.switchOn();
  thermalShutdownIndicator.switchOn();

  Serial.begin(9600);

  Timer1.initialize(1000);
  Timer1.attachInterrupt(timerIsr);

  encoder.setAccelerationEnabled(true);
}

void loop() {
  dac.setValue(2000);

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
  Serial.println(loadVoltage.readConverted());*/
  // delay(100);
  static uint8_t i = 0x77;
  ioExpander.portWrite(PORT_B, i++);
}
