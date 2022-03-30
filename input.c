#pragma clang diagnostic push

#include "input.h"
#include "errMem.h"
#include "cubes.h"
#include "bitTable.h"
#include <stdint.h>

#define IN 1
#define OUT 0
#define ERR -1
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

/**
 * Gets numerical size_t value from a given string.
 * @warn Assumes str != NULL.
 * @param str - String to parse.
 * @param i - Last char index.
 * @return Parsed value.
 */
static size_t getNum(int *str, size_t i) {
    size_t j = 0;
    size_t num = 0, prev;
    while (j < i) {
        prev = num;
        num = 10 * num + str[j] - '0';
        if (prev > num) return 0;
        j++;
    }
    return num;
}

/**
 * Tries to parse a given string. Will return 0 if resulting number does not
 * meet all input criteria.
 * @param parsed - Array of parsed input numbers.
 * @param str - String to parse.
 * @param line - Input line.
 * @param k - Next free number index in parsed array.
 * @param i - Last char index.
 * @return Next increment of k or 0 if criteria check failed.
 */
static size_t tryToParse(size_t **parsed, int *str,
                         size_t line, size_t k, size_t i) {
    size_t num = getNum(str, i);

    if (line != 4)
        if (num < 1 || (line != 1 && (d->dimensions)[k] < num)) {
            // Error: Position is outside dimension or not positive.
            free(*parsed); free(str); return 0;
        }

    (*parsed)[k] = num;
    k++;
    return k;
}

/**
 * @brief Gets numerical input from a line.
 * @warn Assumes 1 <= line <= 4.
 * @param line - Input line number.
 * @param argumentsCount - Number of arguments (ignored for lines 1,4).
 * @return Parsed input array.
 */
static size_t *getInputLine(int line, size_t argumentsCount) {
    int *str = safe_malloc(sizeof(char) * UINT16_MAX);
    size_t inputSize = INITIAL_DIM;
    size_t *parsed;

    if (line == 1)
        parsed = safe_malloc(inputSize * sizeof(size_t));
    else
        parsed = safe_malloc(argumentsCount * sizeof(size_t));

    int c, state = OUT;
    size_t i = 0, k = 0;

    while (state != ERR && (c = getchar()) != '\n') {
        if (line == 1 && k == inputSize) {
            inputSize *= 2;
            parsed = realloc(parsed, inputSize * sizeof(size_t));
            if (!parsed) state = ERR;
        }
        if (isalpha(c)) state = ERR;
        if (state == IN) {
            if (!isspace(c)) {
                str[i] = c;
                i++;
            } else {
                state = OUT;
                if (!(k = tryToParse(&parsed, str, line, k, i))) return NULL;
                i = 0;
            }
        } else if (!isspace(c)) {
            state = IN;
            str[i] = c;
            i++;
        }
    }

    // TODO handle wojtekr_zly08.in

    if (state == ERR) {
        // In such state two errors are possible:
        // 1. realloc failed
        if (!parsed) exitWithError(0, d);
        // 2. Non-digit character encountered. Pass error on.
        free(str); return NULL;
    } else if (state == IN)
        if (!(k = tryToParse(&parsed, str, line, k, i))) return NULL;

    free(str);

    if (!(d->dimNum)) d->dimNum = k;

    if (line != 1 && k != argumentsCount) { free(parsed); return NULL; }
    // Error: Unexpected number of input arguments.

    return parsed;
}

static int getHexValue(int c) {
    // In ASCII, numbers and letters are ordered consecutively, making hex
    // values easy to obtain.
    if ('0' <= c && c <= '9')
        return c - '0';
    else if ('A' <= c && c <= 'F')
        return 10 + c - 'A';
    else if ('a' <= c && c <= 'f')
        return 10 + c - 'a';
    else
        return -1;
}

/**
 * @brief Inverts cells of "bit-reversed" bitTable, thus creating a proper 4th
 * line number binary representation with the least significant bit in the
 * index 0. Technically, it is just inverting cells from 0 to last.
 * @param revHexTable - Bit-reversed bitTable
 * @param last - Last filled cell.
 * @return Inverted bitTable.
 */
