//
// Created by nikesh on 7/2/23.
//

#ifndef DELTARHO_CONTROLLER_MQTT_CONNECTION_H
#define DELTARHO_CONTROLLER_MQTT_CONNECTION_H

#include "FreeRTOS.h"
#include "lib/wifi_config.h"
#include "lwip/apps/mqtt.h"
#include "pico/lwip_freertos.h"
#include <lwip/apps/mqtt_priv.h>
// #include <pico/async_context_freertos.h>
#include <stdbool.h>
#include <string.h>

bool initMQTT();

#endif // DELTARHO_CONTROLLER_MQTT_CONNECTION_H
