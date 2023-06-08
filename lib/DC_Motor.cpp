//
// Created by nikesh on 5/29/23.
//
#include "DC_Motor.h"
extern "C" {
#include "PCA9685.h"
}

DC_Motor::DC_Motor() {}

void DC_Motor::init(int motorNum, uint8_t pwm, uint8_t lineOne, uint8_t lineTwo,
                    float kp) {
  this->motorNum = motorNum;
  motorName = "Motor " + to_string(motorNum);
  pwmPin = pwm;
  lineOneIn = lineOne;
  lineTwoIn = lineTwo;
  Kp = kp;
}

/**
 * Sets the motor to a specific movement effort based on the output of the PCA
 * pwm frequency. If the effort is set to be 0, the motor is sent into a brake
 *
 * @param effort a value to be mapped to the 12bit pwm output of the PCA. CW is
 * a positive value and CCW is a negative value
 */
void DC_Motor::setMotorMovement(int effort) {
  printf("Setting the speeds for %s\n", motorName.c_str());
  // Set the driver to short stop the motor
  if (effort == 0) {
    setPWM(pwmPin, PCA_LOW);
    return;
  }
  if (effort < 0) {
    setLevel(lineOneIn, true);
    setLevel(lineTwoIn, false);
    setPWM(pwmPin, -effort * PCA_PWM_RESOLUTION / 100);
  } else {
    setLevel(lineOneIn, false);
    setLevel(lineTwoIn, true);
    setPWM(pwmPin, effort * PCA_PWM_RESOLUTION / 100);
  }
}

/**
 * This will send the motor to a standby state in order to save power when the
 * motor is idle
 */

void DC_Motor::setMotorStop() {
  setLevel(lineOneIn, false);
  setLevel(lineTwoIn, false);
  setPWM(pwmPin, PCA_HIGH);
}