static uint8_t *invertBitTable(uint8_t *revHexTable, size_t last) {
    uint8_t *bitArray = calloc(getMaxRank(d) / 8 + 1, sizeof(uint8_t));

    if (!bitArray) { free(revHexTable); exitWithError(0, d); }

    for (size_t j = 0; j < last; j++)
        setRevBitsFromHex(&bitArray, j, revHexTable[last - j - 1]);

    free(revHexTable);
    return bitArray;
}

/**
 * Converts Hex number to Binary
 * @return array of bits
 */
static uint8_t *getBinaryFromHex() {
    size_t tableSize = INITIAL_SIZE;
    uint8_t *revHexTable = safe_malloc(tableSize * sizeof(uint8_t));

    int c, hexVal, state = IN;
    int leadingZeros = 1;
    int foundWhitespace = 0;
    size_t i = 0; // bitArray index

    while (state != ERR && (c = getchar()) != EOF && c != '\n') {
        if (isspace(c))
            foundWhitespace = 1;
        else if (foundWhitespace) state = ERR;
            // Input error: Whitespace between digits encountered.
        else if (!leadingZeros || c != '0') {
            leadingZeros = 0;
            if (i == tableSize) {
                tableSize *= 2;
                revHexTable = safe_realloc(revHexTable,
                                           tableSize * sizeof(uint8_t));
            }
            if ((hexVal = getHexValue(c)) == -1) state = ERR;
            revHexTable[i] = hexVal;
            i++;
        }
    }
    if (state == ERR || (i > 1 && 4 * i > getDimProduct(d->dimNum))) {
        free(revHexTable); exitWithError(4, d);
    }
    return invertBitTable(revHexTable, i);
}

static uint8_t *getBinaryFromR() {
    uint8_t *bitArray = safe_calloc(getMaxRank(d) / 8 + 1, sizeof(uint8_t));
    size_t dimProduct = getDimProduct(d->dimNum);
    size_t *params = getInputLine(4, 5);
    if (!params) {
        free(bitArray); exitWithError(4, d);
    }

    size_t a = params[0], b = params[1], m = params[2], r = params[3];

    for (int i = 0; i < 4; i++)
        if (params[i] > UINT32_MAX) {
            free(bitArray); free(params); return NULL;
        }

    size_t *sw = safe_malloc((r + 1) * sizeof(size_t));
    // We will later reuse the sw array for storing w_i numbers.
    // Apart from calculating w_i's, s_i array is useless afterwards.

    sw[0] = params[4];
    free(params);

    if (m == 0) { free(bitArray); free(sw); return NULL; }

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
        if (c == '\n' || inputType != -2 && isspace(c))
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

// TODO refactor/document
size_t getDimProduct(size_t maxNIndex) {
    if (!(d->dimProducts)) {
        d->dimProducts = safe_malloc(sizeof(size_t) * (d->dimNum));
        (d->dimProducts)[0] = (d->dimensions)[0];

        for (size_t i = 1; i < d->dimNum; i++)
            (d->dimProducts)[i] = 0;
    }
    if (maxNIndex < 1) return 1;
    else if (maxNIndex > d->dimNum) return -1;
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
    d->dimNum = d->dimProducts = d->dimensions = d->startPos = d->endPos =
            d->binaryRep = d->dimProducts = NULL;

    if (!(d->dimensions = getInputLine(1, NULL))) exitWithError(1, d);
    if (!(d->startPos = getInputLine(2, d->dimNum))) exitWithError(2, d);
    if (!(d->endPos = getInputLine(3, d->dimNum))) exitWithError(3, d);
    if (!(d->binaryRep = getBinaryWallsRep())) exitWithError(4, d);

    if (getchar() != EOF) exitWithError(5, d);
    if (getBit(d->binaryRep, rankCube(d->startPos, d), d) != 0)
        exitWithError(2, d);
    if (getBit(d->binaryRep, rankCube(d->endPos, d), d) != 0)
        exitWithError(3, d);

    return d;
}
