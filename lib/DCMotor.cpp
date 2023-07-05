//
// Created by nikesh on 5/29/23.
//
#include "DCMotor.h"
extern "C" {
#include "PCA9685.h"
}

constexpr float speedScalingFactor = PCA_PWM_RESOLUTION / 100;

DCMotor::DCMotor() {}

void DCMotor::init(int motorNum, uint8_t pwm, uint8_t lineOne, uint8_t lineTwo,
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
void DCMotor::setMotorMovement(int speed) {
  printf("Setting the speeds for %s\n", motorName.c_str());
  // Set the driver to short stop the motor
  if (speed == 0) {
    setPWM(pwmPin, PCA_LOW);
    return;
  }
  if (speed < 0) {
    setLevel(lineOneIn, true);
    setLevel(lineTwoIn, false);
    setPWM(pwmPin, -speed * speedScalingFactor);
  } else {
    setLevel(lineOneIn, false);
    setLevel(lineTwoIn, true);
    setPWM(pwmPin, speed * speedScalingFactor);
  }
}

/**
 * This will send the motor to a standby state in order to save power when the
 * motor is idle
 */

void DCMotor::setMotorStop() {
  setLevel(lineOneIn, false);
  setLevel(lineTwoIn, false);
  setPWM(pwmPin, PCA_HIGH);
}
