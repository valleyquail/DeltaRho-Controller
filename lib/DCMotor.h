//
// Created by nikesh on 5/29/23.
//
#pragma once
#ifndef DELTARHO_CONTROLLER_DCMOTOR_H
#define DELTARHO_CONTROLLER_DCMOTOR_H

#include "hardware/timer.h"
#include "pico/stdlib.h"
#include <string>

#define BACK_RIGHT_INTERRUPT 0
#define BACK_LEFT_INTERRUPT 1
#define FRONT_INTERRUPT 2
#define EXTRA_INTERRUPT 3
#define clockRollover

void encoderIRQ(void *motorInstance);

class DCMotor {
  friend class Robot;

private:
  // PID coefficients
  float Kp;
  float Ki;
  float Kd;
  // The motor number is used for determining the proper register to write
  // to that makes the pwm signal
  int motorNum;
  // used for debugging if desired
  std::string motorName;
  uint8_t pwmPin;
  uint8_t lineOneIn;
  uint8_t lineTwoIn;

  // True if CW, False if CCW
  bool direction;
  // Target speed in ticks/s
  volatile int16_t targetSpeed;
  const int8_t maxError = 200;
  volatile int currCount;
  volatile int prevCount;
  volatile uint16_t speed;
  // Used for integral control
  int32_t sumError;

  void updatePID();

public:
  DCMotor();
  void init(int motorNumber, uint8_t pwm, uint8_t lineOne, uint8_t lineTwo,
            float kp);
  void setMotorMovement(int effort);

  void setMotorStop();
  /**
   * Calculates the rate of tick change between timer interrupts. Used to
   * set the speed of the robot since this value can be directly converted to
   * rps
   */
  friend void encoderTickRate(void *motorInstance);
  /**
   * Handles the encoders based on an XOR'd signal from the encoders and then
   * using the known direction to set the count
   *
   *
   */
  friend void encoderIRQ(void *motorInstance);
};
#endif // DELTARHO_CONTROLLER_DCMOTOR_H
