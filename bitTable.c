#include "bitTable.h"
#include "dynarray.h"
#include "cubes.h"

#define R_MODULO 4294967296
static int debug_bitTable = 0;

void setBit(uint8_t** arrayPtr, size_t bitIndex, int bitValue) {
    size_t cellIndex = bitIndex / 8;
    int k = 7 - bitIndex % 8;

    if (bitValue)
        (*arrayPtr)[cellIndex] |= (1 << k);
    else
        (*arrayPtr)[cellIndex] &= ~(1 << k);
}

int getBit(uint8_t* arrayPtr, size_t bitIndex) {
    size_t cellIndex = bitIndex / 8;
    int k = 7 - bitIndex % 8;
    int n = arrayPtr[cellIndex];

    return (n & ( 1 << k )) >> k;
}

void setTwoBit(uint8_t **arrayPtr, size_t twoBitIndex, int twoBitValue) {
    size_t cellIndex = twoBitIndex / 4;
    int inCellIndex = (int *) (twoBitIndex % 4);

    (*arrayPtr)[cellIndex] &= ~(0b11 << (2 * inCellIndex));
    (*arrayPtr)[cellIndex] |= (twoBitValue << (2 * inCellIndex));

    if (debug_bitTable)
        printf("# set2B: ICI = %d, CI = %d, TBI = %d, TBV = %d\n", inCellIndex,
               cellIndex, twoBitIndex, twoBitValue);
}

int getTwoBit(uint8_t *arrayPtr, size_t twoBitIndex) {
    size_t cellIndex = twoBitIndex / 4;
    int inCellIndex = (int *) (twoBitIndex % 4);
    int k = inCellIndex;
    int n = arrayPtr[cellIndex];

    int twoBitValue = (n & (0b11 << (2 * k))) >> (2 * k);
    if (debug_bitTable)
        printf("# get2B: ICI = %d, CI = %d, TBI = %d, TBV = %d, ALL = %d\n",
               inCellIndex, cellIndex, twoBitIndex, twoBitValue, n);
    return twoBitValue;
}

/// @returns current bit length
size_t setBitsFromHex(uint8_t **arrayPtr, size_t valueIndex, int hexValue) {
    size_t cellIndex = valueIndex / 2;
    uint8_t cell;

    if (valueIndex % 2 == 0)
        (*arrayPtr)[cellIndex] = hexValue << 4;
    else
        (*arrayPtr)[cellIndex] |= hexValue;

    //return 8 * cellIndex + 4;
    return 4 * valueIndex + 4;
}

size_t setBitsFromR(uint8_t **arrayPtr, size_t index) {
    size_t bitLength = 0;
    while (index < getMaxRank()) {
        setBit(arrayPtr, index, 1);
        bitLength = index;
        if (debug_bitTable) printf("%zu\n", index);
        index += R_MODULO;
    }

    while (bitLength % 8 != 0)
        bitLength++;

    return bitLength;
}
