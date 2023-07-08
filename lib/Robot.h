//
// Created by nikesh on 5/29/23.
//

#ifndef DELTARHO_CONTROLLER_ROBOT_H
#define DELTARHO_CONTROLLER_ROBOT_H
extern "C" {
#include "FreeRTOS.h"
};
#include "DCMotor.h"
#include <math.h>

class Robot {

private:
  const uint8_t robotNum;

  // In cm
  const float wheelRadius = 5;
  const float chassisRadius = 10;
  // Ticks per wheel rotation
  uint16_t encoderTicksPerRotation = 100;
  //  Positional Data
  float currXDist = 0;
  float currYDistance = 0;
  double currHeading = 0;
  volatile float currDistMovedThisInstruction = 0;

public:
  explicit Robot(uint8_t robotNum);
  static void init();
  /**
   *
   * @param speed integer between 0 and 100 representing the speed of the robot
   * in a percent
   * @param direction represents theta, the angle in which to travel relative to
   * the front wheel of the robot
   * @param omega angular rotation speed in rad/sec, or at least will be one
   * day once speed is characterized
   */
  void controlRobot(float speed, float direction, float omega,
                    float distance) const;

  /**
   * Called periodically allow the PID control to work on the motors
   */
  void update();

  float getCurrDistMovedThisInstruction();

  uint8_t getRobotNum();
  void stopRobot();
};

#endif // DELTARHO_CONTROLLER_ROBOT_H
