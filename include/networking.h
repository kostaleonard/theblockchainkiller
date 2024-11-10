/**
 * @brief Contains networking functions.
 */

#ifndef INCLUDE_NETWORKING_H_
#define INCLUDE_NETWORKING_H_

#define COMMAND_PREFIX "LEO:"
#define COMMAND_PREFIX_LEN 4
#define COMMAND_ERROR_MESSAGE_LEN 64
#define COMMAND_SEND_PEER_LIST_PEER_LIST_LEN 504

typedef enum command_t {
    COMMAND_OK,
    COMMAND_ERROR,
    COMMAND_REGISTER_PEER,
    COMMAND_SEND_PEER_LIST,
} command_t;

typedef struct command_ok_t {
    char command_prefix[COMMAND_PREFIX_LEN];
    uint16_t command;
} command_ok_t;

typedef struct command_error_t {
    char command_prefix[COMMAND_PREFIX_LEN];
    uint16_t command;
    char message[COMMAND_ERROR_MESSAGE_LEN];
} command_error_t;

typedef struct command_register_peer_t {
    char command_prefix[COMMAND_PREFIX_LEN];
    uint16_t command;
    uint16_t sin6_family;
    uint16_t sin6_port;
    uint32_t sin6_flowinfo;
    unsigned char addr[sizeof(IN6_ADDR)];
    uint32_t sin6_scope_id;
} command_register_peer_t;

typedef struct command_send_peer_list_t {
    char command_prefix[COMMAND_PREFIX_LEN];
    uint16_t command;
    uint16_t peer_list_len;
    unsigned char peer_list[COMMAND_SEND_PEER_LIST_PEER_LIST_LEN];
} command_send_peer_list_t;

/**
 * @brief Attempts to deserialize a register peer command from the buffer.
 * 
 * @param buf The buffer. The data in the buffer is in network byte order.
 * @param len The length of the buffer.
 * @param command_register_peer A pointer to fill with the deserialized register
 * peer command data. If the buffer contains a valid register peer command, this
 * pointer will be filled with data and the function will return SUCCESS.
 * @param command_error A pointer to fill with an error command on unsuccessful
 * deserialization. If the buffer does not contain a valid register peer
 * command, this pointer will be filled with data and the function will return
 * FAILURE_INVALID_COMMAND.
 * @return return_code_t A return code indicating success or failure.
 */
return_code_t deserialize_command_register_peer(
    char *buf,
    size_t len,
    command_register_peer_t *command_register_peer,
    command_error_t *command_error);

#endif  // INCLUDE_NETWORKING_H_
