////
//// Created by nikesh on 7/4/23.
////
// #include "multicore_management.h"
//
// static void prvLaunchRTOS() {
//   printf("Core %d: Launching FreeRTOS scheduler\n", get_core_num());
//   /* Start the tasks and timer running. */
//   vTaskStartScheduler();
//   /* should never reach here */
//   panic_unsupported();
// }
//
// static int initQueue() {
//   mqttQueue = xQueueCreate(mainQUEUE_LENGTH, sizeof(mqttPacket));
//   if (mqttQueue == NULL) {
//     printf("Failed it initialize the queue");
//     return 1;
//   }
// }
//
// static void prvQueueSendTask(void *pvParameters) {
//   TickType_t xNextWakeTime;
//   const uint32_t ulValueToSend = 100UL;
//
//   /* Initialise xNextWakeTime - this only needs to be done once. */
//   xNextWakeTime = xTaskGetTickCount();
//
//   for (;;) {
//     /* Place this task in the blocked state until it is time to run again.
//     The block time is specified in ticks, the constant used converts ticks
//     to ms.  The task will not consume any CPU time while it is in the
//     Blocked state. */
//     vTaskDelayUntil(&xNextWakeTime, mainQUEUE_SEND_PERIOD_MS);
//
//     /* Send to the queue - causing the queue receive task to unblock and
//     increment its counter.  0 is used as the block time so the sending
//     operation will not block - it shouldn't need to block as the queue
//     should always be empty at this point in the code. */
//     ulCountOfItemsSentOnQueue++;
//     printf("Core %d - Thread '%s': Queue send %d\n", get_core_num(),
//            pcTaskGetName(xTaskGetCurrentTaskHandle()),
//            ulCountOfItemsSentOnQueue);
//     xQueueSend(mqttQueue, &ulValueToSend, 0);
//   }
// }
///*-----------------------------------------------------------*/
//
// static void prvQueueReceiveTask(void *pvParameters) {
//  uint32_t ulReceivedValue;
//
//  for (;;) {
//    /* Wait until something arrives in the queue - this task will block
//    indefinitely provided INCLUDE_vTaskSuspend is set to 1 in
//    FreeRTOSConfig.h. */
//    xQueueReceive(mqttQueue, &ulReceivedValue, portMAX_DELAY);
//    /*  To get here something must have been received from the queue, but
//    is it the expected value?  If it is, increment the counter. */
//    if (ulReceivedValue == 100UL) {
//      /* Count the number of items that have been received correctly. */
//      ulCountOfItemsReceivedOnQueue++;
//      printf("Core %d - Thread '%s': Queue receive %d\n", get_core_num(),
//             pcTaskGetName(xTaskGetCurrentTaskHandle()),
//             ulCountOfItemsReceivedOnQueue);
//    }
//  }
//}
///*-----------------------------------------------------------*/