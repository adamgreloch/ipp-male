#include "bitTable.h"
#include "cubes.h"

#define R_MODULO 4294967296

void setBit(uint8_t **arrayPtr, size_t bitIndex, int bitValue) {
    size_t cellIndex = bitIndex / 8;
    int k = (int) (7 - bitIndex % 8);

    if (bitValue)
        (*arrayPtr)[cellIndex] |= (1 << k);
    else
        (*arrayPtr)[cellIndex] &= ~(1 << k);
}

int getBit(uint8_t *arrayPtr, size_t bitIndex, inputData *d) {
    if (bitIndex > getMaxRank(d)) return -1;

    size_t cellIndex = bitIndex / 8;
    size_t k = 7 - bitIndex % 8;
    int n = arrayPtr[cellIndex];

    return (n & (1 << k)) >> k;
}

static uint8_t reverseBitOrder(uint8_t value) {
    uint8_t rev = 0;
    for (int i = 0; i < 7; i++) {
        if (value % 2 == 1)
            rev |= 1;
        value >>= 1;
        rev <<= 1;
    }
    return rev;
}

void
setRevBitsFromHex(uint8_t **arrayPtr, size_t valueIndex, int hexValue) {
    size_t cellIndex = valueIndex / 2;

    if (valueIndex % 2 == 0)
        (*arrayPtr)[cellIndex] = reverseBitOrder(hexValue);
    else
        (*arrayPtr)[cellIndex] |= reverseBitOrder(hexValue) >> 4;
}

void setBitsFromR(uint8_t **arrayPtr, size_t w, inputData *d) {
    while (w <= getMaxRank(d)) {
        setBit(arrayPtr, w, 1);
        w += R_MODULO;
    }
}
