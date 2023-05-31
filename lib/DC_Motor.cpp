//
// Created by nikesh on 5/29/23.
//
#include "DC_Motor.h"

DC_Motor::DC_Motor() {}

void DC_Motor::init(int motorNum, int pwm, int lineOne, int lineTwo, float kp) {
  this.motorNum = motorNum;
  motorName = "Motor " + motorNum;
  pwmPin = pwm;
  lineOneIn = lineOne;
  lineTwoIn = lineTwo;
  Kp = kp;
}
