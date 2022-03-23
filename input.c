#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-narrowing-conversions"
#include "input.h"
#include "err.h"
#include "bitTable.h"
#include <stdint.h>

static size_t dimNum = -1;
static size_t maxInputBitLength = 0;

static uint8_t* getBinaryFromHex();
static uint8_t* getBinaryFromR();

static size_t getNum(char str[], size_t i, int inputLine) {
    str[i] = ' ';
    size_t num = strtoull(str, NULL, 10);
    if (num < 1)
        // input error: numbers have to be positive
        exitWithError(inputLine);
    else
        return num;
}

size_t* getInput(int inputLine) {
    size_t* inputArr = malloc(dimNum * sizeof(size_t));

    if (!inputArr)
        // error: malloc failed
        exitWithError(0);

    char str[sizeof(size_t)];
    int state = OUT;
    size_t i = 0, k = 0;
    char c;

    while ((c = getchar()) != '\n') {
        if (i >= sizeof(size_t)) { // TODO ERROR 0 niemal na pewno nie powinien być tak wykrywany
            // input error: labyrinth too big
            free(inputArr);
            exitWithError(0);
        }
        if (state == IN) {
            if (c != ' ') {
                str[i] = c;
                i++;
            } else {
                state = OUT;
                inputArr[k] = getNum(str, i, inputLine);
                k++;
                i = 0;
            }
        } else if (c != ' ') {
            state = IN;
            str[i] = c;
            i++;
        }
    }

    if (state == IN) {
        inputArr[k] = getNum(str, i, inputLine);
        k++;
    }

    if (k != dimNum) {
        // input error: number of arguments is mismatched throughout the input lines
        free(inputArr);
        exitWithError(inputLine);
    }

    return inputArr;
}

void getFirstInput(DA* arrayPtr, int inputLine) {
    char str[sizeof(size_t)];
    int state = OUT;
    size_t i = 0, k = 0;
    char c;

    while ((c = getchar()) != '\n') {
        if (i >= sizeof(size_t))
            // input error: labyrinth too big
            exitWithError(0);
        if (state == IN) {
            if (c != ' ') {
                str[i] = c;
                i++;
            } else {
                state = OUT;
                daPut(arrayPtr, k, getNum(str, i, inputLine));
                k++;
                i = 0;
            }
        } else if (c != ' ') {
            state = IN;
            str[i] = c;
            i++;
        }
    }

    if (state == IN) {
        daPut(arrayPtr, k, getNum(str, i, inputLine));
        k++;
    }

    if (k == 0)
        // input error: labyrinth dimension must be non-zero
        exitWithError(1);

    if (dimNum == -1)
        dimNum = k;
}

size_t getDimNum() {
    return dimNum;
}

/// getBinaryWallsRep detects fourth line input type and applies appropriate
/// input parsing function to get a binary expansion of the number in that line.
/// @param [in] arrayPtr is a pointer to an array destined to store
/// the binary expansion.
/// @return pointer to an array of two hex values per cell
uint8_t* getBinaryWallsRep() {
    char c;
    int inputType = -2; // 0 for hex, 1 for R, -1 for in-between state

    while (inputType < 0 && (c = getchar()) != '\n')
        if ((inputType == -2 && c == '0') || (inputType == -1 && c == 'x'))
            inputType++;
        else if (inputType == -2 && c == 'R')
            inputType = 1;
    if (inputType == 0)
        return getBinaryFromHex();
    else
        return getBinaryFromR();
}

/// getBinaryFromHex converts Hex number to Binary
/// @return array of bits
static uint8_t* getBinaryFromHex() {
    // To store binary expansion we will use an array of chars.
    // 1 char can store 8 bits. Since 1 hex digit represents 4 bits,
    // we can store 2 hex values in 1 char thus optimizing memory usage.
    // The bitTable module implements intuitive accessors for such type
    // of storage.

    size_t bitLength;
    uint8_t* arr = (uint8_t*) malloc(1024*sizeof(size_t));  // ! rozmiar narazie roboczy

    char c;
    size_t i = 0; // arrayPtr index
    int hexVal;

    while ((c = getchar()) != '\n') {
        // in ASCII, numbers and letters are ordered consecutively
        if ('0' <= c && c <= '9')
            hexVal = c - '0';
        else if ('A' <= c && c <= 'F')
            hexVal = 10 + c - 'A';
        else if ('a' <= c && c <= 'f')
            hexVal = 10 + c - 'a';

        bitLength = setBitsFromHex(&arr, i, hexVal);

        if (bitLength > maxInputBitLength)
            maxInputBitLength = bitLength;

        i++;
    }

    return arr;
}

uint8_t* getBinaryFromR() {

    uint8_t* arr = (uint8_t*) malloc(1024*sizeof(size_t));
    // TODO dorobić R
    size_t wallConfigNum;
    uint32_t rFormat[5];
    return arr;
}

size_t getMaxInputBitLength() {
    return maxInputBitLength;
}
