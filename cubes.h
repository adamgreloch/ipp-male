/**
 * @file cubes.h
 * @brief Set of procedures for cube manipulation and identification.
 */

#ifndef MALE_CUBES_H
#define MALE_CUBES_H

#include "input.h"
#include "stdint.h"

/**
 * @brief Calculates rank for given (z_1, z_2, ..., z_k) cube.
 *
 * Formula: rank(z_1,z_2,...,z_k) = (z_1-1)+(z_2-1)n_1 + (z_3-1)n_1n_2 + ...
 * + (z_k-1)n_1n_2...n_{k-1}.
 * @param cube - Array of cube coordinates (z_1, z_2, ..., z_k).
 * @param d - Pointer to input data.
 * @return Rank of cube
 */
size_t rankCube(size_t *cube, inputData *d);

/**
 * @brief Calculates coordinate array for a given cube from its rank. Inverse
 * function to rankCube().
 * @param cubeRank - Rank of cube.
 * @param d - Pointer to input data.
 * @return (z_1, z_2, ..., z_k) coordinates of cube.
 */
size_t *unrankCube(size_t cubeRank, inputData *d);

/**
 * @brief Moves from one cube to another in one dimension by a integer steps by
 * using only their ranks and calculates rank of the destination.
 * @param cubeRank - Rank of cube.
 * @param dim - Dimension of movement.
 * @param steps - Numbers of steps to be taken.
 * @return Destination cube rank.
 */
size_t moveRank(size_t cubeRank, size_t dim, int steps);

/**
 * @brief Gets maximum value of rank function for given input data.
 * @param d - Pointer to input data.
 * @return Rank function maximum.
 */
size_t getMaxRank(inputData *d);

#endif //MALE_CUBES_H