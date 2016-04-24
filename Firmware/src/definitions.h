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
#define VLOAD_MAXVALUE (VREF_EXTERNAL) / (VLOAD_R2 / (VLOAD_R1 + VLOAD_R2))
// value of the shunt resistor used to measure the current
#define SHUNT_R 0.1f

// the final conversion factor to get the actual VLOAD from the 10 bit ADC
#define VLOAD_CONVERSION_FACTOR (VLOAD_MAXVALUE / ADC_MAXVALUE)

#define ILOAD_CONVERSION_FACTOR ((VREF_INTERNAL / SHUNT_R) / ADC_MAXVALUE)

// Steps per Notch for the Encoder
#define STEPS_PER_NOTCH 4
