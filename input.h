#include "dynarray.h"
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

void getFirstInput(DA* arrayPtr, int inputLine);
size_t* getInput(int inputLine);
size_t getDimNum();

size_t getDecimalFromHex();
size_t getDecimalFromR();

#define IN 1
#define OUT 0

// setWallConfig powinno radzić sobie z dwoma sposobami reprezentacji liczby w
// czwartej linii
void getBinaryWallsRep(DA* arrayPtr);
