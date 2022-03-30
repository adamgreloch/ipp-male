#pragma clang diagnostic push

#include "input.h"
#include "errMem.h"
#include "cubes.h"
#include "bitTable.h"
#include <stdint.h>

#define IN 1
#define OUT 0
#define INITIAL_SIZE 100
#define INITIAL_DIM 4

static inputData *d;

static void *safe_malloc(size_t size) {
    void *p = malloc(size);
    if (!p) exitWithError(0, d);
    return p;
}

static void *safe_realloc(void *p, size_t size) {
    p = realloc(p, size);
    if (!p) exitWithError(0, d);
    return p;
}

static void *safe_calloc(size_t num, size_t size) {
    void *p = calloc(num, size);
    if (!p) exitWithError(0, d);
    return p;
}

static size_t getNum(int *str, size_t i, int inputLine) {
    size_t j = 0;
    size_t num = 0;
    while (j < i) {
        num = 10 * num + str[j] - '0';
        j++;
    }
    if (inputLine != 4 && num < 1) {
        free(str);
        // Error: Numbers have to be positive.
        exitWithError(inputLine, d);
    }
    else
        return num;
}

static size_t *getFirstInputLine() {
    // TODO wyłapać wycieki! pojawiają się przy testach error10.in, error30.in, kwasowski_7.in
    // dotyczą struktur utworzonych w getFirstInputLine(), getOtherInputLines()
    size_t inputSize = INITIAL_DIM;
    size_t *input = safe_malloc(inputSize * sizeof(size_t));
    int *str = safe_malloc(sizeof(char) * UINT16_MAX);

    int state = OUT;
    size_t i = 0, k = 0;
    int c;

    while ((c = getchar()) != '\n') {
        if (k == inputSize) {
            inputSize *= 2;
            input = realloc(input, inputSize * sizeof(size_t));
            if (!input) {
                free(str);
                exitWithError(0, d);
            }
        }
        if (isalpha(c)) {
            // Error: Non-digit character encountered.
            free(input);
            free(str);
            return NULL;
        }
        if (state == IN) {
            if (!isspace(c)) {
                str[i] = c;
                i++;
            } else {
                state = OUT;
                input[k] = getNum(str, i, 1);
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
        input[k] = getNum(str, i, 1);
        k++;
    }

    if (k == 0) {
        // Error: Dimension must be non-zero.
        free(input);
        free(str);
        return NULL;
    }

    if (!(d->dimNum))
        d->dimNum = k;

    free(str);

    return input;
}

static size_t *getOtherInputLines(int inputLine, size_t argumentsCount) {
    size_t *inputArr = safe_malloc(argumentsCount * sizeof(size_t));
    int *str = safe_malloc(sizeof(char) * UINT16_MAX);

    int state = OUT;
    size_t i = 0, k = 0;
    int c;

    while ((c = getchar()) != '\n' && c != EOF) {
        if (isalpha(c)) {
            free(inputArr);
            free(str);
            return NULL;
        }
        if (state == IN) {
            if (!isspace(c)) {
                str[i] = c;
                i++;
            } else {
                state = OUT;
                inputArr[k] = getNum(str, i, inputLine);
                if (inputLine < 4 && inputArr[k] > (d->dimensions)[k]) {
                    // Error: Position is outside dimension.
                    free(inputArr);
                    free(str);
                    return NULL;
                }
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
        k++;
    }

    if (k != argumentsCount) {
        // Error: Unexpected number of input arguments.
        free(str);
        free(inputArr);
        return NULL;
    }

    free(str);
    return inputArr;
}

/**
 * getBinaryFromHex converts Hex number to Binary
 * @return array of bits
 */
static uint8_t *getBinaryFromHex() {

    size_t tableSize = INITIAL_SIZE;
    uint8_t *hexTable = safe_malloc(tableSize * sizeof(uint8_t));

    int c;
    int hexVal;
    int leadingZeros = 1;
    int foundWhitespace = 0;
    size_t i = 0; // bitArray index

    while ((c = getchar()) != EOF && c != '\n') {
        if (isspace(c))
            foundWhitespace = 1;

        else if (foundWhitespace) {
            // Input error: Whitespace between digits encountered.
            free(hexTable);
            exitWithError(4, d);
        }

        else if (!leadingZeros || c != '0') {
            leadingZeros = 0;

            if (i == tableSize) {
                tableSize *= 2;
                hexTable = safe_realloc(hexTable, tableSize * sizeof(uint8_t));
            }

            // In ASCII, numbers and letters are ordered consecutively.
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

    if (i > 1 && 4*i > getDimProduct(d->dimNum)) {
        free(hexTable);
        exitWithError(4, d);
    }

    uint8_t *bitArray = calloc(getMaxRank(d) / 8 + 1, sizeof(uint8_t));
    if (!bitArray) {
        free(hexTable);
        exitWithError(0, d);
    }

    for (size_t j = 0; j < i; j++)
        setRevBitsFromHex(&bitArray, j, hexTable[i - j - 1]);

    free(hexTable);

    return bitArray;
}

static uint8_t *getBinaryFromR() {
    uint8_t *bitArray = safe_calloc(getMaxRank(d) / 8 + 1, sizeof(uint8_t));

    size_t dimProduct = getDimProduct(d->dimNum);

    size_t *params = getOtherInputLines(4, 5);
    if (!params) {
        free(bitArray);
        exitWithError(4, d);
    }
    size_t a = params[0], b = params[1], m = params[2], r = params[3];

    size_t *sw = safe_malloc((r + 1) * sizeof(size_t));
    // We will later reuse the sw array for storing w_i numbers.
    // Apart from calculating w_i's, s_i array is useless afterwards.

    sw[0] = params[4];
    free(params);

    if (m == 0) return NULL;

    for (size_t i = 1; i <= r; i++)
        sw[i] = (a * sw[i - 1] + b) % m;

    for (size_t i = 0; i <= r; i++)
        sw[i] = sw[i] % dimProduct;

    for (size_t i = 1; i <= r; i++)
        setBitsFromR(&bitArray, sw[i], d);

    free(sw);
    return bitArray;
}

/**
 *  @brief getBinaryWallsRep detects fourth line input type and applies
 *  appropriate input parsing function to get a binary expansion of the
 *  number in that line.
 *  @return Pointer to bitTable.
 *  @see bitTable.h
 */
static uint8_t *getBinaryWallsRep() {
    int c;
    int inputType = -2; // (-2)->(-1)->0 for hex, (-2)->1 for R

    while (inputType < 0) {
        c = getchar();
        if (inputType != -2 && isspace(c))
            // Input error: Space between '0' and 'x' encountered.
            exitWithError(4, d);
        if ((inputType == -2 && c == '0') || (inputType == -1 && c == 'x'))
            inputType++;
        else if (inputType == -2 && c == 'R')
            inputType = 1;
    }
    if (inputType == 0)
        return getBinaryFromHex();
    else
        return getBinaryFromR();
}

size_t getDimProduct(size_t maxNIndex) {
    if (!(d->dimProducts)) {
        d->dimProducts = safe_malloc(sizeof(size_t) * (d->dimNum));
        (d->dimProducts)[0] = (d->dimensions)[0];

        for (size_t i = 1; i < d->dimNum; i++)
            (d->dimProducts)[i] = 0;
    }
    if (maxNIndex < 1)
        return 1;
    else if (maxNIndex > d->dimNum)
        return -1;
    else if ((d->dimProducts)[maxNIndex - 1] != 0)
        return (d->dimProducts)[maxNIndex - 1];
    else {
        size_t product, a, b;

        a = getDimProduct(maxNIndex - 1);
        b = (d->dimensions)[maxNIndex - 1];
        product = a * b;

        if (a != product / b)
            exitWithError(1, d);

        (d->dimProducts)[maxNIndex - 1] = product;
        return product;
    }
}

inputData *getInputData() {
    d = malloc(sizeof(inputData));
    d->dimNum = NULL;
    d->dimProducts = NULL;
    d->dimensions = NULL;
    d->startPos = NULL;
    d->endPos = NULL;
    d->binaryRep = NULL;
    d->dimProducts = NULL;

    if (!(d->dimensions = getFirstInputLine())) exitWithError(1, d);
    if (!(d->startPos = getOtherInputLines(2, d->dimNum))) exitWithError(2, d);
    if (!(d->endPos = getOtherInputLines(3, d->dimNum))) exitWithError(3, d);
    if (!(d->binaryRep = getBinaryWallsRep())) exitWithError(4, d);

    if (getchar() != EOF)
        exitWithError(5, d);

    if (getBit(d->binaryRep, rankCube(d->startPos, d), d) != 0)
        exitWithError(2, d);

    if (getBit(d->binaryRep, rankCube(d->endPos, d), d) != 0)
        exitWithError(3, d);

    return d;
}
