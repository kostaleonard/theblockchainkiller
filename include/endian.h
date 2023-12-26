/**
 * @brief Defines functions for working with endianness.
 */

#ifndef INCLUDE_ENDIAN_H_
#define INCLUDE_ENDIAN_H_
#include <stdint.h>

/**
 * @brief Returns the big endian representation of data.
 * 
 * @param data A 64 bit piece of data. Its endianness does not matter.
 * @return uint64_t The big endian representation of data.
 */
uint64_t htobe64(uint64_t data);

/**
 * @brief Returns the host representation of big endian data.
 * 
 * @param data A 64 bit piece of data in big endian representation.
 * @return uint64_t The host representation of data.
 */
uint64_t betoh64(uint64_t data);

#endif  // INCLUDE_ENDIAN_H_
