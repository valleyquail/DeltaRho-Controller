//
// Created by nikesh on 5/29/23.
//

#ifndef DELTARHO_CONTROLLER_ROBOT_H
#define DELTARHO_CONTROLLER_ROBOT_H
#include "DCMotor.h"
#include <math.h>

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
  /**
   *
   * @param speed integer between 0 and 100 representing the speed of the robot
   * in a percent
   * @param direction represents theta, the angle in which to travel relative to
   * the front wheel of the robot
   * @param rotation angular rotation speed in rad/sec, or at least will be one
   * day once speed is characterized
   */
  void controlRobot(int speed, float direction, float rotation);
  const uint8_t robotNum;
};

#endif // DELTARHO_CONTROLLER_ROBOT_H
