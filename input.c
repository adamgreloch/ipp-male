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

static struct parseData {
    int *str;
    int state;
    int line;
    size_t i;
    size_t k;
    size_t inputSize;
};

typedef struct parseData parseData;

static parseData *initParseData(int line) {
    parseData *p = safe_malloc(sizeof(parseData));
    p->str = safe_malloc(sizeof(int) * UINT16_MAX);
    p->line = line;
    p->state = OUT;
    p->i = p->k = 0;

    return p;
}

static void freeParseData(parseData *p) {
    free(p->str);
    free(p);
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
 * Tries to parse a given string. Will set p->state as ERR if parsed number
 * is incorrect.
 * @param parsed - Array of parsed input numbers.
 * @param p - Parse data pointer.
 */
static void tryToParse(size_t **parsed, parseData *p) {
    if (p->k == p->inputSize) p->state = ERR; // ERR: More args than expected.
    else {
        size_t num = getNum(p->str, p->i);

        if (p->line != 4)
            if (num < 1 || (p->line != 1 && (d->dimensions)[p->k] < num))
                // ERR: Position was not positive when it should or was
                // outside dimension.
                p->state = ERR;

        (*parsed)[p->k] = num;
        (p->k)++;
    }
}

/**
 * Constructs an array of chars, char by char. Checks for some immediate
 * input errors.
 * @param c - Input character.
 * @param parsed - Array of parsed input numbers.
 * @param p - Parse data pointer.
 */
static void stringConstructor(int c, size_t **parsed, parseData *p) {
    if (p->line == 1 && p->k == p->inputSize) {
            p->inputSize *= 2;
            *parsed = realloc(*parsed, p->inputSize * sizeof(size_t));
            if (!(*parsed)) p->state = ERR;
        }
    if (isalpha(c)) p->state = ERR;
    if (p->state == IN) {
        if (!isspace(c)) {
            (p->str)[p->i] = c;
            (p->i)++;
        } else {
            p->state = OUT;
            tryToParse(parsed, p);
            p->i = 0;
        }
    } else if (!isspace(c)) {
        p->state = IN;
        (p->str)[p->i] = c;
        (p->i)++;
    }
}

/**
 * @brief Gets numerical input from a line.
 * @warn Assumes 1 <= line <= 4.
 * @param line - Input line number.
 * @param argumentsCount - Number of arguments (ignored for lines 1,4).
 * @return Parsed input array.
 */
static size_t *getInputLine(int line, size_t argumentsCount) {
    parseData *p = initParseData(line);

    if (!(d->dimNum)) p->inputSize = INITIAL_DIM;
    else p->inputSize = argumentsCount;
    size_t *parsed = safe_malloc((p->inputSize) * sizeof(size_t));

    int c;
    while (p->state != ERR && (c = getchar()) != '\n' && c != EOF)
        stringConstructor(c, &parsed, p);

    if ((p->state) == IN) tryToParse(&parsed, p);

    if (p->state == ERR) {
        if (!parsed) exitWithError(0, d); // ERR: realloc failed earlier.
        free(parsed); freeParseData(p); return NULL; // ...any other error.
    }

    if (p->line != 1 && p->k < argumentsCount) {
        // ERR: Got less input arguments than expected.
        free(parsed); freeParseData(p); return NULL;
    }

    if (!(d->dimNum)) d->dimNum = p->k;

    freeParseData(p);
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
 * Converts hex number to Binary
 * @return array of bits
 */
static uint8_t *getBinaryFromHex() {
    size_t tableSize = INITIAL_SIZE;
    uint8_t *revHexTable = safe_malloc(tableSize * sizeof(uint8_t));

    int c, hexVal, state = IN;
    int leadingZeros = 1;
    int foundWhitespace = 0;
    size_t i = 0; // bitArray index

    while (state != ERR && (c = getchar()) != '\n' && c != EOF) {
        if (isspace(c))
            foundWhitespace = 1;
        else if (foundWhitespace) state = ERR;
            // ERR: Whitespace between digits encountered.
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
 *  @brief Detects 4th line input type by looking for "0x" or "R" and applies
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

/**
 * Calculates a dimProduct (n_1*n_2*...*n_i - a product of known dimension
 * sizes with indexes from 1 to i). Calculated results are then memoized in
 * #inputData.
 * @param i - Highest index that occurs in the dimProduct.
 * @return A dimProduct.
 */
size_t getDimProduct(size_t i) {
    if (!(d->dimProducts)) {
        d->dimProducts = safe_calloc(d->dimNum, sizeof(size_t));
        (d->dimProducts)[0] = (d->dimensions)[0];
    }
    size_t *dp = d->dimProducts;
    if (i < 1) return 1;
    else if (dp[i - 1] != 0) return dp[i - 1];
    else {
        size_t a = getDimProduct(i - 1);
        size_t b = (d->dimensions)[i - 1];
        size_t product = a * b;

        if (a != product / b) exitWithError(1, d);

        dp[i - 1] = product;
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
