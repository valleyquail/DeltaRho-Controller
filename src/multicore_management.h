//
// Created by nikesh on 7/4/23.
//

#ifndef DELTARHO_CONTROLLER_MULTICORE_MANAGEMENT_H
#define DELTARHO_CONTROLLER_MULTICORE_MANAGEMENT_H

/* Kernel includes. */
#include "FreeRTOS.h" /* Must come first. */
#include "mqtt_connection.h"
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
#define mainROBOT_CONTROL_TASK_PRIORITY (tskIDLE_PRIORITY + 2)

/* The period of the example software timer, specified in milliseconds, and
converted to ticks using the pdMS_TO_TICKS() macro. */
#define mainSOFTWARE_TIMER_PERIOD_MS pdMS_TO_TICKS(1000)

/*-----------------------------------------------------------*/

extern TaskHandle_t vMQTTConnectionHandle;
extern TaskHandle_t vControlRobotHandle;
extern TaskHandle_t xADCTaskHandle;

// Queue is initialized in __main__task. The number of packets is defined below
#define MQTT_QUEUE_SIZE 10
extern QueueHandle_t xMQTTQueue;

extern struct mqttPacket *currPacket;
extern struct mqttPacket *prevPacket;

void vControlRobot(void *pvParameters);

// to be implemented later
void vReadADC(void *pvParameters);

void prvMQTTTaskEntry(void *pvParameters);

#endif // DELTARHO_CONTROLLER_MULTICORE_MANAGEMENT_H
