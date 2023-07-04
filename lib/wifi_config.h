//
// Created by nikesh on 7/3/23.
//

#ifndef DELTARHO_CONTROLLER_WIFI_CONFIG_H
#define DELTARHO_CONTROLLER_WIFI_CONFIG_H

#include "lwip/ip4.h"
#include <pico/stdlib.h>

const int ip_address[] = {0, 0, 0, 0};
static const char *const ssid = "your wifi network";
static const char *password = "your wifi password";
static const ip_addr_t ip_addr = IPADDR4_INIT_BYTES(
    ip_address[0], ip_address[1], ip_address[2], ip_address[3]);

#endif // DELTARHO_CONTROLLER_WIFI_CONFIG_H
