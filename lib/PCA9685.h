//
// Created by nikesh on 6/8/23.
//

#ifndef DELTARHO_CONTROLLER_PCA9685_H
#define DELTARHO_CONTROLLER_PCA9685_H

#include "I2C.h"
#include "pico/stdio.h"
#include "pico/stdlib.h"

/**
 * Sets the pwm signal for the motor
 * @param channel
 * @param effort
 */
static inline void setPWM(int channel, int effort) {
  uint8_t offset = channel * 4;
  uint8_t offLow = effort & 0xFF; // First 8 bits of the 12 bit value
  uint8_t offHigh = effort >> 8;  // Last 4 bits of the 12 bits
  reg_write(i2c_default, PCA9685_ADDRESS, LED0_ON_L + offset, 0, 1);
  reg_write(i2c_default, PCA9685_ADDRESS, LED0_ON_H + offset, 0, 1);
  reg_write(i2c_default, PCA9685_ADDRESS, LED0_OFF_L + offset, &offLow, 1);
  reg_write(i2c_default, PCA9685_ADDRESS, LED0_OFF_H + offset, &offHigh, 1);
  printf("OffLow = %i and OffHigh = %i\n", offLow, offHigh);
}

/**
 * Inline function that will set the level of the pins responsible for
 * determining the direction to be either high or low depending on the input. A
 * true value for level indicates high and a low value indicates low
 * @param channel the channel the input line is connected to
 * @param level a boolean that sets the line high or low. True = high and False
 * = low
 */
static inline void setLevel(int channel, bool level) {
  if (level)
    setPWM(channel, PCA_HIGH);
  else
    setPWM(channel, PCA_LOW);
}

#endif // DELTARHO_CONTROLLER_PCA9685_H
