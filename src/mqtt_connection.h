//
// Created by nikesh on 7/2/23.
//

#ifndef DELTARHO_CONTROLLER_MQTT_CONNECTION_H
#define DELTARHO_CONTROLLER_MQTT_CONNECTION_H

#include <stdbool.h>

#include "FreeRTOS.h"
#include "lwip/apps/mqtt.h"
#include "multicore_management.h"
#include "pico/cyw43_arch.h"
#include "pico/lwip_freertos.h"
#include "wifi_config.h"
#include <lwip/apps/mqtt_priv.h>
#include <string.h>

enum TOPIC { INSTRUCTIONS, ADC, MISC };
/**
 * Lists all of the topics that the robot should ever interact with and map them
 * to the TOPIC enum to make it clear
 */
static const char *topics[] = {"instruc", "adc", "misc"};
/**
 * Use this to keep a list of topics the robot should subscribe to during
 * initialization
 */
static const enum TOPIC topics_to_subscribe_to[] = {INSTRUCTIONS};

/**
 * Initialized the MQTT connection by creating a client struct and the calling
 * the connection Specifically, it handles creating the client, organizing the
 * callbacks, and subscribing to topics
 */
void initMQTT();
/**
 * Used for debugging the connection to the MQTT Server
 * @param params unused parameters needed by the function
 */
void mqttDebug(__unused void *params);

/**
 * Used to publish readings from the ADC
 * @param adcReading1 reading from the first ADC on the force sensor
 * @param adcReading2 reading from the second ADC on the force sensor
 */
void publishADC(int adcReading1, int adcReading2);
void publishLocation(float xLoc, float yLoc, float theta);

#endif // DELTARHO_CONTROLLER_MQTT_CONNECTION_H
