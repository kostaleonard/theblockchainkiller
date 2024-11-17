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
