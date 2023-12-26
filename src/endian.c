#include "include/endian.h"

uint64_t htobe64(uint64_t data) {
    uint64_t encoded = 0;
    uint8_t *p = (uint8_t *)&encoded;
    p[0] = data >> 56;
    p[1] = data >> 48;
    p[2] = data >> 40;
    p[3] = data >> 32;
    p[4] = data >> 24;
    p[5] = data >> 16;
    p[6] = data >> 8;
    p[7] = data >> 0;
    return encoded;
}

uint64_t betoh64(uint64_t data) {
    return htobe64(data);
}
