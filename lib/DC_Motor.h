//
// Created by nikesh on 5/29/23.
//

#ifndef DELTARHO_CONTROLLER_DC_MOTOR_H
#define DELTARHO_CONTROLLER_DC_MOTOR_H

#include <string>

using namespace std;
class DC_Motor {
private:
  float Kp;
  // The motor number is used for determining the proper register to write to
  // that makes the pwm signal
  int motorNum;
  // used for debugging if desired
  string motorName;
  uint8_t pwmPin;
  uint8_t lineOneIn;
  uint8_t lineTwoIn;

public:
  DC_Motor();
  void init(int motorNum, uint8_t pwm, uint8_t lineOne, uint8_t lineTwo,
            float Kp);

  void setMotorMovement(int effort);

  void setMotorStop(void);
};

#endif // DELTARHO_CONTROLLER_DC_MOTOR_H
