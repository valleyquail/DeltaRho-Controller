//
// Created by nikesh on 5/29/23.
//

#include "Robot.h"
extern "C" {
#include "I2C.h"
}

Robot::Robot() {}

void Robot::init() {
  backRight.init(0, 0, 1, 2, 1);
  backLeft.init(1, 3, 4, 5, 1);
  front.init(2, 6, 7, 8, 1);
  extra.init(3, 9, 10, 11, 1);
  __init__i2c__();
  __init__PCA();
}

void Robot::controlRobot(float xComponent, float yComponent, int speed) {
  backRight.setMotorMovement(0);
  backLeft.setMotorMovement(0);
  front.setMotorMovement(0);
  printf("Should be setting the speed\n");
  sleep_ms(10);
}
