//
// Created by nikesh on 7/6/23.
//

#include "DCMotor.h"

//
// Created by nikesh on 7/6/23.
//

#ifndef DELTARHO_CONTROLLER_MOTORENCODER_H
#define DELTARHO_CONTROLLER_MOTORENCODER_H

/**
 * Handles the encoders based on an XOR'd signal from the encoders and then
 * using the known direction to set the count
 */
void DCMotor::encoderIRQ(uint gpio, uint32_t events) {
  if (direction)
    currCount++;
  else
    currCount--;
}
#endif // DELTARHO_CONTROLLER_MOTORENCODER_H
