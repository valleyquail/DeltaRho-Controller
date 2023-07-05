//
// Created by nikesh on 5/29/23.
//

#ifndef DELTARHO_CONTROLLER_I2C_CONTROL_H
#define DELTARHO_CONTROLLER_I2C_CONTROL_H

#include "hardware/i2c.h"
#include "pico/stdio.h"
#include "pico/stdlib.h"

// Values that might want to be changed
//_____________________________________________________________________________
//  The signal frequency
#define UPDATE_FREQ (400 * 1000) // 400kbits/sec send rate
#define MIN_OUTPUT_FREQ 50       // PCA will output at this as the minimum (Hz)

// SDA and SCL pins on the Pico_W
#define SDA 20
#define SCL 21

// User-defined via setting the jumpers on the board itself
#define PCA9685_ADDRESS 0x40

// Datasheet for the TB6612FNG at:
// https://www.sparkfun.com/datasheets/Robotics/TB6612FNG.pdf

//_________________________________________________________________________
// Set up the constants for the TB6612FNG

//_________________________________________________________________________

void __init__i2c__(void);

extern void __init__PCA(void);
// void __reset__();
int reg_read(i2c_inst_t *i2c, const uint addr, const uint8_t reg, uint8_t *buf,
             const uint8_t nbytes);

int reg_write(i2c_inst_t *i2c, const uint addr, const uint8_t reg, uint8_t *buf,
              const uint8_t nbytes);

#endif // DELTARHO_CONTROLLER_I2C_CONTROL_H
