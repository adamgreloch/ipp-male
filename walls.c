#include "walls.h"

int findCubeState(size_t* cube, DA* dimensionsPtr, DA* binaryRepArrayPtr) {
    size_t dim = getDimNum();
    size_t sum = 0, product = 1;
    for (int k = 0; k < dim; k++) {
        product = 1;
        for (int i = 0; i < k; i++)
            product *= daGet(dimensionsPtr, i);
        sum += (cube[k] - 1) * product;
    }
    return daGet(binaryRepArrayPtr, sum);
}
