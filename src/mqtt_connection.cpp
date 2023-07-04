//
// Created by nikesh on 7/2/23.
//

#include "mqtt_connection.h"
static mqtt_client_t *client = NULL;

static const struct mqtt_connect_client_info_t clientInfo = {
    "test",
    "nik",  /* user */
    "temp", /* pass */
    100,    /* keep alive */
    NULL,   /* will_topic */
    NULL,   /* will_msg */
    0,      /* will_qos */
    0       /* will_retain */
#if LWIP_ALTCP && LWIP_ALTCP_TLS
    ,
    NULL
#endif
};
async_context_freertos_t *asyncContext;

static void connect(mqtt_client_t *client);

bool initMqtt() {
  client = mqtt_client_new();
  if (client == NULL) {
    printf("Could not initialize the mqtt client");
    return 1;
  }
  async_context_freertos_init_with_defaults(asyncContext);
  lwip_freertos_init(reinterpret_cast<async_context_t *>(asyncContext));
  connect(client);
  return true;
}
/* The idea is to demultiplex topic and create some reference to be used in data
   callbacks Example here uses a global variable, better would be to use a
   member in arg If RAM and CPU budget allows it, the easiest implementation
   might be to just take a copy of the topic string and use it in
   mqtt_incoming_data_cb
*/
static int inpub_id;

static void mqtt_incoming_publish_cb(void *arg, const char *topic,
                                     u32_t tot_len) {
  printf("Incoming publish at topic %s with total length %u\n", topic,
         (unsigned int)tot_len);

  /* Decode topic string into a user defined reference */
  if (strcmp(topic, "print_payload") == 0) {
    inpub_id = 0;
  } else if (topic[0] == 'A') {
    /* All topics starting with 'A' might be handled at the same way */
    inpub_id = 1;
  } else {
    /* For all other topics */
    inpub_id = 2;
  }
}

static void mqtt_incoming_data_cb(void *arg, const u8_t *data, u16_t len,
                                  u8_t flags) {
  printf("Incoming publish payload with length %d, flags %u\n", len,
         (unsigned int)flags);

  if (flags & MQTT_DATA_FLAG_LAST) {
    /* Last fragment of payload received (or whole part if payload fits receive
       buffer See MQTT_VAR_HEADER_BUFFER_LEN)  */

    /* Call function or do action depending on reference, in this case inpub_id
     */
    if (inpub_id == 0) {
      /* Don't trust the publisher, check zero termination */
      if (data[len - 1] == 0) {
        printf("mqtt_incoming_data_cb: %s\n", (const char *)data);
      }
    } else if (inpub_id == 1) {
      /* Call an 'A' function... */
    } else {
      printf("mqtt_incoming_data_cb: Ignoring payload...\n");
    }
  } else {
    /* Handle fragmented payload, store in buffer, write to file or whatever */
  }
}

static void mqtt_sub_request_cb(void *arg, err_t result) {
  /* Just print the result code here for simplicity,
     normal behaviour would be to take some action if subscribe fails like
     notifying user, retry subscribe or disconnect from server */
  printf("Subscribe result: %d\n", result);
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

/* Called when publish is complete either with sucess or failure */
static void mqtt_pub_request_cb(void *arg, err_t result) {
  if (result != ERR_OK) {
    printf("Publish result: %d\n", result);
  }
}

void publish(mqtt_client_t *client, void *arg) {
  const char *pub_payload = "PubSubHubLubJub";
  err_t err;
  u8_t qos = 2;    /* 0 1 or 2, see MQTT specification */
  u8_t retain = 0; /* No don't retain such crappy payload... */
  err = mqtt_publish(client, "pub_topic", pub_payload, strlen(pub_payload), qos,
                     retain, mqtt_pub_request_cb, arg);
  if (err != ERR_OK) {
    printf("Publish err: %d\n", err);
  }
}

void connect(mqtt_client_t *client) {
  err_t err;
  /* Initiate client and connect to server, if this fails immediately an error
     code is returned otherwise mqtt_connection_cb will be called with
     connection result after attempting to establish a connection with the
     server. For now MQTT version 3.1.1 is always used */

  err = mqtt_client_connect(client, &ip_addr, MQTT_PORT, mqtt_connection_cb, 0,
                            &clientInfo);

  /* For now just print the result code if something goes wrong */
  if (err != ERR_OK) {
    printf("mqtt_connect return %d\n", err);
  }
}