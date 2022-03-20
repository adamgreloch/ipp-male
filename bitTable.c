#include "bitTable.h"

int getBit(unsigned char* arrayPtr, size_t bitIndex) {
    size_t cellIndex = bitIndex / 8;
    int k = 7 - bitIndex % 8;
    int n = arrayPtr[cellIndex];

    return (n & ( 1 << k )) >> k;
}

/// @returns current bit length
size_t setBitsFromHex(unsigned char** arrayPtr, size_t valueIndex, int hexValue) {
    size_t bitLength;

    if (valueIndex % 2 == 0) {
        (*arrayPtr)[valueIndex/2] = hexValue << 4; // add 4 bits to the left
        bitLength = 8*(valueIndex/2) + 4;
    }
    else {
        (*arrayPtr)[valueIndex/2] |= hexValue; // add 4 bits to the right
        bitLength = 8*(valueIndex/2) + 8;
    }

    return bitLength;
}
