//
// Created by nikesh on 5/29/23.
//
#include "DC_Motor.h"

DC_Motor::DC_Motor() {}

void DC_Motor::init(string name, int pwm, int lineOne, int lineTwo, float kp) {
  motorName = name;
  pwmPin = pwm;
  lineOneIn = lineOne;
  lineTwoIn = lineTwo;
  Kp = kp;
}
