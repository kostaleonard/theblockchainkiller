#include <stdio.h>
#include <stdbool.h>
#include "include/networking.h"
#include "tests/test_networking.h"

void test_command_header_serialize_fails_on_invalid_input() {
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

void test_command_header_serialize_fails_on_invalid_prefix() {
    command_header_t command_header = COMMAND_HEADER_INITIALIZER;
    command_header.command_prefix[2] = 'A';
    command_header.command = COMMAND_REGISTER_PEER;
    command_header.command_len = 17;
    unsigned char *buffer = NULL;
    uint64_t buffer_size = 0;
    return_code_t return_code = command_header_serialize(
        &command_header, &buffer, &buffer_size);
    assert_true(FAILURE_INVALID_COMMAND_PREFIX == return_code);
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
    free(buffer);
    free(empty_buffer);
}

void test_command_header_deserialize_reconstructs_command() {
    command_header_t command_header = COMMAND_HEADER_INITIALIZER;
    command_header.command = COMMAND_REGISTER_PEER;
    command_header.command_len = 17;
    unsigned char *buffer = NULL;
    uint64_t buffer_size = 0;
    return_code_t return_code = command_header_serialize(
        &command_header, &buffer, &buffer_size);
    assert_true(SUCCESS == return_code);
    command_header_t deserialized_command_header = {0};
    return_code = command_header_deserialize(
        &deserialized_command_header, buffer, buffer_size);
    assert_true(SUCCESS == return_code);
    assert_true(0 == memcmp(
        &command_header,
        &deserialized_command_header,
        sizeof(command_header_t)));
    free(buffer);
}

void test_command_header_deserialize_fails_on_read_past_buffer() {
    command_header_t command_header = COMMAND_HEADER_INITIALIZER;
    command_header.command = COMMAND_REGISTER_PEER;
    command_header.command_len = 17;
    unsigned char *buffer = NULL;
    uint64_t buffer_size = 0;
    return_code_t return_code = command_header_serialize(
        &command_header, &buffer, &buffer_size);
    assert_true(SUCCESS == return_code);
    command_header_t deserialized_command_header = {0};
    return_code = command_header_deserialize(
        &deserialized_command_header, buffer, buffer_size - 4);
    assert_true(FAILURE_BUFFER_TOO_SMALL == return_code);
    free(buffer);
}

void test_command_header_deserialize_fails_on_invalid_prefix() {
    command_header_t command_header = COMMAND_HEADER_INITIALIZER;
    command_header.command = COMMAND_REGISTER_PEER;
    command_header.command_len = 17;
    unsigned char *buffer = NULL;
    uint64_t buffer_size = 0;
    return_code_t return_code = command_header_serialize(
        &command_header, &buffer, &buffer_size);
    assert_true(SUCCESS == return_code);
    buffer[2] = 'A';
    command_header_t deserialized_command_header = {0};
    return_code = command_header_deserialize(
        &deserialized_command_header, buffer, buffer_size);
    assert_true(FAILURE_INVALID_COMMAND_PREFIX == return_code);
    free(buffer);
}

void test_command_header_deserialize_fails_on_invalid_input() {
    command_header_t command_header = COMMAND_HEADER_INITIALIZER;
    command_header.command = COMMAND_REGISTER_PEER;
    command_header.command_len = 17;
    unsigned char *buffer = NULL;
    uint64_t buffer_size = 0;
    return_code_t return_code = command_header_serialize(
        &command_header, &buffer, &buffer_size);
    assert_true(SUCCESS == return_code);
    command_header_t deserialized_command_header = {0};
    return_code = command_header_deserialize(
        NULL, buffer, buffer_size);
    assert_true(FAILURE_INVALID_INPUT == return_code);
    return_code = command_header_deserialize(
        &deserialized_command_header, NULL, buffer_size);
    assert_true(FAILURE_INVALID_INPUT == return_code);
    free(buffer);
}
