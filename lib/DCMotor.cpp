//
// Created by nikesh on 5/29/23.
//
#include "DCMotor.h"

extern "C" {
#include "PCA9685.h"
}

constexpr float speedScalingFactor = PCA_PWM_RESOLUTION / 100;

DCMotor::DCMotor() {}

// Callback function for the interrupt. This acts as an intermediate since the
// pico SDK is in C
// void gpio_callback(uint gpio, uint32_t events) { DCMotor::encoderIRQ(); }

/**
 * Initializes each of the DC motors on the robot
 * @param motorNum id of the motor
 * @param pwm the pin on the PCA that sets the register associated with setting
 * the PWM signal for the motor
 * @param lineOne one of the two pins on the PCA that determines the direction
 * of the motor
 * @param lineTwo one of the two pins on the PCA that determines the direction
 * of the motor
 * @param kp used for PID to set the motor speed
 * @param interruptPin pin associated with the interrupt signal from the
 * encoders
 */
void DCMotor::init(int motorNum, uint8_t pwm, uint8_t lineOne, uint8_t lineTwo,
                   float kp, uint8_t interruptPin) {
  this->motorNum = motorNum;
  motorName = "Motor " + std::to_string(motorNum);
  pwmPin = pwm;
  lineOneIn = lineOne;
  lineTwoIn = lineTwo;
  Kp = kp;
  // Enables interrupts
  gpio_set_irq_enabled_with_callback(interruptPin,
                                     GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL,
                                     false, &DCMotor::encoderIRQ);
}

/**
 * Sets the motor to a specific movement effort based on the output of the PCA
 * pwm frequency. If the speed is set to be 0, the motor is sent into a brake
 *
 * @param speed an integer value in ticks/second to be mapped to the 12bit pwm
 * output of the PCA. CW is a positive value and CCW is a negative value
 */
void DCMotor::setMotorMovement(int speed) {
  printf("Setting the speeds for %s\n", motorName.c_str());
  // Set the driver to short stop the motor
  targetSpeed = speed;
  if (speed == 0) {
    setPWM(pwmPin, PCA_LOW);
    return;
  }
  if (speed < 0) { // CW
    direction = true;
    setLevel(lineOneIn, true);
    setLevel(lineTwoIn, false);
    setPWM(pwmPin, -speed * speedScalingFactor);
  } else { // CCW
    direction = false;
    setLevel(lineOneIn, false);
    setLevel(lineTwoIn, true);
    setPWM(pwmPin, speed * speedScalingFactor);
  }
}

/**
 * This will send the motor to a standby state in order to save power when the
 * motor is idle
 */

void DCMotor::setMotorStop() {
  setLevel(lineOneIn, false);
  setLevel(lineTwoIn, false);
  setPWM(pwmPin, PCA_HIGH);
}

void DCMotor::updatePID() {
  uint16_t speed = encoderTickRate();
  int16_t error = targetSpeed - speed;
  if (sumError < maxError)
    sumError += error;
  int16_t newPWM =
      (targetSpeed + Kp * error - Ki * sumError) * speedScalingFactor;
  setPWM(pwmPin, newPWM);
}