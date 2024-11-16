/**
 * @brief Contains networking functions.
 */

#ifndef INCLUDE_NETWORKING_H_
#define INCLUDE_NETWORKING_H_

#define COMMAND_PREFIX "LEO:"
#define COMMAND_PREFIX_LEN 4
#define COMMAND_ERROR_MESSAGE_LEN 256

typedef enum command_t {
    COMMAND_OK,
    COMMAND_ERROR,
    COMMAND_REGISTER_PEER,
    COMMAND_SEND_PEER_LIST,
} command_t;

typedef struct command_header_t {
    char command_prefix[COMMAND_PREFIX_LEN];
    uint16_t command;
    uint64_t command_len; // TODO this does not include the header length.
} command_header_t;

typedef struct command_ok_t {
    command_header_t header;
} command_ok_t;

typedef struct command_error_t {
    command_header_t header;
    char message[COMMAND_ERROR_MESSAGE_LEN];
} command_error_t;

typedef struct command_register_peer_t {
    command_header_t header;
    uint16_t sin6_family;
    uint16_t sin6_port;
    uint32_t sin6_flowinfo;
    unsigned char addr[sizeof(IN6_ADDR)];
    uint32_t sin6_scope_id;
} command_register_peer_t;

typedef struct command_send_peer_list_t {
    command_header_t header;
    uint64_t peer_list_data_len;
    unsigned char *peer_list_data;
} command_send_peer_list_t;

// TODO order these functions based on the command_t enum.
// TODO check return_codes.h for which error codes are needed to communicate what went wrong

return_code_t command_header_deserialize(
    command_header_t *command_header, unsigned char *buf, size_t len);

/**
 * @brief Attempts to deserialize a register peer command from the buffer.
 * 
 * @param command_register_peer A pointer to fill with the deserialized register
 * peer command data. If the buffer contains a valid register peer command, this
 * pointer will be filled with data and the function will return SUCCESS.
 * @param buf The buffer. The data in the buffer is in network byte order.
 * @param len The length of the buffer.
 * @return return_code_t A return code indicating success or failure.
 */
return_code_t command_register_peer_deserialize(
    command_register_peer_t *command_register_peer,
    unsigned char *buf,
    size_t len);

return_code_t command_send_peer_list_serialize(
    command_send_peer_list_t *command_send_peer_list,
    unsigned char **buf,
    size_t *len);

return_code_t command_error_serialize(
    command_error_t *command_error,
    unsigned char **buf,
    size_t *len);

#endif  // INCLUDE_NETWORKING_H_
