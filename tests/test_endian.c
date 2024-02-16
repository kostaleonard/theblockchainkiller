#include "include/endian.h"
#include "tests/test_endian.h"

void test_htobe64_correctly_encodes_data() {
    uint64_t data = 0x0123456789abcdef;
    uint64_t encoded = htobe64(data);
    assert_true(0x01 == ((uint8_t *)&encoded)[0]);
    assert_true(0x23 == ((uint8_t *)&encoded)[1]);
    assert_true(0x45 == ((uint8_t *)&encoded)[2]);
    assert_true(0x67 == ((uint8_t *)&encoded)[3]);
    assert_true(0x89 == ((uint8_t *)&encoded)[4]);
    assert_true(0xab == ((uint8_t *)&encoded)[5]);
    assert_true(0xcd == ((uint8_t *)&encoded)[6]);
    assert_true(0xef == ((uint8_t *)&encoded)[7]);
}

void test_betoh64_correctly_decodes_data() {
    uint64_t encoded = 0;
    ((uint8_t *)&encoded)[0] = 0x01;
    ((uint8_t *)&encoded)[1] = 0x23;
    ((uint8_t *)&encoded)[2] = 0x45;
    ((uint8_t *)&encoded)[3] = 0x67;
    ((uint8_t *)&encoded)[4] = 0x89;
    ((uint8_t *)&encoded)[5] = 0xab;
    ((uint8_t *)&encoded)[6] = 0xcd;
    ((uint8_t *)&encoded)[7] = 0xef;
    uint64_t data = betoh64(encoded);
    assert_true(0x0123456789abcdef == data);
}
