#include "bitTable.h"

int getBit(uint8_t* arrayPtr, size_t bitIndex) {
    size_t cellIndex = bitIndex / 8;
    int k = 7 - bitIndex % 8;
    int n = arrayPtr[cellIndex];

    return (n & ( 1 << k )) >> k;
}

void setBit(uint8_t** arrayPtr, size_t bitIndex, int bitValue) {
    size_t cellIndex = bitIndex / 8;
    int k = 7 - bitIndex % 8;

    if (bitValue)
        (*arrayPtr)[cellIndex] |= (1 << k);
    else
        (*arrayPtr)[cellIndex] &= ~(1 << k);
}

void setTwoBit(uint8_t** arrayPtr, size_t twoBitIndex, int twoBitValue) {
    size_t cellIndex = twoBitIndex / 4;
    int inCellIndex = (int*) (twoBitIndex % 4);

//    printf("# ARR1: %d\n", (*arrayPtr)[cellIndex]);
    (*arrayPtr)[cellIndex] &= ~(0b11 << (2*inCellIndex));
//    printf("# ARR2: %d\n", (*arrayPtr)[cellIndex]);
    (*arrayPtr)[cellIndex] |= (twoBitValue << (2*inCellIndex));
//    printf("# ARR3: %d\n", (*arrayPtr)[cellIndex]);

//    printf("# set2B: ICI = %d, CI = %d, TBI = %d, TBV = %d\n", inCellIndex, cellIndex, twoBitIndex, twoBitValue);
}

int getTwoBit(uint8_t* arrayPtr, size_t twoBitIndex) {
    size_t cellIndex = twoBitIndex / 4;
    int inCellIndex = (int*) (twoBitIndex % 4);
    int k = inCellIndex;
    int n = arrayPtr[cellIndex];

    int twoBitValue = (n & ( 0b11 << (2*k) )) >> (2*k);
//    printf("# get2B: ICI = %d, CI = %d, TBI = %d, TBV = %d, ALL = %d\n", inCellIndex, cellIndex, twoBitIndex, twoBitValue, n);
    return twoBitValue;
}

/// @returns current bit length
size_t setBitsFromHex(uint8_t** arrayPtr, size_t valueIndex, int hexValue) {
    size_t cellIndex = valueIndex / 2;
    size_t bitLength;

    if (valueIndex % 2 == 0) {
        (*arrayPtr)[cellIndex] = hexValue << 4; // add 4 bits to the left
        bitLength = 8*(cellIndex) + 4;
    }
    else {
        (*arrayPtr)[cellIndex] |= hexValue; // add 4 bits to the right
        bitLength = 8*(cellIndex) + 8;
    }
    return bitLength;
}
