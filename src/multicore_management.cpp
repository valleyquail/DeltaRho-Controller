//
// Created by nikesh on 7/4/23.
//
extern "C" {
#include "multicore_management.h"
}
#include "Robot.h"

TaskHandle_t vMQTTConnectionHandle = NULL;
TaskHandle_t vControlRobotHandle = NULL;
TaskHandle_t xADCTaskHandle = NULL;

QueueHandle_t xMQTTQueue = NULL;

struct mqttPacket *currPacket = NULL;
struct mqttPacket *prevPacket = NULL;

static volatile uint32_t ulCountOfItemsSentOnQueue = 0;
static volatile uint32_t ulCountOfItemsReceivedOnQueue = 0;
static volatile uint32_t ulCountOfReceivedSemaphores = 0;
static volatile uint32_t ulCountOfSDKMutexEnters = 0;
static volatile uint32_t ulCountOfSDKSemaphoreAcquires = 0;

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
/**
 *
 * @param pvParameters
 */
void prvMQTTTaskEntry(void *pvParameters) {
  (void *)pvParameters;
  initMQTT();
}

/*
 * Gets called via a task notify when a mqtt instruction comes in. Calls the
 * robot.stopRobot() function in order to set the motor speeds.
 */
/**
 * Controls the robot based on the packet of data sent to it
 * @param pvParameters an instance of a robot
 */
extern "C" void vControlRobot(void *pvParameters) {
  const TickType_t xMaxExpectedBlockTime = pdMS_TO_TICKS(10);
  Robot *robot = const_cast<Robot *>(static_cast<Robot *>(pvParameters));
  while (1) {
    float dist = robot->getCurrDistMovedThisInstruction();

    // Accept the task notify if it is there, but otherwise return immediately
    // so that the robot can work on PID control
    ulTaskNotifyTake(pdTRUE, 0);
    // TODO: determine acceptable movement tolerance
    if (dist >= currPacket->distance) {
      // If the task has been called via notify and the robot has finished
      // executing its current instruction, then it will take the next task from
      // the queue and begin executing it
      prevPacket = currPacket;
      BaseType_t isEmpty =
          xQueueReceive(xMQTTQueue, currPacket, xMaxExpectedBlockTime);
      // If the robot has completed its movement but has no further
      // instructions, stop the robot
      if (!isEmpty)
        robot->stopRobot();
      else
        robot->controlRobot(currPacket->speed, currPacket->theta,
                            currPacket->rotation);
    } else {
      robot->update();
    }
    // Delay the task 10ms so that it updates at about 100Hz which is more than
    // enough
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}