/**
 * Copyright (c) 2022 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "Robot.h"
extern "C" {
#include "FreeRTOS.h"
#include "I2C_Control.h"
#include "PCA9685.h"
#include "multicore_management.h"
#include "pico/cyw43_arch.h"
#include "pico/stdio.h"
#include "pico/stdlib.h"
#include "task.h"

#include "wifi_config.h"
}
// Global instance of the robot
Robot robot = Robot(1);

void __main__task(__unused void *pvParams);
void vLaunch();

int main() {
  stdio_init_all();
  for (int i = 0; i < 100; ++i) {
    printf("Launching in %i ms\n", 100 * 20 - i);
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

void __main__task(__unused void *pvParams) {
  __init__i2c__();
  __init__PCA__();
  robot.init();
  printf("Trying to connect to the wifi\n");
  //  printf("Shouldn't be initialized yet: %i\n",
  //         cyw43_is_initialized(&cyw43_state));
  int error = cyw43_arch_init();
  //  printf("Should be initialized : %i\n",
  //  cyw43_is_initialized(&cyw43_state));
  if (error) {
    printf("Wi-Fi init failed --> %i\n", error);
    robot.controlRobot(0, 0, 0);
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
  printf("connected\n");
  sleep_ms(1000);

  cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);

  xMQTTQueue = xQueueCreate(MQTT_QUEUE_SIZE, sizeof(mqttPacket *));
  // Launches the MQTT connection. Configures 1kb of stack words since Wi-Fi
  // communication is likely going to be parsing/encoding a lot of data
  xTaskCreate(prvMQTTTaskEntry, "MQTT Connection", 1024, NULL,
              mainMQTT_EVENT_TASK_PRIORITY, &vMQTTConnectionHandle);
  // Launches the task for controlling the robot and updating the motor control
  xTaskCreate(vControlRobot, "Robot Control", 512, 0,
              mainROBOT_CONTROL_TASK_PRIORITY, &vControlRobotHandle);
  // Deletes this task
  vTaskDelete(NULL);
}

void vLaunch(void) {
  // Ignore making a task handle for the main task since it is just used to
  // initialize the Wi-Fi connection and then will delete itself after
  // scheduling the remaining tasks
  xTaskCreate(__main__task, "TestMainThread", configMINIMAL_STACK_SIZE, NULL,
              tskIDLE_PRIORITY, NULL);
  /* Start the tasks and timer running. */
  vTaskStartScheduler();
  // Ideally we should never reach this point
  panic_unsupported();
}
