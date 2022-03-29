#pragma clang diagnostic push

#include "input.h"
#include "err.h"
#include "cubes.h"
#include "main.h"
#include "bitTable.h"
#include <stdint.h>

//#define DEBUG_INPUT

#define IN 1
#define OUT 0
#define INITIAL_SIZE 100

static size_t dimNum = -1;

static uint8_t *getBinaryFromHex();

static uint8_t *getBinaryFromR();

static size_t getNum(int *str, size_t i, int inputLine) {
    size_t j = 0;
    size_t num = 0;
    while (j < i) {
        num = 10 * num + str[j] - '0';
        j++;
    }
    if (inputLine != 4 && num < 1)
        // input error: numbers have to be positive
        exitWithError(inputLine);
    else
        return num;
}

// TODO check for abnormal input characters

size_t *getInput(int inputLine, size_t argumentsCount) {
    size_t *inputArr = malloc(argumentsCount * sizeof(size_t));

    if (!inputArr)
        // error: malloc failed
        exitWithError(0);

    int *str = malloc(sizeof(char) * UINT16_MAX);
    int state = OUT;
    size_t i = 0, k = 0;
    int c;

    while ((c = getchar()) != '\n') {
        if (state == IN) {
            if (!isspace(c)) {
                str[i] = c;
                i++;
            } else {
                state = OUT;
                inputArr[k] = getNum(str, i, inputLine);
#ifdef DEBUG_INPUT
                printf("%s -> %zu (k=%zu)\n", str, inputArr[k], k);
#endif
                k++;
                i = 0;
            }
        } else if (!isspace(c)) {
            state = IN;
            str[i] = c;
            i++;
        }
    }

    if (state == IN) {
        inputArr[k] = getNum(str, i, inputLine);
#ifdef DEBUG_INPUT
        printf("%s -> %zu (k=%zu)\n", str, inputArr[k], k);
#endif
        k++;
    }

    if (k != argumentsCount) {
        // input error: unexpected number of input arguments
        free(inputArr);
        exitWithError(inputLine);
    }

    free(str);

#ifdef DEBUG_INPUT
    for (int j = 0; j < dimNum; j++)
        printf("inputArr[%zu] = %zu\n", j, inputArr[j]);
#endif

    return inputArr;
}

void getFirstInput(DA *arrayPtr) {
    int *str = malloc(sizeof(char) * UINT16_MAX);
    int state = OUT;
    size_t i = 0, k = 0;
    int c;

    while ((c = getchar()) != '\n') {
        if (state == IN) {
            if (!isspace(c)) {
                str[i] = c;
                i++;
            } else {
                state = OUT;
                daPut(arrayPtr, k, getNum(str, i, 1));
                k++;
                i = 0;
            }
        } else if (!isspace(c)) {
            state = IN;
            str[i] = c;
            i++;
        }
    }

    if (state == IN) {
        daPut(arrayPtr, k, getNum(str, i, 1));
        k++;
    }

    if (k == 0)
        // input error: labyrinth dimension must be non-zero
        exitWithError(1);

    if (dimNum == -1)
        dimNum = k;

    free(str);
}

/***
 *  getBinaryWallsRep detects fourth line input type and applies appropriate
 *  input parsing function to get a binary expansion of the number in that line.
 *  @param [in] arrayPtr is a pointer to an array destined to store
 *  the binary expansion.
 *  @return pointer to an array of two hex values per cell
 */
uint8_t *getBinaryWallsRep() {
    int c;
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

/***
 * getBinaryFromHex converts Hex number to Binary
 * @return array of bits
 */
static uint8_t *getBinaryFromHex() {
    // To store the binary expansion we will use an array of chars.
    // 1 char can store 8 bits. Since 1 hex digit represents 4 bits,
    // we can store 2 hex values in 1 char thus optimizing memory usage.
    // The bitTable module implements intuitive accessors for such type
    // of storage.

    size_t tableSize = INITIAL_SIZE;
    uint8_t *hexTable = malloc(tableSize * sizeof(uint8_t));
    if (!hexTable) exitWithError(0);

    int c;
    int hexVal;
    int leadingZeros = 1;
    size_t i = 0; // bitArray index

    while ((c = getchar()) != EOF && !isspace(c)) {
        if (i == tableSize) {
            tableSize *= 2;
            hexTable = realloc(hexTable, tableSize * sizeof(uint8_t));
            if (!hexTable) exitWithError(0);
        }
        if (!leadingZeros || c != '0') {
            leadingZeros = 0;
            // in ASCII, numbers and letters are ordered consecutively
            if ('0' <= c && c <= '9')
                hexVal = c - '0';
            else if ('A' <= c && c <= 'F')
                hexVal = 10 + c - 'A';
            else if ('a' <= c && c <= 'f')
                hexVal = 10 + c - 'a';

            hexTable[i] = hexVal;
            i++;
        }
    }

    uint8_t *bitArray = calloc(tableSize, sizeof(uint8_t));
    if (!bitArray) exitWithError(0);

    for (size_t j = 0; j < i; j++)
        setReversedBitsFromHex(&bitArray, j, hexTable[i - j - 1]);

    free(hexTable);

    return bitArray;
}

uint8_t *getBinaryFromR() {
    // TODO implement dynamic tables
    uint8_t *bitArray = calloc(getMaxRank() / 8 + 1, sizeof(uint8_t));
    if (!bitArray) exitWithError(0);

    size_t dimProduct = getDimProduct(dimNum);

#ifdef DEBUG_INPUT
    printf("dimProduct: %zu\n", dimProduct);
#endif

    size_t *params = getInput(4, 5);
    size_t a = params[0], b = params[1], m = params[2], r = params[3];

#ifdef DEBUG_INPUT
    for (int i = 0; i < 5; i++)
        printf("params[%lu] = %lu\n", i, params[i]);
#endif

    size_t *sw = malloc((r + 1) * sizeof(size_t));
    // We will later reuse the sw array for storing w_i numbers.
    // Apart from calculating w_i's, s_i array is useless afterwards.

    sw[0] = params[4];
    free(params);

    if (m == 0) return -1;

    for (size_t i = 1; i <= r; i++) {
        sw[i] = (a * sw[i - 1] + b) % m;
#ifdef DEBUG_INPUT
        printf("s[%lu] = %lu\n", i, sw[i]);
#endif
    }

    for (size_t i = 0; i <= r; i++) {
        sw[i] = sw[i] % dimProduct;
#ifdef DEBUG_INPUT
        printf("w[%lu] = %lu\n", i, sw[i]);
#endif
    }

    for (size_t i = 1; i <= r; i++)
        setBitsFromR(&bitArray, sw[i]);

    free(sw);

    return bitArray;
}

size_t *dimProducts;

// TODO przerobić na iterację
size_t getDimProduct(size_t maxNIndex) {
    if (!dimProducts) {
        dimProducts = malloc(sizeof(size_t) * dimNum);
        dimProducts[0] = daGet(getDimensions(), 0);

        for (size_t i = 1; i < dimNum; i++)
            dimProducts[i] = 0;
    }
    if (maxNIndex < 1)
        return 1;
    else if (maxNIndex > dimNum)
        return -1;
    else if (dimProducts[maxNIndex - 1] != 0)
        return dimProducts[maxNIndex - 1];
    else {
        size_t product, a, b;

        a = getDimProduct(maxNIndex - 1);
        b = daGet(getDimensions(), maxNIndex - 1);
        product = a * b;

        if (a != product / b) exitWithError(1);

        dimProducts[maxNIndex - 1] = product;
        return product;
    }
}

size_t getDimNum() { return dimNum; }
