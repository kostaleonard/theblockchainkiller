#include <stdio.h>
#include <stdbool.h>
#include "include/networking.h"
#include "tests/test_networking.h"

void test_command_header_serialize_fails_on_invalid_inputs() {
    command_header_t command_header = COMMAND_HEADER_INITIALIZER;
    command_header.command = COMMAND_REGISTER_PEER;
    command_header.command_len = 17;
    unsigned char *buffer = NULL;
    uint64_t buffer_size = 0;
    return_code_t return_code = command_header_serialize(
        NULL, &buffer, &buffer_size);
    assert_true(FAILURE_INVALID_INPUT == return_code);
    return_code = command_header_serialize(
        &command_header, NULL, &buffer_size);
    assert_true(FAILURE_INVALID_INPUT == return_code);
    return_code = command_header_serialize(
        &command_header, &buffer, NULL);
    assert_true(FAILURE_INVALID_INPUT == return_code);
}

void test_command_header_serialize_creates_nonempty_buffer() {
    command_header_t command_header = COMMAND_HEADER_INITIALIZER;
    command_header.command = COMMAND_REGISTER_PEER;
    command_header.command_len = 17;
    unsigned char *buffer = NULL;
    uint64_t buffer_size = 0;
    return_code_t return_code = command_header_serialize(
        &command_header, &buffer, &buffer_size);
    assert_true(SUCCESS == return_code);
    assert_true(NULL != buffer);
    assert_true(0 != buffer_size);
    unsigned char *empty_buffer = calloc(buffer_size, 1);
    assert_true(0 != memcmp(buffer, empty_buffer, buffer_size));
    // TODO remove debugging
    printf("Size of header: %lld\n", sizeof(command_header_t));
    printf("Size of buffer: %lld\n", buffer_size);
    for (size_t idx = 0; idx < buffer_size; idx++) {
        printf("%02hhx ", buffer[idx]);
    }
    printf("\n");
    free(buffer);
    free(empty_buffer);
}

void test_command_header_deserialize_reconstructs_list() {
    assert_true(false);
}

void test_command_header_deserialize_fails_on_read_past_buffer() {
    assert_true(false);
}

void test_command_header_deserialize_fails_on_invalid_input() {
    assert_true(false);
}
