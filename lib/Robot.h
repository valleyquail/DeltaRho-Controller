//
// Created by nikesh on 5/29/23.
//

#ifndef DELTARHO_CONTROLLER_ROBOT_H
#define DELTARHO_CONTROLLER_ROBOT_H
#include "DC_Motor.h"
#include <math.h>

constexpr float speedScalingFactor = 1;

class Robot {
  friend DCMotor;

private:
  DCMotor backRight;
  DCMotor backLeft;
  DCMotor front;
  DCMotor extra;

  // In cm
  const float wheelRadius = 5;
  const float chassisRadius = 10;

public:
  Robot(uint8_t robotNum);
  void init();
  void controlRobot(float xComponent, float yComponent, float rotation,
                    int speed);
  const uint8_t robotNum;
};

#endif // DELTARHO_CONTROLLER_ROBOT_H
