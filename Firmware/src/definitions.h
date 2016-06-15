// number of samples to average for a single ADC conversion
#define ADC_DENOISE_SAMPLES 100

// first resistorvalue from voltage divider
#define VLOAD_R1 10.0f // 10k ohm
// second resistorvalue from voltage divider
#define VLOAD_R2 3.0f // 3k ohm
// ADCs reference voltage
#define VREF_EXTERNAL 5.0f
// ADCs internal 1.1v reference
#define VREF_INTERNAL 1.1f
// 10 bit ADCs max value (2^10)
#define ADC_MAXVALUE 1024
// VLOAD if the ADC reads ADC_MAXVALUE
#define VLOAD_MAXVALUE ((VREF_EXTERNAL) / (VLOAD_R2 / (VLOAD_R1 + VLOAD_R2)))
// value of the shunt resistor used to measure the current
#define SHUNT_R 0.1f

// the final conversion factor to get the actual VLOAD from the 10 bit ADC
#define VLOAD_CONVERSION_FACTOR (VLOAD_MAXVALUE / ADC_MAXVALUE)

#define ILOAD_MAXVALUE (VREF_INTERNAL / SHUNT_R)

#define ILOAD_CONVERSION_FACTOR (ILOAD_MAXVALUE / ADC_MAXVALUE)

// Steps per Notch for the Encoder
#define STEPS_PER_NOTCH 4

// value to calculate the dac value from a given milliamp value
// the dac has a VREF of 1024mV and has 12 bit (max value 4096)
#define MA_TO_DAC_VALUE_FACTOR 4
// the shunt is 0.1ohm so we need to multiply the voltage by that
// value (or divide by 1/R_SHUNT to avoid floating point conversions)
#define MA_TO_DAC_VALUE_DIVISOR 10

#define OFF false
#define ON true

#define COARSE_ADJUSTMENT_FACTOR 10

#define MIN_LOAD 0
#define MAX_LOAD 5000
