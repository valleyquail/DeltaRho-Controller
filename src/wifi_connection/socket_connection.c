//
// Created by nikesh on 7/10/23.
//
#include "FreeRTOS.h"
#include "connection.h"
#include "lwip/sockets.h"
#include "pico/cyw43_arch.h"
#include "queue.h"
#include "task.h"

// Port to connect to
uint16_t xPort = 80;

int wifi_connect() {
  struct sockaddr_in serv_addr;
  int xSock = lwip_socket(AF_INET, SOCK_STREAM, 0);
  if (xSock < 0) {
    printf("ERROR opening socket\n");
    return -1;
  }
  int on = 1;
  lwip_setsockopt(xSock, SOL_SOCKET, SO_KEEPALIVE, &on, sizeof(on));

  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_len = sizeof(serv_addr);
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = PP_HTONS(xPort);
  printf("Connecting...\n");

  if (lwip_bind(xSock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    lwip_close(xSock);
    printf("Failed to bind\n");
    return -1;
  }
  printf("Bound to: %s\n", ipaddr_ntoa(netif_ip4_addr(&cyw43_state.netif[0])));

  if (lwip_listen(xSock, TCP_LISTEN_BACKLOG) != 0) {
    lwip_close(xSock);
    printf("Failed to listen\n");
    return -1;
  }
  printf("Starting server at %s on port %u\n",
         ip4addr_ntoa(netif_ip4_addr(netif_list)), ntohs(serv_addr.sin_port));
  return xSock;
}

/***
 * Get status of the socket
 * @return int <0 is error
 */
void ensureConnection() {
  int error = 0;
  socklen_t len = sizeof(error);
  //  int retval = getsockopt(, SOL_SOCKET, SO_ERROR, &error, &len);
  //  return error;
}
