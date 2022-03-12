#include "input.h"
#include "err.h"

#define IN 1
#define OUT 0

size_t dimNum = -1;

static void insertElem(DA* arrayPtr, char str[], size_t i, size_t k, int inputLine) {
    str[i] = ' ';
    size_t elem = strtol(str, NULL, 10);
    if (elem < 1)
        // input error: numbers have to be positive
        exitWithError(inputLine);
    daPut(arrayPtr, k, strtol(str, NULL, 10));
}

void getInput(DA* arrayPtr, int inputLine) {

    char str[sizeof(size_t)];
    int state = OUT;
    size_t i = 0, k = 0;
    char c;

    while ((c = getchar()) != '\n') {
        if (i >= sizeof(size_t))
            // input error: labyrinth too big
            exitWithError(0);
        // TODO refactor
        if (state == IN) {
            if (c != ' ') {
                str[i] = c;
                i++;
            } else {
                state = OUT;
                insertElem(arrayPtr, str, i, k, inputLine);
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
        insertElem(arrayPtr, str, i, k, inputLine);
        k++;
    }

    if (k == 0)
        // input error: labyrinth dimension must be non-zero
        exitWithError(1);

    if (dimNum == -1)
        dimNum = k;
    else if (k != dimNum)
        // input error: number of arguments is mismatched throughout the input lines
        exitWithError(inputLine);
}

size_t getDimNum() {
    return dimNum;
}

void getWallConfig() {
    size_t wallConfigNum;
    int mode = -1; // 0 for hex, 1 for R
    char c;

    while ((c=getchar()) != '\n') {
        if (mode == -1 && c == 'R')
            mode = 1;

        if (mode == 1) {

        }
    }


    scanf("%x", &wallConfigNum);
}

