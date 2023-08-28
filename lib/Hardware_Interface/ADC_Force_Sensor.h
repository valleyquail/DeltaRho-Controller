//
// Created by nikesh on 7/6/23.
//

#ifndef DELTARHO_CONTROLLER_ADC_FORCE_SENSOR_H
#define DELTARHO_CONTROLLER_ADC_FORCE_SENSOR_H
#if __cpluspluss
extern "C" {
#endif

#include "hardware/adc.h"
#include "pico/stdlib.h"

#define FORCE_SENSOR_LEFT_PIN 26
#define FORCE_SENSOR_RIGHT_PIN 27

inline void init_ADC(void);

uint16_t *read_ADC_Values();
float *calculate_force_vector(uint16_t *values);
#if __cpluspluss
}
#endif

#endif // DELTARHO_CONTROLLER_ADC_FORCE_SENSOR_H
