//
// Created by nikesh on 7/6/23.
//

#ifdef __cplusplus
extern "C" {
#endif
#include "ADC_Force_Sensor.h"

#define FORCE_SENSOR_LEFT_INPUT 0
#define FORCE_SENSOR_RIGHT_INPUT 1

#define VREF = 3.3

// 12-bit conversion, assume max value == ADC_VREF == 3.3 V
const float conversion_factor = 3.3f / (1 << 12);

#define SPRING_CONSTANT 1 //

uint16_t left_initial_read, right_initial_read, left_val_range, right_val_range;
const uint16_t max_left_val = 1000, max_right_val = 1000;
const float initial_left_angle = 2.5, initial_right_angle = .5;
const float link_angle_range = 1;

static inline float *interpolate_angle(uint16_t *curr_reading);

/**
 * Initializes the ADC pins and then also stores the initial readings as an
 * offset
 */
inline void init_ADC(void) {
  adc_init();
  adc_gpio_init(FORCE_SENSOR_LEFT_PIN);
  adc_gpio_init(FORCE_SENSOR_RIGHT_PIN);
  uint16_t leftVal, rightVal;
  adc_select_input(FORCE_SENSOR_LEFT_PIN);
  left_initial_read = adc_read();
  adc_select_input(FORCE_SENSOR_RIGHT_PIN);
  right_initial_read = adc_read();
  left_val_range = max_left_val - left_initial_read;
  right_val_range = max_right_val - right_initial_read;
}

/**
 * Reads the ADCs connected to the force sensor
 * @return an int array containing the readings from the two ADCs
 */
uint16_t *read_ADC_Values() {
  uint16_t leftVal, rightVal;
  adc_select_input(FORCE_SENSOR_LEFT_PIN);
  leftVal = adc_read();
  adc_select_input(FORCE_SENSOR_RIGHT_PIN);
  rightVal = adc_read();
  uint16_t values[] = {leftVal, rightVal};
  return values;
}
/**
 *
 * @param values
 * @return
 */
float *calculate_force_vector(uint16_t *values) {

  float *angles = interpolate_angle(values);
  float left_torque = angles[0] * SPRING_CONSTANT;
  float right_torque = angles[1] * SPRING_CONSTANT;
  float angle = 0;
  float *vector = 0;
}

/**
 *  So, this is cursed, but a very,very fast way to approximate the angles of
 * the springs. It works by taking a reference baseline value stored during ADC
 * initialization and the predetermined max value of the ADC determined by the
 * physical limits of the sensor. By taking the current reading and determining
 * its placement between these two conditions, we can approximate the angle
 * using Hooke's Law
 * @param curr_reading the current ADC reading
 * @return a float array representing the springs' angles of deflection in
 * radians
 */
static inline float *interpolate_angle(uint16_t *curr_reading) {
  float left_angle = (curr_reading[0] - left_initial_read) / left_val_range;
  float right_angle = (right_initial_read - curr_reading[1]) / right_val_range;
  left_angle *= link_angle_range;
  right_angle *= link_angle_range;
  float angles[] = {left_angle, right_angle};
  return angles;
}

#ifdef __cplusplus
};
#endif