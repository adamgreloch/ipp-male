#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-narrowing-conversions"
#include "input.h"
#include "err.h"

static size_t dimNum = -1;

static size_t getDecimalWallsRep();

static size_t getNum(char str[], size_t i, int inputLine) {
    str[i] = ' ';
    size_t num = strtol(str, NULL, 10);
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
        if (i >= sizeof(size_t)) {
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

void getBinaryWallsRep(DA* arrayPtr) {
    size_t walls = getDecimalWallsRep();
    size_t k = 0;
    while (walls != 0) {
        if (k == sizeof(size_t))
            exitWithError(4);
        daPut(arrayPtr, k, walls % 2);
        k++;
        walls = walls / 2;
    }
}

static size_t getDecimalWallsRep() {
    char c;
    int mode = -2; // 0 for hex, 1 for R, -1 for in-between state

    while (mode < 0 && (c = getchar()) != '\n')
        if ((mode == -2 && c == '0') || (mode == -1 && c == 'x'))
            mode++;
        else if (mode == -2 && c == 'R')
            mode = 1;

    if (mode == 0)
        return getDecimalFromHex();
    else
        return getDecimalFromR();
}

size_t getDecimalFromHex() {
    char *hexFormat = NULL;
    hexFormat = (char *) malloc(sizeof(size_t));
    size_t i = 0;
    char c;

    while ((c = getchar()) != '\n') {
        hexFormat[i] = c;
        i++;
    }

    size_t decimal = strtol(hexFormat, NULL, 16);
    free(hexFormat);

    return decimal;
}

size_t getDecimalFromR() {
    // TODO dorobić R
    size_t wallConfigNum;
    uint32_t rFormat[5];
    return wallConfigNum;
}
