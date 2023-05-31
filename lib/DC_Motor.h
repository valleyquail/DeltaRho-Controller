//
// Created by nikesh on 5/29/23.
//

#ifndef DELTARHO_CONTROLLER_DC_MOTOR_H
#define DELTARHO_CONTROLLER_DC_MOTOR_H

#include "I2C.h"
#include <string>
// Used for setting the direction of the motor
#define CLOCKWISE 0
#define COUNTERCLOCKWISE 1

using namespace std;
class DC_Motor {
private:
  float Kp;
  int motorNum;
  string motorName;
  int pwmPin;
  int lineOneIn;
  int lineTwoIn;

public:
  DC_Motor();
  void init(int motorNum, int pwm, int lineOne, int lineTwo, float Kp);
  void setMotorMovement(int effort);
};

#endif // DELTARHO_CONTROLLER_DC_MOTOR_H
