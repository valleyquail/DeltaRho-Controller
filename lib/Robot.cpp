//
// Created by nikesh on 5/29/23.
//

#include "Robot.h"

Robot::Robot() {
  backRight.init("Motor 1", 0, 1, 2, 1);
  backLeft.init("Motor 2", 3, 4, 5, 1);
  front.init("Motor 3", 6, 7, 8, 1);
  extra.init("Motor 4", 9, 10, 11, 1);
}

void Robot::controlRobot(float xComponent, float yComponent, int speed) {}
