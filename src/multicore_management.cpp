//
// Created by nikesh on 7/4/23.
//

#include "multicore_management.h"
#include "Robot.h"

TaskHandle_t vMQTTConnectionHandle = nullptr;
TaskHandle_t vControlRobotHandle = nullptr;
TaskHandle_t xADCTaskHandle = nullptr;

QueueHandle_t xMQTTQueue = nullptr;

struct mqttPacket *currPacket = nullptr;

// static volatile uint32_t ulCountOfItemsSentOnQueue = 0;
// static volatile uint32_t ulCountOfItemsReceivedOnQueue = 0;
// static volatile uint32_t ulCountOfReceivedSemaphores = 0;
// static volatile uint32_t ulCountOfSDKMutexEnters = 0;
// static volatile uint32_t ulCountOfSDKSemaphoreAcquires = 0;

/*
 * Note that there will be a task notify in the mqtt_incoming_data_cb function
 * so that it goes directly to controlling the robot. This allows the robot to
 * immediately respond to a wi-fi command once the mqtt callback happens rather
 * than waiting for the next time the robot control thread will execute.
 *
 * A FreeRTOS queue is used to transfer data between the tasks since it is
 * likely that in the future multiple instructions will be sent so the robot can
 * execute a sequence of movements rather than depending on unreliable timing
 * requirements from the broker
 */

void prvMQTTTaskEntry(__unused void *pvParameters) {
  bool check_mqtt_connected;
#if Debug
  printf("Going to connect to the client\n");
#endif
  connect();
  while (true) {
    printf("in mqtt task");
    ensureConnection();
  }
}

/*
 * Gets called via a task notify when a mqtt instruction comes in. Calls the
 * robot.stopRobot() function in order to set the motor speeds.
 */
/**
 * Controls the robot based on the packet of data sent to it
 * @param pvParameters an instance of a robot
 */
extern "C" [[noreturn]] void vControlRobot(void *pvParameters) {
  const TickType_t xMaxExpectedBlockTime = pdMS_TO_TICKS(10);
  auto *robot = const_cast<Robot *>(static_cast<Robot *>(pvParameters));
  while (true) {
    float dist = robot->getCurrDistMovedThisInstruction();

    // Accept the task notify if it is there, but otherwise return immediately
    // so that the robot can work on PID control
    ulTaskNotifyTake(pdTRUE, 0);
    // TODO: determine acceptable movement tolerance
    if (dist >= currPacket->distance) {
      // If the task has been called via notify and the robot has finished
      // executing its current instruction, then it will take the next task from
      // the queue and begin executing it
      BaseType_t isEmpty =
          xQueueReceive(xMQTTQueue, currPacket, xMaxExpectedBlockTime);
      // If the robot has completed its movement but has no further
      // instructions, stop the robot
      if (!isEmpty)
        robot->stopRobot();
      else
        robot->controlRobot(currPacket->speed, currPacket->theta,
                            currPacket->omega, 0);
    }
    // Delay the task 10ms so that it updates at about 100Hz which is more than
    // enough
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

void vLaunchControlRobot(void *robot) {

  // Launches the task for controlling the robot and updating the motor control
  xTaskCreate(vControlRobot, "Robot Control", 512, robot,
              mainROBOT_CONTROL_TASK_PRIORITY, &vControlRobotHandle);
  // Can only run on core one
  vTaskCoreAffinitySet(vControlRobotHandle, (UBaseType_t)1);
}

void vBlinkDebug(void *pvParameters) {
  const uint8_t ledPinOne = 100;
  const uint8_t ledPinTwo = 100;
  const uint8_t ledPinThree = 100;
  uint mask = 0 | (1 << ledPinOne) | (1 << ledPinTwo) | (1 << ledPinThree);
  gpio_init_mask(mask);
  uint8_t leds[] = {ledPinOne, ledPinTwo, ledPinThree};
  uint8_t ledIndex = 0;
  while (true) {
    gpio_put(leds[ledIndex], 0);
    ledIndex = (ledIndex + 1) % 3;
    gpio_put(leds[ledIndex], 1);
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}