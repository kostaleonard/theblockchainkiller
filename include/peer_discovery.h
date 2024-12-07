/**
 * @brief Contains functions for peer discovery.
 */

#ifndef INCLUDE_PEER_DISCOVERY_H_
#define INCLUDE_PEER_DISCOVERY_H_

#include <time.h>
#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
#else
    #include <arpa/inet.h>
    #include <netdb.h>
    #include <netinet/in.h>
    #include <sys/socket.h>
    #include <sys/types.h>
    #include <unistd.h>
#endif
#include "include/linked_list.h"

/**
 * @brief Contains peer connection information.
 * 
 * @param listen_addr The peer's address. The peer is listening for connections
 * on this address.
 * @param last_connected The time at which the peer last connected to the
 * bootstrap server. The bootstrap server uses this information to filter out
 * inactive peers.
 */
typedef struct peer_info_t {
    struct sockaddr_in6 listen_addr;
    time_t last_connected;
} peer_info_t;

// TODO this function may go to networking.h or peer_info.h
int compare_peer_info_t(void *peer1, void *peer2);

// TODO docstrings
return_code_t peer_info_list_serialize(
    linked_list_t *peer_info_list,
    unsigned char **buffer,
    uint64_t *buffer_size
);

return_code_t peer_info_list_deserialize(
    linked_list_t **peer_info_list,
    unsigned char *buffer,
    uint64_t buffer_size
);

#endif  // INCLUDE_PEER_DISCOVERY_H_
