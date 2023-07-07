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
#define BACK_LEFT_INTERRUPT 0
#define FRONT_INTERRUPT 0
#define EXTRA_INTERRUPT 0

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
  uint32_t prevTime;
  const int8_t maxError = 200;
  volatile int currCount;
  volatile int prevCount;

  // Used for integral control
  int32_t sumError;

  void updatePID();
  /**
   * Calculates the rate of tick change between calls of the function. Used to
   * set the speed of the robot since this value can be directly converted to
   * rps
   * @return the speed of the wheel in ticks/second
   */
  inline int16_t encoderTickRate() {
    // Time since last call
    uint32_t currTime = time_us_32();
    uint32_t deltaTime = currTime - prevTime;
    prevTime = currTime;
    // Absolute value of the change in encoder count since last call
    uint16_t deltaEncoder = currCount - prevCount;
    prevCount = currCount;
    // Calculates the speed as an unsigned value since the motor already knows
    // the direction
    int16_t speed = deltaEncoder * 1000000 / deltaTime;
    return speed;
  }

public:
  DCMotor();
  void init(int motorNum, uint8_t pwm, uint8_t lineOne, uint8_t lineTwo,
            float kp);
  void setMotorMovement(int effort);

  void setMotorStop(void);
  /**
   * Handles the encoders based on an XOR'd signal from the encoders and then
   * using the known direction to set the count
   */
  void encoderIRQ();
};

#endif // DELTARHO_CONTROLLER_DCMOTOR_H
