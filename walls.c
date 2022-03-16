#include "walls.h"
#include "bitTable.h"

int findCubeState(size_t cube[], DA* dimensionsPtr, unsigned char* binaryRepArrayPtr) {
    size_t sum = 0, product = 1;
    for (int k = 0; k < getDimNum(); k++) {
        product = 1;
        for (int i = 0; i < k; i++)
            product *= daGet(dimensionsPtr, i);
        sum += (cube[k] - 1) * product;
    }
    return getBit(binaryRepArrayPtr, getMaxBitLength() - 1 - sum);
}