#ifndef MALE_BITTABLE_H
#define MALE_BITTABLE_H

#include "dynarray.h"
#include <stdio.h>
#include <stdint.h>

// TODO remake bitTable for DA

void setBit(uint8_t **arrayPtr, size_t bitIndex, int bitValue);

int getBit(DA *arrayPtr, size_t bitIndex);

void setTwoBit(uint8_t **arrayPtr, size_t twoBitIndex, int twoBitValue);

int getTwoBit(uint8_t *arrayPtr, size_t twoBitIndex);

size_t setBitsFromHex(DA *arrayPtr, size_t valueIndex, int hexValue);

size_t getMaxBitLength();

size_t getMaxInputBitLength();

#endif //MALE_BITTABLE_H