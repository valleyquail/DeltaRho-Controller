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

#include "../multicore_management.h"
#include "pico/cyw43_arch.h"
#include "pico/lwip_freertos.h"
#include "wifi_config.h"
#include <stdbool.h>
#include <string.h>

enum TOPIC { INSTRUCTIONS, QUEUE_FULL, ADC, MISC };
/**
 * Lists all of the topics that the robot should ever interact with and map them
 * to the TOPIC enum to make it clear
 */
extern const char *topics[];
/**
 * Use this to keep a list of topics the robot should subscribe to during
 * initialization
 */
extern const enum TOPIC topics_to_subscribe_to[];
extern dataPacket staticallyReserved_mqttPackets[];
extern uint8_t mqttPacketIndex;

#ifdef MQTT_CONNECTION

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

/**
 * Used for debugging the connection to the MQTT Server
 * @param params unused parameters needed by the function
 */
_Noreturn void mqttDebug(__unused void *params);
#endif

#ifdef SOCKET_CONNECTION
#include "lwip/sockets.h"
void handle_connection(int socket);
#endif

//______________________________________________________________________________

/// Required functions
int wifi_connect();
void ensureConnection();

/**
 * Used to publish readings from the ADC
 * @param adcReading1 reading from the first ADC on the force sensor
 * @param adcReading2 reading from the second ADC on the force sensor
 */
void publishADC(int adc_reading_one, int adc_reading_two);
void publishLocation(float x_loc, float y_loc, float theta);

void parseInstruction(char *instruction, dataPacket *packet);

#if __cplusplus
}
#endif
#endif // DELTARHO_CONTROLLER_MQTT_CONNECTION_H
