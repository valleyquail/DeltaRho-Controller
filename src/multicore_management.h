//
// Created by nikesh on 7/4/23.
//

#ifndef DELTARHO_CONTROLLER_MULTICORE_MANAGEMENT_H
#define DELTARHO_CONTROLLER_MULTICORE_MANAGEMENT_H

/* Kernel includes. */
#include "FreeRTOS.h" /* Must come first. */
#include "pico/stdlib.h"
#include "queue.h"  /* RTOS queue related API prototypes. */
#include "semphr.h" /* Semaphore related API prototypes. */
#include "task.h"   /* RTOS task related API prototypes. */
#include "timers.h" /* Software timer related API prototypes. */
#include <stdio.h>

/* Priorities at which the tasks are created.  The event semaphore task is
given the maximum priority of ( configMAX_PRIORITIES - 1 ) to ensure it runs as
soon as the semaphore is given. */

#define mainEVENT_SEMAPHORE_TASK_PRIORITY (configMAX_PRIORITIES - 1)
#define mainMQTT_EVENT_TASK_PRIORITY (tskIDLE_PRIORITY + 1)

/* The period of the example software timer, specified in milliseconds, and
converted to ticks using the pdMS_TO_TICKS() macro. */
#define mainSOFTWARE_TIMER_PERIOD_MS pdMS_TO_TICKS(1000)

/*-----------------------------------------------------------*/

static TaskHandle_t vMQTTConnection;
static TaskHandle_t vControlRobotViaWifi;
static TaskHandle_t xADCTask;

typedef struct mqttPacket {
  float xComponent;
  float yComponent;
  float rotation;
  // add more parameters later depending on what needs to be parsed
} mqttPacket;

void prvControlRobot(void *pvParameters);

// to be implemented later
void prvReadADC(void *pvParameters);

void prvMQTTTaskEntry(void *pvParameters);

#endif // DELTARHO_CONTROLLER_MULTICORE_MANAGEMENT_H
