#ifndef MALE_BITTABLE_H
#define MALE_BITTABLE_H

#include <stdio.h>

int getBit(unsigned char* arrayPtr, size_t bitIndex);
void setBitsFromHex(unsigned char** arrayPtr, size_t valueIndex, int hexValue);
size_t getMaxBitLength();

#endif //MALE_BITTABLE_H
