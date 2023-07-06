//
// Created by nikesh on 7/4/23.
//
#include "multicore_management.h"
#include "mqtt_connection.h"
static volatile uint32_t ulCountOfItemsSentOnQueue = 0;
static volatile uint32_t ulCountOfItemsReceivedOnQueue = 0;
static volatile uint32_t ulCountOfReceivedSemaphores = 0;
static volatile uint32_t ulCountOfSDKMutexEnters = 0;
static volatile uint32_t ulCountOfSDKSemaphoreAcquires = 0;

void prvMQTTTaskEntry(void *pvParameters) {
  (void *)pvParameters;
  initMQTT();
}