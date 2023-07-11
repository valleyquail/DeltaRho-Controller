//
// Created by nikesh on 7/3/23.
//

#ifndef DELTARHO_CONTROLLER_WIFI_CONFIG_H
#define DELTARHO_CONTROLLER_WIFI_CONFIG_H

#include "lwip/ip4.h"
#include "lwip/ip4_addr.h"
#include <pico/stdlib.h>

// mosquitto_pub -h 192.168.86.32 -t instruc -m "iF9T2D7" -u nik -P temp

/**
 * Sets the IP address of the MQTT Broker to connect to. Place the IP Address
 * segments in each of the spots inf the IPADDR4_INIT_BYTES macro below
 */
#define mqtt_broker_ip_address = IPADDR4_INIT_BYTES(192, 168, 86, 32)
static const ip4_addr_t mqtt_ip_addr mqtt_broker_ip_address;
/**
 * The SSID and Password of the wifi network to connect to
 */
static const char *SSID = "Pudgie Budgie";
static const char *PASSWORD = "<00k1381rd";

// The number of packets that the FreeRTOS Queue can hold
#define DATA_QUEUE_SIZE 10

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
typedef struct dataPacket {
  float speed;
  float theta;
  float omega;
  float distance;
  float phi;
  // add more parameters later depending on what needs to be parsed
} dataPacket;

#endif // DELTARHO_CONTROLLER_WIFI_CONFIG_H
