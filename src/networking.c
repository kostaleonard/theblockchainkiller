/**
 * @brief Contains networking functions.
 */

#include "include/networking.h"

return_code_t command_header_deserialize(
    command_header_t *command_header, unsigned char *buf, uint64_t len) {
    return FAILURE_INVALID_INPUT;
}

return_code_t command_register_peer_deserialize(
    command_register_peer_t *command_register_peer,
    unsigned char *buf,
    uint64_t len) {
    return FAILURE_INVALID_INPUT;
}

return_code_t command_send_peer_list_serialize(
    command_send_peer_list_t *command_send_peer_list,
    unsigned char **buf,
    uint64_t *len) {
    return FAILURE_INVALID_INPUT;
}
