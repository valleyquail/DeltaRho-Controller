/**
 * Copyright (c) 2022 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "Robot.h"
#include "mqtt_connection.h"
extern "C" {
#include "FreeRTOS.h"
#include "multicore_management.h"
#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"
#include "wifi_config.h"
}

int main() {
  stdio_init_all();

  if (cyw43_arch_init()) {
    printf("Wi-Fi init failed");
    return -1;
  }
  printf("Initialized the wifi");

  if (cyw43_arch_wifi_connect_timeout_ms(ssid, password,
                                         CYW43_AUTH_WPA2_AES_PSK, 10000)) {
    printf("failed to connect\n");
    return 1;
  }
  printf("connected\n");
  // Allocates a FreeRTOS queue allowing for 10 mqtt data packets to be queued
  // at a time
  mqttQueue = xQueueCreate(10, sizeof(mqttPacket));
  if (mqttQueue == NULL) {
    printf("Failed it initialize the queue");
    return 1;
  }

  Robot robot = Robot(1);
  robot.init();

  robot.controlRobot(0, 0, 0, 0);
  sleep_ms(1000);
  while (true) {

    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
    sleep_ms(250);
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
    sleep_ms(250);
  }
}
