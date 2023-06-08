//
// Created by nikesh on 5/29/23.
//

#ifndef DELTARHO_CONTROLLER_ROBOT_H
#define DELTARHO_CONTROLLER_ROBOT_H
#include "DC_Motor.h"

class Robot {
  friend DC_Motor;

private:
  DC_Motor backRight;
  DC_Motor backLeft;
  DC_Motor front;
  DC_Motor extra;

public:
  Robot(void);
  void init(void);

  void controlRobot(float xComponent, float yComponent, int speed);
};

#endif // DELTARHO_CONTROLLER_ROBOT_H
