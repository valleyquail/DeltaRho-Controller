//
// Created by nikesh on 5/29/23.
//

#ifndef DELTARHO_CONTROLLER_ROBOT_H
#define DELTARHO_CONTROLLER_ROBOT_H
#include "DC_Motor.h"

class Robot {
private:
  DC_Motor backRight;
  DC_Motor backLeft;
  DC_Motor front;
  DC_Motor extra;

public:
  Robot(void);
};

#endif // DELTARHO_CONTROLLER_ROBOT_H
