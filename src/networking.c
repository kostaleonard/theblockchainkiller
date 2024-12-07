/**
 * @brief Contains networking functions.
 */

#include "include/endian.h"
#include "include/networking.h"

return_code_t command_header_serialize(
    command_header_t *command_header,
    unsigned char **buffer,
    uint64_t *buffer_size) {
    return_code_t return_code = SUCCESS;
    if (NULL == command_header || NULL == buffer || NULL == buffer_size) {
        return_code = FAILURE_INVALID_INPUT;
        goto end;
    }
    uint64_t size = sizeof(command_header_t);
    unsigned char *serialization_buffer = calloc(1, size);
    if (NULL == serialization_buffer) {
        return_code = FAILURE_COULD_NOT_MALLOC;
        goto end;
    }
    unsigned char *next_spot_in_buffer = serialization_buffer;
    for (size_t idx = 0; idx < COMMAND_PREFIX_LEN; idx++) {
        *next_spot_in_buffer = command_header->command_prefix[idx];
        next_spot_in_buffer++;
    }
    *(uint32_t *)next_spot_in_buffer = htonl(command_header->command);
    next_spot_in_buffer += sizeof(command_header->command);
    *(uint64_t *)next_spot_in_buffer = htobe64(command_header->command_len);
    next_spot_in_buffer += sizeof(command_header->command_len);
    *buffer = serialization_buffer;
    *buffer_size = size;
end:
    return return_code;
}

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
