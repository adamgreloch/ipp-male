/**
 * @file bitTable.h
 * @brief Set of procedures for accessing and operating on tables that serve
 * purpose as a bit storage (hereinafter bitTables).
 */

#ifndef MALE_BITTABLE_H
#define MALE_BITTABLE_H

#include "input.h"
#include <stdio.h>
#include <stdint.h>

/**
 * Sets a specified bit to a given value.
 * @warn Assumes bitValue is correct (it is either 0 or 1). Assumes arrayPtr
 * is non-NULL.
 * @param arrayPtr - BitTable pointer.
 * @param bitIndex - Index of bit that is being modified.
 * @param bitValue - New bit value.
 */
void setBit(uint8_t **arrayPtr, size_t bitIndex, int bitValue);

/**
 * Gets a value of a specifed bit.
 * @warn Returns -1 if bitIndex is out of bitTable bounds. Assumes arrayPtr
 * is non-NULL.
 * @param arrayPtr - BitTable pointer.
 * @param bitIndex - Index of bit that is being accessed.
 * @param d - Input data pointer..
 * @return Bit value.
 */
int getBit(uint8_t *arrayPtr, size_t bitIndex, inputData *d);

/**
 * Fills 4 bits of one cell with binary representation of hexValue in
 * reversed order.
 * @warn Assumes arrayPtr is non-NULL.
 * @param arrayPtr - BitTable pointer.
 * @param valueIndex - Index of hex value that is being written.
 * @param hexValue - Hex value.
 */
void setRevBitsFromHex(uint8_t **arrayPtr, size_t valueIndex, int hexValue);

/**
 * Lights up every j-th bit such that j mod 2^32 = w_i.
 * @param arrayPtr - BitTable pointer.
 * @param w - A particular w_i parameter.
 * @param d - Input data pointer..
 */
void setBitsFromR(uint8_t **arrayPtr, size_t w, inputData *d);

#endif //MALE_BITTABLE_H