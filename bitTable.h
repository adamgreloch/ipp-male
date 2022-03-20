#ifndef MALE_BITTABLE_H
#define MALE_BITTABLE_H

#include <stdio.h>

int getBit(unsigned char* arrayPtr, size_t bitIndex);
size_t setBitsFromHex(unsigned char** arrayPtr, size_t valueIndex, int hexValue);

size_t getMaxBitLength();
size_t getMaxInputBitLength();

#endif //MALE_BITTABLE_H
