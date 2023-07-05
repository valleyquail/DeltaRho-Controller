//
// Created by nikesh on 7/2/23.
//

#ifndef DELTARHO_CONTROLLER_MQTT_CONNECTION_H
#define DELTARHO_CONTROLLER_MQTT_CONNECTION_H

#include <stdbool.h>
#include <string>

extern "C" {
#include "FreeRTOS.h"
#include "lwip/apps/mqtt.h"
#include "multicore_management.h"
#include "pico/cyw43_arch.h"
#include "pico/lwip_freertos.h"
#include "wifi_config.h"
#include <lwip/apps/mqtt_priv.h>
#include <pico/async_context_freertos.h>
#include <string.h>
};

bool initMQTT();
bool mqttDebug();

#endif // DELTARHO_CONTROLLER_MQTT_CONNECTION_H
