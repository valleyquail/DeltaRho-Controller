/**
 * Copyright (c) 2022 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "Robot.h"
#include "mqtt_connection.h"

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

void __main__task(__unused void *pvParams);
void vLaunch();

int main() {
  stdio_init_all();
  sleep_ms(1000);
  for (int i = 0; i < 100; ++i) {
    printf("Hello %i\n", i);
    sleep_ms(50);
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
  Robot robot = Robot(1);
  robot.init();
  //  robot.controlRobot(50, 0, 0);
  sleep_ms(3000);
  printf("Trying to connect to the wifi\n");
  printf("Shouldn't be initialized yet: %i\n",
         cyw43_is_initialized(&cyw43_state));
  int error = cyw43_arch_init_with_country(CYW43_COUNTRY_USA);
  printf("Should be initialized : %i\n", cyw43_is_initialized(&cyw43_state));
  if (error) {
    printf("Wi-Fi init failed --> %i\n", error);
    robot.controlRobot(0, 0, 0);
    return;
  }
  printf("Initialized the wifi\n");

  cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
  //  robot.controlRobot(50, 0, 0);
  if (cyw43_arch_wifi_connect_timeout_ms(ssid, password,
                                         CYW43_AUTH_WPA2_AES_PSK, 10000)) {
    printf("failed to connect\n");
    robot.controlRobot(0, 0, 0);
    return;
  }
  printf("connected\n");

  sleep_ms(1000);
  initMQTT();

  //  while (true) {
  cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);

  //  }
  mqttDebug();
}

void vLaunch(void) {
  TaskHandle_t task;
  xTaskCreate(__main__task, "TestMainThread", configMINIMAL_STACK_SIZE, NULL,
              tskIDLE_PRIORITY, &task);
  /* Start the tasks and timer running. */
  vTaskStartScheduler();
}
