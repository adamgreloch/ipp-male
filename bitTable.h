#ifndef MALE_BITTABLE_H
#define MALE_BITTABLE_H

#include "input.h"
#include <stdio.h>
#include <stdint.h>

void setBit(uint8_t **arrayPtr, size_t bitIndex, int bitValue);

int getBit(uint8_t *arrayPtr, size_t bitIndex, inputData *d);

void
setReversedBitsFromHex(uint8_t **arrayPtr, size_t valueIndex, int hexValue);

void setBitsFromR(uint8_t **arrayPtr, size_t remainder, inputData *d);

#endif //MALE_BITTABLE_H