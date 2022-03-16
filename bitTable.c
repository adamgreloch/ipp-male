#include "bitTable.h"

static size_t maxBitLength = 0;

int getBit(unsigned char* arrayPtr, size_t bitIndex) {
    size_t cellIndex = bitIndex / 8;
    int k = 7 - bitIndex % 8;
    int n = arrayPtr[cellIndex];

    return (n & ( 1 << k )) >> k;
}

void setBitsFromHex(unsigned char** arrayPtr, size_t valueIndex, int hexValue) {
    int bitLength;

    if (valueIndex % 2 == 0) {
        (*arrayPtr)[valueIndex/2] = hexValue << 4; // add 4 bits to the left
        bitLength = 8*(valueIndex/2) + 4;
    }
    else {
        (*arrayPtr)[valueIndex/2] |= hexValue; // add 4 bits to the right
        bitLength = 8*(valueIndex/2) + 8;
    }
    if (bitLength > maxBitLength)
        maxBitLength = bitLength;
}

size_t getMaxBitLength() {
    return maxBitLength;
}
