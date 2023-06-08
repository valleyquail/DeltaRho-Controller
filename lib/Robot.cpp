//
// Created by nikesh on 5/29/23.
//

#include "Robot.h"
#include <functional>

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
// Kinematics taken from here:
// https://www.internationaljournalssrg.org/IJEEE/2019/Volume6-Issue12/IJEEE-V6I12P101.pdf

/**
 *Assumes a heading of 0 is in front front wheel of the robot and increases
 *going CW
 * @param xComponent
 * @param yComponent
 * @param rotation
 * @param speed
 */
// TODO need to figure out what the maximum RPM of the motors are so that the
// directions and speeds can be converted into actual units
void Robot::controlRobot(float xComponent, float yComponent, float rotation,
                         int speed) {

  double theta = atan2(yComponent, xComponent);
  double velocity = sqrt(xComponent * xComponent + yComponent * yComponent) *
                    speedScalingFactor;
  if (velocity > 100)
    velocity = 100;
  float backRightSpeed = velocity * cos(theta + 2 * M_PI / 3) - rotation;
  float backLeftSpeed = velocity * cos(theta - 2 * M_PI / 3) - rotation;
  float frontSpeed = velocity * cos(theta) - rotation;

  backRight.setMotorMovement(backRightSpeed);
  backLeft.setMotorMovement(backLeftSpeed);
  front.setMotorMovement(frontSpeed);
  printf("Should be setting the speed\n");
}
