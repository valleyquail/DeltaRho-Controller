//
// Created by nikesh on 7/10/23.
//

#include "connection.h"

const char *topics[] = {"instruc", "full", "adc", "misc"};

const enum TOPIC topics_to_subscribe_to[] = {INSTRUCTIONS, QUEUE_FULL};

void parseInstruction(char *instruction, dataPacket *packet) {
  packet->speed = 0;
  packet->theta = 0;
  packet->distance = 0;
  packet->omega = 0;
  packet->phi = 0;
  char *distance_index = strchr(instruction, 'D');
  char *controlVals;
  if (distance_index != NULL) {
    // Removes the "F" from the front of the string
    printf("string: %s\n", instruction);
    controlVals = strtok(instruction, "iFT");
    printf("string: %s\n", instruction);
    packet->speed = atof(controlVals);
    printf("speed: %f\n", packet->speed);
    controlVals = strtok(NULL, "TD");
    packet->theta = atof(controlVals);
    printf("theta: %f\n", packet->theta);
    controlVals = strtok(NULL, NULL);
    packet->distance = atof(controlVals);
    printf("dist: %f\n", packet->distance);

  } else {
    controlVals = strtok(instruction, "iRP");
    packet->omega = atof(controlVals);
    controlVals = strtok(NULL, NULL);
    packet->phi = atof(controlVals);
  }
}