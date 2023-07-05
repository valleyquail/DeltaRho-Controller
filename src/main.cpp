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
#include "pico/stdlib.h"
#include "wifi_config.h"
}

int main() {
  stdio_init_all();
  __init__i2c__();
  __init__PCA__();
  
  sleep_ms(3000);
  if (cyw43_arch_init()) {
    printf("Wi-Fi init failed");
    return -1;
  }
  printf("Initialized the wifi");
  cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
  if (cyw43_arch_wifi_connect_timeout_ms(ssid, password,
                                         CYW43_AUTH_WPA2_AES_PSK, 10000)) {
    printf("failed to connect\n");
    return 1;
  }
  printf("connected\n");

  Robot robot = Robot(1);
  robot.init();

  sleep_ms(1000);
  initMQTT();

  //  while (true) {
  cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);

  //  }
  mqttDebug();
}
