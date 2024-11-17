#include <stdio.h>
#include <stdbool.h>
#include "include/peer_discovery.h"
#include "tests/test_peer_discovery.h"

void test_compare_peer_info_t_compares_ip_addresses() {
    peer_info_t peer1 = {0};
    peer1.listen_addr.sin6_family = AF_INET6;
    peer1.listen_addr.sin6_port = 12345;
    peer1.listen_addr.sin6_flowinfo = 0;
    // ::1 is the loopback address.
    // The cast is the easiest way to make this code portable to Windows.
    ((unsigned char *)(&peer1.listen_addr.sin6_addr))[sizeof(IN6_ADDR) - 1] = 1;
    peer1.listen_addr.sin6_scope_id = 0;
    peer1.last_connected = 100;
    peer_info_t peer2 = {0};
    peer2.listen_addr.sin6_family = AF_INET6;
    peer2.listen_addr.sin6_port = 12345;
    peer2.listen_addr.sin6_flowinfo = 0;
    ((unsigned char *)(&peer2.listen_addr.sin6_addr))[sizeof(IN6_ADDR) - 1] = 1;
    peer2.listen_addr.sin6_scope_id = 0;
    peer2.last_connected = 200;
    assert_true(0 == compare_peer_info_t(&peer1, &peer2));
    // fe80::1 is a link local address.
    ((unsigned char *)(&peer2.listen_addr.sin6_addr))[0] = 0xfe;
    ((unsigned char *)(&peer2.listen_addr.sin6_addr))[1] = 0x80;
    assert_true(0 != compare_peer_info_t(&peer1, &peer2));
}

void test_peer_info_list_serialize_fails_on_invalid_inputs() {
    linked_list_t *peer_info_list = NULL;
    return_code_t return_code = linked_list_create(
        &peer_info_list, free, compare_peer_info_t);
    assert_true(SUCCESS == return_code);
    unsigned char *buffer = NULL;
    uint64_t buffer_size = 0;
    return_code = peer_info_list_serialize(NULL, &buffer, &buffer_size);
    assert_true(FAILURE_INVALID_INPUT == return_code);
    return_code = peer_info_list_serialize(peer_info_list, NULL, &buffer_size);
    assert_true(FAILURE_INVALID_INPUT == return_code);
    return_code = peer_info_list_serialize(peer_info_list, &buffer, NULL);
    assert_true(FAILURE_INVALID_INPUT == return_code);
    linked_list_destroy(peer_info_list);
}

void test_peer_info_list_serialize_creates_nonempty_buffer() {
    linked_list_t *peer_info_list = NULL;
    return_code_t return_code = linked_list_create(
        &peer_info_list, free, compare_peer_info_t);
    assert_true(SUCCESS == return_code);
    unsigned char *buffer = NULL;
    uint64_t buffer_size = 0;
    return_code = peer_info_list_serialize(
        peer_info_list, &buffer, &buffer_size);
    assert_true(SUCCESS == return_code);
    assert_true(NULL != buffer);
    assert_true(0 != buffer_size);
    unsigned char *empty_buffer = calloc(buffer_size, 1);
    assert_true(0 != memcmp(buffer, empty_buffer, buffer_size));
    free(buffer);
    free(empty_buffer);
    linked_list_destroy(peer_info_list);
}

void test_peer_info_list_deserialize_reconstructs_list() {
    // TODO
}

void test_peer_info_list_deserialize_fails_on_attempted_read_past_buffer() {
    // TODO
}

void test_peer_info_list_deserialize_fails_on_invalid_input() {
    // TODO
}
