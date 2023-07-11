//
// Created by nikesh on 5/29/23.
//

#include "Robot.h"
#include "../src/multicore_management.h"
#include "DCMotor.h"
#define ALARM_NUM 0
#define ALARM_IRQ TIMER_IRQ_0

DCMotor backRight;
DCMotor backLeft;
DCMotor front;
DCMotor extra;

Robot::Robot(uint8_t robotNum) : robotNum(robotNum) {}

// Callback function for the interrupt. This acts as an intermediate since the
// pico SDK is in C
extern "C" void back_right_gpio_callback(uint gpio, uint32_t events) {
  encoderIRQ(&backRight);
}
extern "C" void back_left_gpio_callback(uint gpio, uint32_t events) {
  encoderIRQ(&backLeft);
}
extern "C" void front_gpio_callback(uint gpio, uint32_t events) {
  encoderIRQ(&front);
}
extern "C" void alarm_in_us(uint32_t delay_us);
void Robot::init() {

  backRight.init(0, 0, 1, 2, 1);
  backLeft.init(1, 3, 4, 5, 1);
  front.init(2, 6, 7, 8, 1);
  extra.init(3, 9, 10, 11, 1);

  gpio_set_irq_enabled_with_callback(BACK_RIGHT_INTERRUPT,
                                     GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL,
                                     false, &back_right_gpio_callback);
  gpio_set_irq_enabled_with_callback(BACK_LEFT_INTERRUPT,
                                     GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL,
                                     false, &back_left_gpio_callback);
  gpio_set_irq_enabled_with_callback(FRONT_INTERRUPT,
                                     GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL,
                                     false, &front_gpio_callback);
  //  alarm_in_us(10000);
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
 * @param omega angular rotation of the robot in rad/s
 * @param distance how far the robot should move in cm
 */
void Robot::controlRobot(float speed, float direction, float omega,
                         float distance) const {
  //  vTaskEnterCritical();
  uint32_t interrupts = save_and_disable_interrupts();
  // Convert speed into ticks/s
  speed = (speed / wheelRadius) * encoderTicksPerRotation;
  float backRightSpeed = speed * cos(direction + 2 * M_PI / 3.) - omega;
  float backLeftSpeed = speed * cos(direction - 2 * M_PI / 3.) - omega;
  float frontSpeed = speed * cos(direction) - omega;

  backRight.setMotorMovement(floor(backRightSpeed));
  backLeft.setMotorMovement(floor(backLeftSpeed));
  front.setMotorMovement(floor(frontSpeed));
  backRight.sumError = 0;
  backLeft.sumError = 0;
  front.sumError = 0;
  restore_interrupts(interrupts);
  //  vTaskExitCritical();
  printf("Should be setting the speed\n");
}

void Robot::stopRobot() {
  backRight.setMotorMovement(0);
  backLeft.setMotorMovement(0);
  front.setMotorMovement(0);
}

void Robot::update() {
  //  printf("Doing some PID\n");
  return;
  backRight.updatePID();
  backLeft.updatePID();
  front.updatePID();
}

float Robot::getCurrDistMovedThisInstruction() { return 0; }

static void alarmIRQ() {
  hw_clear_bits(&timer_hw->intr, 1u << ALARM_NUM);
  // calculate the encoder tick rates
  encoderTickRate(&backRight);
  encoderTickRate(&backRight);
  encoderTickRate(&backRight);
}

extern "C" void alarm_in_us(uint32_t delay_us) {
  // Enable the interrupt for our alarm (the timer outputs 4 alarm irqs)
  hw_set_bits(&timer_hw->inte, 1u << ALARM_NUM);
  // Set irq handler for alarm irq
  irq_set_exclusive_handler(ALARM_IRQ, alarmIRQ);
  // Enable the alarm irq
  irq_set_enabled(ALARM_IRQ, true);
  // Enable interrupt in block and at processor

  // Alarm is only 32 bits so if trying to delay more
  // than that need to be careful and keep track of the upper
  // bits
  uint64_t target = timer_hw->timerawl + delay_us;

  // Write the lower 32 bits of the target time to the alarm which
  // will arm it
  timer_hw->alarm[ALARM_NUM] = (uint32_t)target;
}
