#include "labirynth.h"
#include "dynarray.h"

#define IN 1
#define OUT 0

void getInput(DA* arrayPtr, int inputLine);
void exitWithError(int errNum);

size_t dimNum = -1;

int main() {

    DA *dimensions, *startCords, *destCords;

    dimensions = malloc(sizeof(DA));
    dimensions->next = NULL;

    startCords = malloc(sizeof(DA));
    startCords->next = NULL;

    destCords = malloc(sizeof(DA));
    destCords->next = NULL;

    getInput(dimensions, 1);
    getInput(startCords, 2);
    getInput(destCords, 3);

    free(dimensions);
    free(startCords);
    free(destCords);

    return 0;
}

void exitWithError(int errNum) {
    fprintf(stderr, "ERROR %d\n", errNum);
    // TODO jawne zwolnienie pamięci
    exit(1);
}

void insertElem(DA* arrayPtr, char str[], size_t i, size_t k, int inputLine) {
    str[i] = ' ';
    size_t elem = strtol(str, NULL, 10);
    if (elem < 1 && inputLine == 3)
        exitWithError(3);
    daPut(arrayPtr, k, strtol(str, NULL, 10));
}

void getInput(DA* arrayPtr, int inputLine) {

    char str[sizeof(size_t)];
    int state = OUT;
    size_t i = 0, k = 0;
    char c;

    while ((c = getchar()) != '\n') {
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
        exitWithError(1);

    if (dimNum == -1)
        dimNum = k;
    else if (k != dimNum)
        // raise error if number of arguments per line is not the same
        exitWithError(inputLine);
}

