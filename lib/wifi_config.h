//
// Created by nikesh on 7/3/23.
//

#ifndef DELTARHO_CONTROLLER_WIFI_CONFIG_H
#define DELTARHO_CONTROLLER_WIFI_CONFIG_H

#include "lwip/ip4.h"
#include "lwip/ip4_addr.h"
#include <pico/stdlib.h>

/**
 * Sets the IP address of the MQTT Broker to connect to. Place the IP Address
 * segments in each of the spots inf the IPADDR4_INIT_BYTES macro below
 */
#define mqtt_broker_ip_address = IPADDR4_INIT_BYTES(0, 0, 0, 0)
static const ip4_addr_t ip_addr mqtt_broker_ip_address;
/**
 * The SSID and Password of the wifi network to connect to
 */
static const char *SSID = "your ssid";
static const char *PASSWORD = "your password";

#endif // DELTARHO_CONTROLLER_WIFI_CONFIG_H
