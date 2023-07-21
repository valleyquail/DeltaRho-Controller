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
const uint16_t xPort = 80;
int socket = 0;

int wifi_connect() {
  struct sockaddr_in serv_addr;
  socket = lwip_socket(AF_INET, SOCK_STREAM, 0);
  if (socket < 0) {
    printf("ERROR opening socket\n");
    return -1;
  }
  // Sets the timeout option on the socket
  int on = 10;
  lwip_setsockopt(socket, SOL_SOCKET, SO_RCVTIMEO, &on, sizeof(on));
  // Allows the port to be reused if the socket is closed
  on = 1;
  lwip_setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_len = sizeof(serv_addr);
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = PP_HTONS(xPort);
  printf("Connecting...\n");

  if (lwip_bind(socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    lwip_close(socket);
    printf("Failed to bind\n");
    return -1;
  }
  //  printf("Bound to: %s\n",
  //  ipaddr_ntoa(netif_ip4_addr(&cyw43_state.netif[0])));

  if (lwip_listen(socket, TCP_LISTEN_BACKLOG) != 0) {
    lwip_close(socket);
    printf("Failed to listen\n");
    return -1;
  }
  printf("Starting server at %s on port %u\n",
         ip4addr_ntoa(netif_ip4_addr(netif_list)), ntohs(serv_addr.sin_port));
  return socket;
}

int handle_connection(int conn_socket) {
  char data[32];
  int done = 0;
  while (done < sizeof(data)) {
    int done_now = recv(conn_socket, data + done, sizeof(data) - done, 0);
    if (done_now <= 0)
      return done_now;
    done += done_now;

    char *end = strnstr(data, "\n", done);
    if (!end)
      continue;
    *end = 0;

    err_t err;
    dataPacket packet = staticallyReserved_mqttPackets[mqttPacketIndex];
    char *temp = strtok(data, (const char *)'\n');
    parseInstruction(temp, &packet);
    // Add to the queue but don't wait for a chance to add, just immediately
    // return
    memset(data, 0, strlen(data));
    err = xQueueSendToBack(xMQTTQueue, (void *)&packet, pdMS_TO_TICKS(10));
    // if the packet is successfully added to the queue, increment the index
    // and notify the robot controller
    if (err == pdPASS) {
      // resets the packet index since we know the queue must progress in a
      // linear fashion
      mqttPacketIndex++;
      mqttPacketIndex %= 10;
      xTaskNotifyGive(vControlRobotHandle);
    }
    break;
  }
  printf("Sent a command\n");
  return done;
}
/***
 * Get status of the socket
 * @return int <0 is error
 */
void ensureConnection() { return; }
