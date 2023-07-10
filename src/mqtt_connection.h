#include <sys/cdefs.h>
//
// Created by nikesh on 7/2/23.
//

#ifndef DELTARHO_CONTROLLER_MQTT_CONNECTION_H
#define DELTARHO_CONTROLLER_MQTT_CONNECTION_H

#if __cplusplus
extern "C" {
#endif

#include "../lib/Robot_Config.h"
#include "FreeRTOS.h"
#include "lwip/apps/mqtt.h"
#include "multicore_management.h"
#include "pico/cyw43_arch.h"
#include "pico/lwip_freertos.h"
#include "wifi_config.h"
#include <lwip/apps/mqtt_priv.h>
#include <stdbool.h>
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
  float omega;
  float distance;
  float phi;
  // add more parameters later depending on what needs to be parsed
} mqttPacket;
// The number of packets that the FreeRTOS Queue can hold is defined below
#define MQTT_QUEUE_SIZE 10

enum TOPIC { INSTRUCTIONS, QUEUE_FULL, ADC, MISC };
/**
 * Lists all of the topics that the robot should ever interact with and map them
 * to the TOPIC enum to make it clear
 */
static const char *topics[] = {"instruc", "full", "adc", "misc"};
/**
 * Use this to keep a list of topics the robot should subscribe to during
 * initialization
 */
static const enum TOPIC topics_to_subscribe_to[] = {INSTRUCTIONS, QUEUE_FULL};

void connect();
void ensureConnection();

// Callbacks
//______________________________________________________________________________
static void mqtt_incoming_data_cb(__unused void *arg, const u8_t *data,
                                  u16_t len, u8_t flags);
static void mqtt_incoming_publish_cb(void *arg, const char *topic,
                                     u32_t tot_len);
static void mqtt_sub_request_cb(__unused void *arg, err_t result);

static void subscribe_to_default_topics(mqtt_client_t *client, void *arg);

static void mqtt_connection_cb(mqtt_client_t *client, void *arg,
                               mqtt_connection_status_t status);

void mqtt_pub_request_cb(__unused void *arg, err_t result);

//______________________________________________________________________________
/**
 * Used for debugging the connection to the MQTT Server
 * @param params unused parameters needed by the function
 */
_Noreturn void mqttDebug(__unused void *params);

/**
 * Used to publish readings from the ADC
 * @param adcReading1 reading from the first ADC on the force sensor
 * @param adcReading2 reading from the second ADC on the force sensor
 */
void publishADC(int adc_reading_one, int adc_reading_two);
void publishLocation(float x_loc, float y_loc, float theta);

#if __cplusplus
}
#endif

#endif // DELTARHO_CONTROLLER_MQTT_CONNECTION_H
