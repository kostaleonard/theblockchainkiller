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
    uint64_t decoded = 0;
    uint8_t *p = (uint8_t *)&data;
    decoded |= (uint64_t)p[0] << 56;
    decoded |= (uint64_t)p[1] << 48;
    decoded |= (uint64_t)p[2] << 40;
    decoded |= (uint64_t)p[3] << 32;
    decoded |= (uint64_t)p[4] << 24;
    decoded |= (uint64_t)p[5] << 16;
    decoded |= (uint64_t)p[6] << 8;
    decoded |= (uint64_t)p[7] << 0;
    return decoded;
}
