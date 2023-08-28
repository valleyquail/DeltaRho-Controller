//
// Created by nikesh on 5/29/23.
//
#include "DCMotor.h"

extern "C" {
#include "Hardware_Interface/PCA9685.h"
}

constexpr float speedScalingFactor = PCA_PWM_RESOLUTION / 100.0;

DCMotor::DCMotor() {}

/**
 * Initializes each of the DC motors on the robot
 * @param motorNumber id of the motor
 * @param pwm the pin on the PCA that sets the register associated with setting
 * the PWM signal for the motor
 * @param lineOne one of the two pins on the PCA that determines the direction
 * of the motor
 * @param lineTwo one of the two pins on the PCA that determines the direction
 * of the motor
 * @param kp used for PID to set the motor speed
 */
void DCMotor::init(int motorNumber, uint8_t pwm, uint8_t lineOne,
                   uint8_t lineTwo, float kp) {
  motorNum = motorNumber;
  motorName = "Motor " + std::to_string(motorNumber);
  pwmPin = pwm;
  lineOneIn = lineOne;
  lineTwoIn = lineTwo;
  Kp = kp;
}

/**
 * Sets the motor to a specific movement effort based on the output of the PCA
 * pwm frequency. If the speed is set to be 0, the motor is sent into a brake
 *
 * @param speed an integer value in ticks/second to be mapped to the 12bit pwm
 * output of the PCA. CW is a positive value and CCW is a negative value
 */
void DCMotor::setMotorMovement(int speed) {
#ifdef Debug
  printf("Setting the speeds for %s\n", motorName.c_str());
#endif
  // Set the driver to short stop the motor
  targetSpeed = speed;
  if (speed == 0) {
    set_PWM(pwmPin, PCA_LOW);
    return;
  }
  if (speed < 0) { // CW
    direction = true;
    set_Level(lineOneIn, true);
    set_Level(lineTwoIn, false);
    set_PWM(pwmPin, -speed * speedScalingFactor);
  } else { // CCW
    direction = false;
    set_Level(lineOneIn, false);
    set_Level(lineTwoIn, true);
    set_PWM(pwmPin, speed * speedScalingFactor);
  }
}

/**
 * This will send the motor to a standby state in order to save power when the
 * motor is idle
 */

void DCMotor::setMotorStop() {
  set_Level(lineOneIn, false);
  set_Level(lineTwoIn, false);
  set_PWM(pwmPin, PCA_HIGH);
}

void DCMotor::updatePID() {
  int16_t error = targetSpeed - speed;
  if (sumError < maxError)
    sumError += error;
  int16_t newPWM =
      (targetSpeed + Kp * error - Ki * sumError) * speedScalingFactor;
  set_PWM(pwmPin, newPWM);
}

void encoderTickRate(void *motorInstance) {
  auto *motor = static_cast<DCMotor *>(motorInstance);
  // Absolute value of the change in encoder count since last call
  uint16_t deltaEncoder = abs(motor->currCount - motor->prevCount);
  motor->prevCount = motor->currCount;
  // Calculates the speed as an unsigned value since the motor already knows
  // the direction
  motor->speed = deltaEncoder;
}
/**
 * Handles the encoders based on an XOR'd signal from the encoders and then
 * using the known direction to set the count
 */
void encoderIRQ(void *motorInstance) {
  auto *motor = static_cast<DCMotor *>(motorInstance);
  if (motor->direction)
    motor->currCount++;
  else
    motor->currCount--;
}