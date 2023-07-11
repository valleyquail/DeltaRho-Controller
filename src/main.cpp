/**
 * Copyright (c) 2022 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "FreeRTOS.h"
#include "Hardware_Interface/I2C_Control.h"
#include "Hardware_Interface/PCA9685.h"
#include "Robot.h"
#include "Robot_Config.h"
#include "multicore_management.h"
#include "pico/cyw43_arch.h"
#include "pico/stdio.h"
#include "task.h"
#include "wifi_connection/connection.h"
#include "wifi_connection/wifi_config.h"
#include <pico/async_context_freertos.h>

// Global instance of the robot
Robot robot = Robot(ROBOT_NUMBER);

void main_task(__unused void *pvParams);
void vLaunch();
//// Used to raise the priority of the cyw43 async context task so that it runs
//// more often to parse instructions
void configure_cyw43();

int main() {
  stdio_init_all();
  sleep_ms(1000);
  for (int i = 0; i < 100; ++i) {
    printf("Launching in %i ms\n", (100 - i) * 20);
    sleep_ms(20);
  }
  const char *rtos_name;
#if (portSUPPORT_SMP == 1)
  rtos_name = "FreeRTOS SMP";
#else
  rtos_name = "FreeRTOS";
#endif

#if (portSUPPORT_SMP == 1) && (configNUM_CORES == 2)
  printf("Starting %s on both cores:\n", rtos_name);
  vLaunch();
#elif (RUN_FREE_RTOS_ON_CORE == 1)
  printf("Starting %s on core 1:\n", rtos_name);
  multicore_launch_core1(vLaunch);
  while (true)
    ;
#else
  printf("Starting %s on core 0:\n", rtos_name);
  vLaunch();
#endif
  return 0;
}

void main_task(__unused void *pvParams) {
  init_i2c();
  init_PCA();
  robot.init();
  printf("Trying to connect to the wifi\n");
  //  printf("Shouldn't be initialized yet: %i\n",
  //         cyw43_is_initialized(&cyw43_state));
  int error = cyw43_arch_init();
  //  printf("Should be initialized : %i\n",
  //  cyw43_is_initialized(&cyw43_state));
  if (error) {
    printf("Wi-Fi init failed --> %i\n", error);
    robot.controlRobot(0, 0, 0, 0);
    return;
  }
  printf("Initialized the wifi\n");

  cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
  cyw43_arch_enable_sta_mode();
  if (cyw43_arch_wifi_connect_timeout_ms(SSID, PASSWORD,
                                         CYW43_AUTH_WPA2_AES_PSK, 10000)) {
    printf("failed to connect\n");
    return;
  }
  printf("socket\n");
  printf("This is the IP address of the board: %s\n",
         ipaddr_ntoa(netif_ip4_addr(&cyw43_state.netif[0])));
  configure_cyw43();

  cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);

  xMQTTQueue = xQueueCreate(DATA_QUEUE_SIZE, sizeof(dataPacket *));

  // Launches the Wi-Fi connection. Configures 1kb of stack words since Wi-Fi
  // communication is likely going to be parsing/encoding a lot of data
  printf("Going to connect to the client\n");
  int socket = wifi_connect();
  if (socket >= 0)
    printf("Connected successfully\n");
  else {
    printf("Failed to connect, aborting: socket = %i\n", socket);
    panic_unsupported();
  }

  xTaskCreate(prvWifiTaskEntry, "Wifi Connection", 1024, (void *)socket,
              mainWifi_EVENT_TASK_PRIORITY, &vWifiConnectionHandle);

  xTaskCreate(vBlinkDebug, "Blink Debug", configMINIMAL_STACK_SIZE, nullptr,
              mainBLINK_DEBUG_TASK, nullptr);
  // Launches the robot controller
  vLaunchControlRobot(&robot);
  // Deletes this task
  vTaskDelete(nullptr);
}

void vLaunch() {
  // Ignore making a task handle for the main task since it is just used to
  // initialize the Wi-Fi connection and then will delete itself after
  // scheduling the remaining tasks
  TaskHandle_t mainTask;
  xTaskCreate(main_task, "TestMainThread", configMINIMAL_STACK_SIZE, nullptr,
              tskIDLE_PRIORITY, &mainTask);
  // Bind the main task to execute on core one since that is where the robot
  // controller will be running and that needs to be linked to the interrupts
  vTaskCoreAffinitySet(mainTask, 1);
  /* Start the tasks and timer running. */
  vTaskStartScheduler();
  // Ideally we should never reach this point
  printf("Something failed hard\n");
  panic_unsupported();
}

void configure_cyw43() {
  // Makes a new FreeRTOS async context configuration
  async_context_freertos_config_t asyncContextFreertosConfig =
      async_context_freertos_default_config();
  // Sets the context task priority to the user defined level
  asyncContextFreertosConfig.task_priority = mainCYW43_ASYNC_PROCESS_PRIORITY;
  // Makes a new async context
  async_context_freertos_t asyncContextFreertos;
  async_context_freertos_init(&asyncContextFreertos,
                              &asyncContextFreertosConfig);
  // Sets the cyw43 config
  cyw43_arch_set_async_context(&asyncContextFreertos.core);
  // Sets the Wi-Fi connection to performance mode
  cyw43_wifi_pm(&cyw43_state, CYW43_PERFORMANCE_PM);
}