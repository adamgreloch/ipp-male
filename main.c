#include "input.h"
#include "errMem.h"
#include "labyrinth.h"
#include <stdint.h>

int main() {

    inputData *d = getInputData();

    int64_t pathLength = findPath(d);

    if (pathLength == -1)
        printf("NO WAY\n");
    else
        printf("%d\n", pathLength);

    freeData(d);

    return 0;
}
