//
// Created by nikesh on 7/2/23.
//

#include "mqtt_connection.h"

static mqtt_client_t *static_client;
// async_context_t *asyncContext;

static void connect(mqtt_client_t *client);

bool initMqtt() {
  //  async_context_freertos_init_with_defaults(asyncContext);
  //  lwip_freertos_init(asyncContext);
}

void connect(mqtt_client_t *client) {
  struct mqtt_connect_client_info_t ci;
  err_t err;

  /* Set up an empty client info structure */
  memset(&ci, 0, sizeof(ci));

  /* Minimal amount of information required is client identifier, so set it here
   */
  ci.client_id = "lwip_test";

  /* Initiate client and connect to server, if this fails immediately an error
     code is returned otherwise mqtt_connection_cb will be called with
     connection result after attempting to establish a connection with the
     server. For now MQTT version 3.1.1 is always used */

  err = mqtt_client_connect(static_client, ip_addr, MQTT_PORT,
                            mqtt_connection_cb, 0, &ci);

  /* For now just print the result code if something goes wrong */
  if (err != ERR_OK) {
    printf("mqtt_connect return %d\n", err);
  }
}

static void mqtt_connection_cb(mqtt_client_t *client, void *arg,
                               mqtt_connection_status_t status) {
  err_t err;
  if (status == MQTT_CONNECT_ACCEPTED) {
    printf("mqtt_connection_cb: Successfully connected\n");

    /* Setup callback for incoming publish requests */
    mqtt_set_inpub_callback(client, mqtt_incoming_publish_cb,
                            mqtt_incoming_data_cb, arg);

    /* Subscribe to a topic named "subtopic" with QoS level 1, call
     * mqtt_sub_request_cb with result */
    err = mqtt_subscribe(client, "subtopic", 1, mqtt_sub_request_cb, arg);

    if (err != ERR_OK) {
      printf("mqtt_subscribe return: %d\n", err);
    }
  } else {
    printf("mqtt_connection_cb: Disconnected, reason: %d\n", status);

    /* Its more nice to be connected, so try to reconnect */
    connect(client);
  }
}

static void mqtt_sub_request_cb(void *arg, err_t result) {
  /* Just print the result code here for simplicity,
     normal behaviour would be to take some action if subscribe fails like
     notifying user, retry subscribe or disconnect from server */
  printf("Subscribe result: %d\n", result);
}