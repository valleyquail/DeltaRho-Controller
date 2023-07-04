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
#include "pico/lwip_freertos.h"
#include "wifi_config.h"
#include <lwip/apps/mqtt_priv.h>
#include <pico/async_context_freertos.h>
#include <string.h>
};

typedef struct mqttPacket {
  float xComponent;
  float yComponent;
  float rotation;
  // add more parameters later depending on what needs to be parsed
} mqttPacket;

bool initMQTT();

#endif // DELTARHO_CONTROLLER_MQTT_CONNECTION_H
