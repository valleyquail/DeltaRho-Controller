
//
// Created by nikesh on 7/2/23.
//
#ifdef MQTT_CONNECTION
#include "FreeRTOS.h"
#include "connection.h"
#include "lwip/apps/mqtt.h"
#include "queue.h"
#include "task.h"
#include <lwip/apps/mqtt_priv.h>

// Struct holding the mqtt broker connection
mqtt_client_t mqtt_client;

// Keeps track of which packet is the next in line to be sent to the queue. We
// can do this since we know the packet information is always going to be sent
// in order because we always send to the back of the queue and pull from the
// front of the queue
char robot_header[3] = {'R', ('0' + ROBOT_NUMBER), '/'};

static const struct mqtt_connect_client_info_t clientInfo = {
    "usertwo",
    "usertwo",
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

#if Debug
mqtt_request_cb_t pub_mqtt_request_cb_t;
char PUB_PAYLOAD[] = "this is a message from pico_w ctrl 0       ";
char PUB_PAYLOAD_SCR[] = "this is a message from pico_w ctrl 0       ";
char PUB_EXTRA_ARG[] = "test";
u16_t payload_size;
#endif

// The following are all callback functions and can be largely ignored
//_____________________________________________________________________________
/**
 * Callback for an incoming publish
 * @param arg
 * @param topic the topic that has been received
 * @param tot_len length of the topic
 */
static void mqtt_incoming_publish_cb(void *arg, const char *topic,
                                     u32_t tot_len) {
  const struct mqtt_connect_client_info_t *client_info =
      (const struct mqtt_connect_client_info_t *)arg;
#if Debug
  LWIP_PLATFORM_DIAG(("MQTT client \"%s\" publish cb: topic %s, len %d\n",
                      client_info->client_id, topic, (int)tot_len));
#endif
}
/**
 * Callback for any incoming data
 * @param arg
 * @param data the data associated with the topic
 * @param len
 * @param flags
 */
static void mqtt_incoming_data_cb(__unused void *arg, const u8_t *data,
                                  u16_t len, u8_t flags) {
  printf("Incoming publish payload with length %d, flags %u\n", len,
         (unsigned int)flags);
  printf("mqtt_incoming_data_cb: %s\n", (const char *)data);
  if (flags & MQTT_DATA_FLAG_LAST) {
    // Used for message send confirmation:
    // use "p" to indicate the packet was successfully passed and "f" to
    // indicate the packet failed to be put in the queue
    char payload[] = "p";
    // ensure this message is delivered
    uint8_t qos = 1;

    // Matches the first character of the subscribed topics
    if (data[0] == topics[INSTRUCTIONS][0]) {
      err_t err;
      dataPacket packet = staticallyReserved_mqttPackets[mqttPacketIndex];

      parseInstruction(data, &packet);
      // Add to the queue but don't wait for a chance to add, just immediately
      // return
      memset(data, 0, strlen(data));
      err = xQueueSendToBack(xMQTTQueue, (void *)&packet, 0);
      // if the packet is successfully added to the queue, increment the index
      // and notify the robot controller
      if (err == pdPASS) {
        // resets the packet index since we know the queue must progress in a
        // linear fashion
        mqttPacketIndex++;
        mqttPacketIndex %= 10;
        xTaskNotifyGive(vControlRobotHandle);
      } else {
        payload[0] = 'f';
        err = mqtt_publish(&mqtt_client, topics[QUEUE_FULL], payload,
                           strlen(payload), qos, 0, mqtt_pub_request_cb, arg);
      }

#if Debug
      if (err != ERR_OK) {
        printf("mqtt_subscribe return: %d\n", err);
      }
      //      printf("mqtt_incoming_data_cb: %s\n", (const char *)data);
    } else {
      printf("mqtt_incoming_data_cb: Ignoring payload...\n");
#endif
    }
  } else {
    // Handle fragmented payload or something else
    return;
  }
}

void mqtt_sub_request_cb(__unused void *arg, err_t result) {
  /* Just print the result code here for simplicity,
     normal behaviour would be to take some action if subscribe fails like
     notifying user, retry subscribe or disconnect from server */
#if Debug
  printf("Subscribe result: %d\n", result);
#endif
}

void subscribe_to_default_topics(mqtt_client_t *client, void *arg) {
  err_t err;
  printf("mqtt_connection_cb: Successfully connected\n");
  /* Subscribe to a topic named "subtopic" with QoS level 1, call
   * mqtt_sub_request_cb with result */
  int size = sizeof(topics_to_subscribe_to) / sizeof(enum TOPIC);
  for (int i = 0; i < size; i++) {
    enum TOPIC topic = topics_to_subscribe_to[i];
    //    char buffer[8];
    //    memcpy(buffer, robot_header, sizeof(robot_header));
    //    strcat(buffer, topics[topic]);
    err = mqtt_subscribe(client, topics[topic], 0, mqtt_sub_request_cb, arg);
    printf("Subscribed to: %s\n", topics[topic]);
#if Debug
    if (err != ERR_OK) {
      printf("mqtt_subscribe return: %d\n", err);
    }
#endif
  }
  printf("Done subscribing\n");
}

void mqtt_connection_cb(mqtt_client_t *client, void *arg,
                        mqtt_connection_status_t status) {
  if (status == MQTT_CONNECT_ACCEPTED) {
    //    subscribe_to_default_topics(client, arg);
    printf("did a thing\n");
  } else {
#if Debug
    printf("mqtt_connection_cb: Disconnected, reason: %d\n", status);
#endif
    /* Its more nice to be connected, so try to reconnect */
    connect();
  }
}

/* Called when publish is complete either with success or failure */
void mqtt_pub_request_cb(__unused void *arg, err_t result) {
  if (result != ERR_OK) {
    printf("Publish result: %d\n", result);
  }
}

//_____________________________________________________________________________

void connect() {
  err_t err;
  /*
   * Initiate client and connect to server, if this fails immediately an
   * error code is returned otherwise mqtt_connection_cb will be called with
   * connection result after attempting to establish a connection with
   * the server. For now MQTT version 3.1.1 is always used
   */
  err = mqtt_client_connect(&mqtt_client, &mqtt_ip_addr, MQTT_PORT,
                            mqtt_connection_cb,
                            LWIP_CONST_CAST(void *, &clientInfo), &clientInfo);
  /* For now just print the result code if something goes wrong */
  if (err != ERR_OK) {
    printf("mqtt_connect return %d\n", err);
  } else {
#if Debug
//    printf("Made the client\n");
#endif
  }
  mqtt_set_inpub_callback(&mqtt_client, mqtt_incoming_publish_cb,
                          mqtt_incoming_data_cb,
                          LWIP_CONST_CAST(void *, &clientInfo));
  subscribe_to_default_topics(&mqtt_client, 0);
}

void ensureConnection() {
  // mqtt_client_is_connected 1 if connected to server, 0 otherwise
  bool check_mqtt_connected = mqtt_client_is_connected(&mqtt_client);
  if (check_mqtt_connected == 0) {
    wifi_connect();
  }
}

void publish(mqtt_client_t *client, void *arg) {
  const char *pub_payload = "testpublish";
  err_t err;
  u8_t qos = 0;    /* 0 1 or 2, see MQTT specification */
  u8_t retain = 0; /* Don't retain the payload... */
  err = mqtt_publish(client, "test", pub_payload, strlen(pub_payload), qos,
                     retain, mqtt_pub_request_cb, arg);
  if (err != ERR_OK) {
    printf("Publish err: %d\n", err);
  }
}

//
//_Noreturn void mqttDebug(__unused void *params) {
//  printf("mqtt_task starts\n");
//  mqtt_subscribe(&mqtt_client, "testremote", 0, pub_mqtt_request_cb_t,
//                 PUB_EXTRA_ARG);
//
//  while (true) {
//    printf("in mqtt\n");
//    strcpy(PUB_PAYLOAD_SCR, PUB_PAYLOAD);
//    //    strcat(PUB_PAYLOAD_SCR, "testing");
//    payload_size = sizeof(PUB_PAYLOAD_SCR);
//    printf("%s  %d \n", PUB_PAYLOAD_SCR, sizeof(PUB_PAYLOAD_SCR));
//    bool check_mqtt_connected = mqtt_client_is_connected(&mqtt_client);
//    if (check_mqtt_connected == 0) {
//      connect();
//    }
//    // mqtt_client_is_connected 1 if connected to server, 0 otherwise
//    printf("saved_mqtt_client 0x%i check_mqtt_connected %d \n", mqtt_client,
//           check_mqtt_connected);
//
//    err_t err =
//        mqtt_publish(&mqtt_client, "test", PUB_PAYLOAD_SCR, payload_size, 0,
//        0,
//                     pub_mqtt_request_cb_t, PUB_EXTRA_ARG);
//    if (err != ERR_OK) {
//      printf("mqtt_publish return %d\n", err);
//    }
//  }
//}
#endif