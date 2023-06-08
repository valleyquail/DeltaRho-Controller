/**
 * Copyright (c) 2022 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "../lib/Robot.h"
#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"

int main() {
  stdio_init_all();

  sleep_ms(1000);
  if (cyw43_arch_init()) {
    printf("Wi-Fi init failed");
    return -1;
  }
  Robot robot = Robot();
  robot.init();

  robot.controlRobot(0, 0, 0);
  sleep_ms(1000);
  while (true) {

    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
    sleep_ms(250);
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
    sleep_ms(250);
  }
}