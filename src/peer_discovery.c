/**
 * @brief Contains functions for peer discovery.
 */

#include "include/peer_discovery.h"

int compare_peer_info_t(void *peer1, void *peer2) {
    if (NULL == peer1 || NULL == peer2) {
        return 0;
    }
    peer_info_t *p1 = (peer_info_t *)peer1;
    peer_info_t *p2 = (peer_info_t *)peer2;
    return memcmp(
        &p1->listen_addr,
        &p2->listen_addr,
        sizeof(struct sockaddr_in6));
}

return_code_t peer_info_list_serialize(
    linked_list_t *peer_info_list,
    unsigned char **buffer,
    uint64_t *buffer_size
) {
    return_code_t return_code = SUCCESS;
    if (NULL == peer_info_list || NULL == buffer || NULL == buffer_size) {
        return_code = FAILURE_INVALID_INPUT;
        goto end;
    }
    // TODO
end:
    return return_code;
}
