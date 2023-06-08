//
// Created by nikesh on 5/29/23.
//

#include "I2C.h"
#include <math.h>
#include <pico/printf.h>

/**
 * Initializes the pico i2c connections using the default i2c
 */
void __init__i2c__() {
  // setup the I2C communication
  i2c_init(i2c_default, 400 * 1000);
  gpio_set_function(SDA, GPIO_FUNC_I2C);
  gpio_set_function(SCL, GPIO_FUNC_I2C);
  gpio_pull_up(SDA);
  gpio_pull_up(SCL);
}

/**
 * sets the base output frequency for the PCA chip. The default value is set to
 * be 200hz, but it is easy to change
 */
void __init__PCA() {

  uint8_t oldState;

  reg_read(i2c_default, PCA9685_ADDRESS, MODE1, &oldState, 1);

  uint8_t newState = (oldState & 0x7F) | 0x10; // sleep state
  uint8_t buf[] = {MODE1, newState};
  // put to sleep
  reg_write(i2c_default, PCA9685_ADDRESS, MODE1, &newState, 1);
  // Set the prescale values
  uint8_t prescaleVal =
      roundf(PCA_CLOCK_FREQ / (PCA_PWM_RESOLUTION * MIN_OUTPUT_FREQ)) - 1;
  // updates the prescale value
  reg_write(i2c_default, PCA9685_ADDRESS, PRESCALE_REGISTER, &prescaleVal, 1);

  reg_write(i2c_default, PCA9685_ADDRESS, MODE1, &oldState, 1);
  sleep_us(500); // stops sleep and allows the oscillator to stabilize
  buf[1] = oldState | 0x80; // sets the reset register to be 1
  reg_write(i2c_default, PCA9685_ADDRESS, MODE1, &oldState, 1);
  printf("Should have updated the PCA9685");
}

// Read byte(s) from specified register. If nbytes > 1, read from consecutive
// registers.
int reg_read(i2c_inst_t *i2c, const uint addr, const uint8_t reg, uint8_t *buf,
             const uint8_t nbytes) {

  int num_bytes_read = 0;

  // Check to make sure caller is asking for 1 or more bytes
  if (nbytes < 1) {
    return 0;
  }
  // Read data from register(s) over I2C
  i2c_write_blocking(i2c, addr, &reg, 1, true);
  num_bytes_read = i2c_read_blocking(i2c, addr, buf, nbytes, false);

  return num_bytes_read;
}

// Write 1 byte to the specified register
int reg_write(i2c_inst_t *i2c, const uint addr, const uint8_t reg, uint8_t *buf,
              const uint8_t nbytes) {

  int num_bytes_read = 0;
  uint8_t msg[nbytes + 1];

  // Check to make sure caller is sending 1 or more bytes
  if (nbytes < 1) {
    return 0;
  }

  // Append register address to front of data packet
  msg[0] = reg;
  for (int i = 0; i < nbytes; i++) {
    msg[i + 1] = buf[i];
  }

  // Write data to register(s) over I2C
  i2c_write_blocking(i2c, addr, msg, (nbytes + 1), false);

  return num_bytes_read;
}
