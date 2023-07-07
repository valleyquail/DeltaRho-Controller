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

/**
 * Data structure that will be sent to the robot controller
 * speed - Speed at which the robot should travel
 * theta - The direction the robot should head relative to an absolute heading.
 *         Ideally this is set with a magnetometer to easily get an absolute
 *         heading
 * rotation - Angular rotation of the robot in rad/s
 * distance -  The distance the robot should move. This is used to prevent MQTT
 *             task notification from forcing the robot to process the next mqtt
 *             command if the current instruction has not finished executing
 */
typedef struct mqttPacket {
  float speed;
  float theta;
  float rotation;
  float distance;
  // add more parameters later depending on what needs to be parsed
} mqttPacket;

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
