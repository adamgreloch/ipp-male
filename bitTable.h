#ifndef MALE_BITTABLE_H
#define MALE_BITTABLE_H

#include "dynarray.h"
#include <stdio.h>
#include <stdint.h>

void setBit(uint8_t **arrayPtr, size_t bitIndex, int bitValue);

int getBit(uint8_t *arrayPtr, size_t bitIndex);

void
setReversedBitsFromHex(uint8_t **arrayPtr, size_t valueIndex, int hexValue);

void setBitsFromR(uint8_t **arrayPtr, size_t remainder);

#endif //MALE_BITTABLE_H