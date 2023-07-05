//
// Created by nikesh on 5/29/23.
//

#include "Robot.h"

extern "C" {
#include "I2C_Control.h"
}

Robot::Robot(uint8_t robotNum) : robotNum(robotNum) {}

void Robot::init() {
  backRight.init(0, 0, 1, 2, 1);
  backLeft.init(1, 3, 4, 5, 1);
  front.init(2, 6, 7, 8, 1);
  extra.init(3, 9, 10, 11, 1);
}
// Kinematics taken from here:
// https://www.researchgate.net/publication/228786543_Three_omni-directional_wheels_control_on_a_mobile_robot

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
void Robot::controlRobot(int speed, float direction, float rotation) {

  float backRightSpeed = speed * cos(direction + 2 * M_PI / 3) - rotation;
  float backLeftSpeed = speed * cos(direction - 2 * M_PI / 3) - rotation;
  float frontSpeed = speed * cos(direction) - rotation;

  backRight.setMotorMovement(backRightSpeed);
  backLeft.setMotorMovement(backLeftSpeed);
  front.setMotorMovement(frontSpeed);
  //  printf("Should be setting the speed\n");
}
