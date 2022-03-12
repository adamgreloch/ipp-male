#include "dynarray.h"
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

void getInput(DA* arrayPtr, int inputLine);
size_t getDimNum();

// setWallConfig powinno radzić sobie z dwoma sposobami reprezentacji liczby w
// czwartej linii
//
// czyli beda istniec static funkcje readHex oraz readR (TODO).
void getWallConfig(size_t wallConfigRep);

// findCubeState ustala, czy kostka o współrzędnych (z_1,z_2,...,z_k),
// zapisanych w cube[], jest pusta, czy wypełniona po znalezieniu wartości bitu
// (z_1-1) + (z_2-1)*n_1 + (z_3-1)*n_1*n_2 + ... + (z_k-1)*n_1*...*n_{k-1}.
int findCubeState(size_t cube[]);
