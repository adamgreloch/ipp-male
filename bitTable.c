#include "bitTable.h"
#include "dynarray.h"

static int debug = 0;

int getBit(DA *arrayPtr, size_t bitIndex) {
    size_t cellIndex = bitIndex / 8;
    int k = (int) (7 - bitIndex % 8);
    int n = daGet8Bit(arrayPtr, cellIndex);

    if (debug) printf("# get1B: %d\n", (n & (1 << k)) >> k);
    return (n & (1 << k)) >> k;
}

void setBit(uint8_t **arrayPtr, size_t bitIndex, int bitValue) {
    size_t cellIndex = bitIndex / 8;
    int k = 7 - bitIndex % 8;

    if (bitValue)
        (*arrayPtr)[cellIndex] |= (1 << k);
    else
        (*arrayPtr)[cellIndex] &= ~(1 << k);
}

void setTwoBit(uint8_t **arrayPtr, size_t twoBitIndex, int twoBitValue) {
    size_t cellIndex = twoBitIndex / 4;
    int inCellIndex = (int *) (twoBitIndex % 4);

    (*arrayPtr)[cellIndex] &= ~(0b11 << (2 * inCellIndex));
    (*arrayPtr)[cellIndex] |= (twoBitValue << (2 * inCellIndex));

    if (debug) printf("# set2B: ICI = %d, CI = %d, TBI = %d, TBV = %d\n", inCellIndex, cellIndex, twoBitIndex, twoBitValue);
}

int getTwoBit(uint8_t *arrayPtr, size_t twoBitIndex) {
    size_t cellIndex = twoBitIndex / 4;
    int inCellIndex = (int *) (twoBitIndex % 4);
    int k = inCellIndex;
    int n = arrayPtr[cellIndex];

    int twoBitValue = (n & (0b11 << (2 * k))) >> (2 * k);
    if (debug) printf("# get2B: ICI = %d, CI = %d, TBI = %d, TBV = %d, ALL = %d\n", inCellIndex, cellIndex, twoBitIndex, twoBitValue, n);
    return twoBitValue;
}

/// @returns current bit length
size_t setBitsFromHex(DA *arrayPtr, size_t valueIndex, int hexValue) {
    size_t cellIndex = valueIndex / 2;
    size_t bitLength;
    uint8_t cell;

    if (valueIndex % 2 == 0) {
        daBut8Bit(arrayPtr, cellIndex, hexValue << 4);
        //(*arrayPtr)[cellIndex] = hexValue << 4; // add 4 bits to the left
        bitLength = 8 * (cellIndex) + 4;
    } else {
        cell = daGet8Bit(arrayPtr, cellIndex);
        daBut8Bit(arrayPtr, cellIndex, cell | hexValue);
        //(*arrayPtr)[cellIndex] |= hexValue; // add 4 bits to the right
        bitLength = 8 * (cellIndex) + 4;
    }
    return bitLength;
}
