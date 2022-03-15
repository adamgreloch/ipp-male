#include "main.h"

int main() {

    DA *dimensions;
    dimensions = malloc(sizeof(DA));
    dimensions->next = NULL;

    getFirstInput(dimensions, 1);

    size_t* startPos = getInput(2);
    size_t* endPos = getInput(3);

    unsigned char* binaryRepArray = getBinaryWallsRep();

    if (!binaryRepArray)
        // memory error: malloc failed
        exitWithError(0);

/*
    if (getchar() != EOF) {
        // input error: too many input lines
        free(dimensions);
        free(binaryRepArray);
        exitWithError(5);
    }
*/

    int twoBit;
    int invArr[8];
    for (int i = 0; i < 100; i++) {
        twoBit = binaryRepArray[i];
        for (int j = 0; j < 8; j++) {
            invArr[8-j-1] = twoBit % 2;
            twoBit /= 2;
        }
        for (int j = 0; j < 8; j++)
            if (invArr[j])
                printf("\033[0;32m1");
            else
                printf("\033[0;37m0");
    }

/*
    if (findCubeState(startPos, dimensions, binaryRepArray)) {
        // input error: startPos filled
        free(dimensions);
        free(binaryRepArray);
        exitWithError(2);
    }
*/

    free(startPos);
    free(endPos);
    free(dimensions);
    free(binaryRepArray);

    return 0;
}
