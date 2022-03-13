#include "labyrinth.h"
#include "walls.h"

int main() {

    DA *dimensions, *binaryRepArray;
    daInit(dimensions);
    daInit(binaryRepArray);

    getFirstInput(dimensions, 1);

    size_t* startPos = getInput(2);
    size_t* endPos = getInput(3);

    getBinaryWallsRep(binaryRepArray);

    if (getchar() != EOF) {
        // input error: too many input lines
        free(dimensions);
        free(binaryRepArray);
        exitWithError(5);
    }
    if (findCubeState(startPos, dimensions, binaryRepArray)) {
        // input error: startPos filled
        free(dimensions);
        free(binaryRepArray);
        exitWithError(2);
    }

    free(startPos);
    free(endPos);
    free(dimensions);
    free(binaryRepArray);

    return 0;
}
