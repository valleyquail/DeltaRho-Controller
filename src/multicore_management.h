//
// Created by nikesh on 7/4/23.
//

#ifndef DELTARHO_CONTROLLER_MULTICORE_MANAGEMENT_H
#define DELTARHO_CONTROLLER_MULTICORE_MANAGEMENT_H

/* Kernel includes. */
#include "FreeRTOS.h" /* Must come first. */
#include "pico/multicore.h"
#include "pico/stdlib.h"
#include "queue.h"  /* RTOS queue related API prototypes. */
#include "semphr.h" /* Semaphore related API prototypes. */
#include "task.h"   /* RTOS task related API prototypes. */
#include "timers.h" /* Software timer related API prototypes. */
#include <stdio.h>

#ifndef mainRUN_FREE_RTOS_ON_CORE
#define mainRUN_FREE_RTOS_ON_CORE 0
#endif

/* Priorities at which the tasks are created.  The event semaphore task is
given the maximum priority of ( configMAX_PRIORITIES - 1 ) to ensure it runs as
soon as the semaphore is given. */

#define mainSDK_MUTEX_USE_TASK_PRIORITY (tskIDLE_PRIORITY + 3)
#define mainSDK_SEMAPHORE_USE_TASK_PRIORITY (tskIDLE_PRIORITY + 2)
#define mainQUEUE_RECEIVE_TASK_PRIORITY (tskIDLE_PRIORITY + 2)
#define mainQUEUE_SEND_TASK_PRIORITY (tskIDLE_PRIORITY + 1)
#define mainEVENT_SEMAPHORE_TASK_PRIORITY (configMAX_PRIORITIES - 1)

/* The rate at which data is sent to the queue, specified in milliseconds, and
converted to ticks using the pdMS_TO_TICKS() macro. */
#define mainQUEUE_SEND_PERIOD_MS pdMS_TO_TICKS(200)

/* The period of the example software timer, specified in milliseconds, and
converted to ticks using the pdMS_TO_TICKS() macro. */
#define mainSOFTWARE_TIMER_PERIOD_MS pdMS_TO_TICKS(1000)

/* The number of items the queue can hold.  This is 1 as the receive task
has a higher priority than the send task, so will remove items as they are
added, meaning the send task should always find the queue empty. */
#define mainQUEUE_LENGTH (1)

/*-----------------------------------------------------------*/

#if configNUM_CORES > 1
#error Require only one core configured for FreeRTOS
#endif

// TaskHandle_t xControlRobotViaWifi;
// TaskHandle_t xADCTask;

auto_init_mutex(xSDKMutex);
static semaphore_t xSDKSemaphore;

typedef struct mqttPacket {
  float xComponent;
  float yComponent;
  float rotation;
  // add more parameters later depending on what needs to be parsed
} mqttPacket;

static QueueHandle_t mqttQueue = NULL;

static void prvLaunchRTOS();

static int initQueue();

static void prvQueueSendTask(void *pvParameters);
// static void prvQueueReceiveTask(void *pvParameters);

static void prvControlRobot(void *pvParameters);

// to be implemented later
static void prvReadADCISR(void *pvParameters);

static void mqttTaskEntry(void *pvParameters);

#endif // DELTARHO_CONTROLLER_MULTICORE_MANAGEMENT_H
