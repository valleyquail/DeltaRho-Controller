//
// Created by nikesh on 5/29/23.
//

#include "I2C.h"
#include <math.h>

void __init__i2c__() {
  // setup the I2C communication
  i2c_init(i2c_default, 400 * 1000);
  gpio_set_function(SDA, GPIO_FUNC_I2C);
  gpio_set_function(SCL, GPIO_FUNC_I2C);
  gpio_pull_up(SDA);
  gpio_pull_up(SCL);
}

void __init__PCA() {

  uint8_t oldState;
  uint8_t modeRegister = MODE1;
  i2c_write_blocking(i2c_default, PCA9685_ADDRESS, &modeRegister, 1, true);
  i2c_read_blocking(i2c_default, PCA9685_ADDRESS, &oldState, 1, false);

  uint8_t newState = (oldState & 0x7F) | 0x10; // sleep state
  uint8_t buf[] = {MODE1, newState};
  i2c_write_blocking(i2c_default, PCA9685_ADDRESS, buf, 1,
                     true); // put to sleep
  // Set the prescale values
  int prescaleVal =
      roundf(PCA_CLOCK_FREQ / (PCA_PWM_RESOLUTION * MIN_OUTPUT_FREQ)) - 1;
  buf[0] = PRESCALE_REGISTER;
  buf[1] = prescaleVal;
  i2c_write_blocking(i2c_default, PCA9685_ADDRESS, buf, 1,
                     false); // updates the prescale value
  buf[0] = MODE1;
  buf[1] = oldState;
  i2c_write_blocking(i2c_default, PCA9685_ADDRESS, buf, 1, false); // Sets the
  sleep_us(500); // stops sleep and allows the oscillator to stabilize
  buf[1] = oldState | 0x80; // sets the reset register to be 1
  i2c_write_blocking(i2c_default, PCA9685_ADDRESS, buf, 1,
                     false); // Resets the device with the new prescale value
}
