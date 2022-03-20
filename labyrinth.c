#include "labyrinth.h"
#include "bitTable.h"

// TODO FIFO

size_t findPathLength(DA* dimensions, size_t* startPos, size_t* endPos, unsigned char* binaryRepArray);

static void expand(size_t* currentPos);
