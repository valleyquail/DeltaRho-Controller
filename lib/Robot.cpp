//
// Created by nikesh on 5/29/23.
//

#include "Robot.h"

extern "C" {
#include "I2C_Control.h"
}

#define BACK_RIGHT_INTERRUPT 0
#define BACK_LEFT_INTERRUPT 0
#define FRONT_INTERRUPT 0
#define EXTRA_INTERRUPT 0

Robot::Robot(uint8_t robotNum) : robotNum(robotNum) {}

void Robot::init() {
  backRight.init(0, 0, 1, 2, 1, BACK_RIGHT_INTERRUPT);
  backLeft.init(1, 3, 4, 5, 1, BACK_LEFT_INTERRUPT);
  front.init(2, 6, 7, 8, 1, FRONT_INTERRUPT);
  extra.init(3, 9, 10, 11, 1, EXTRA_INTERRUPT);
}
// Kinematics taken from here:
// https://www.researchgate.net/publication/228786543_Three_omni-directional_wheels_control_on_a_mobile_robot

// TODO need to figure out what the maximum RPM of the motors are so that the
// directions and speeds can be converted into actual units
/**
 * Controls the robot speeds. Enters a critical section so that this process is
 * not interrupted while setting the motor speeds. Otherwise there is a risk of
 * a task switch while the I2C communication is used
 * @param speed desired speed of the robot in cm/s
 * @param direction the direction of the wheel in terms of angle relative to
 * absolute heading in radians
 * @param rotation angular rotation of the robot in rad/s
 */
void Robot::controlRobot(float speed, float direction, float rotation) {
  vTaskEnterCritical();
  // Convert speed into ticks/s
  speed = (speed / wheelRadius) * encoderTicksPerRotation;
  float backRightSpeed = speed * cos(direction + 2 * M_PI / 3.) - rotation;
  float backLeftSpeed = speed * cos(direction - 2 * M_PI / 3.) - rotation;
  float frontSpeed = speed * cos(direction) - rotation;

  backRight.setMotorMovement(floor(backRightSpeed));
  backLeft.setMotorMovement(floor(backLeftSpeed));
  front.setMotorMovement(floor(frontSpeed));
  backRight.sumError = 0;
  backLeft.sumError = 0;
  front.sumError = 0;
  vTaskExitCritical();
  //  printf("Should be setting the speed\n");
}

void Robot::stopRobot() {
  backRight.setMotorMovement(0);
  backLeft.setMotorMovement(0);
  front.setMotorMovement(0);
}

void Robot::update() {
  backRight.updatePID();
  backLeft.updatePID();
  front.updatePID();
}
