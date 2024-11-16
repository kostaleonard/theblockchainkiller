/**
 * @brief Runs the peer discovery bootstrap server.
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
#include "include/networking.h"
#include "include/peer_discovery_bootstrap_server.h"
#include "include/return_codes.h"

#define LISTEN_BACKLOG 5

int compare_peer_info_t(void *peer1, void *peer2) {
    // TODO test
    // TODO check for null pointers?
    peer_info_t *p1 = (peer_info_t *)peer1;
    peer_info_t *p2 = (peer_info_t *)peer2;
    return memcmp(
        &p1->listen_addr,
        &p2->listen_addr,
        sizeof(struct sockaddr_in6));
}

int main(int argc, char **argv) {
    return_code_t return_code = SUCCESS;
    printf("Hello peer discovery server\n");
    // TODO accepts connections, adds IP info to peer list, sends current peer list to peer that just connected, closes connection with peer
    // TODO IP information in peer list expires after 60 seconds, and peers are responsible for reconnecting with the server every now and then to refresh.
    // TODO just before sending the current peer list, filter out any expired peer addresses.
    if (2 != argc) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        return_code = FAILURE_INVALID_COMMAND_LINE_ARGS;
        goto end;
    }
    linked_list_t *peer_list = NULL;
    return_code = linked_list_create(&peer_list, free, compare_peer_info_t);
    if (SUCCESS != return_code) {
        goto end;
    }
    int port = strtol(argv[1], NULL, 10);
    #ifdef _WIN32
        WSADATA wsaData;
        if (0 != WSAStartup(MAKEWORD(2, 2), &wsaData)) {
            return_code = FAILURE_NETWORK_FUNCTION;
            goto end;
        }
    #endif
    struct sockaddr_in6 server_addr = {0};
    struct sockaddr_in6 client_addr = {0};
    int listen_fd = socket(AF_INET6, SOCK_STREAM, 0);
    if (listen_fd < 0) {
        return_code = FAILURE_NETWORK_FUNCTION;
        goto end;
    }
    int optval = 1;
    if (0 != setsockopt(
        listen_fd,
        SOL_SOCKET,
        SO_REUSEADDR,
        (const void *)&optval,
        sizeof(optval))) {
        return_code = FAILURE_NETWORK_FUNCTION;
        goto end;
    }
    int ipv6_v6only = 1;
    if (0 != setsockopt(
        listen_fd,
        IPPROTO_IPV6,
        IPV6_V6ONLY,
        (const void *)&ipv6_v6only,
        sizeof(ipv6_v6only))) {
        return_code = FAILURE_NETWORK_FUNCTION;
        goto end;
    }
    server_addr.sin6_family = AF_INET6;
    server_addr.sin6_addr = in6addr_any;
    server_addr.sin6_port = htons((unsigned short)port);
    if (bind(
        listen_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        return_code = FAILURE_NETWORK_FUNCTION;
        goto end;
    }
    if (listen(listen_fd, LISTEN_BACKLOG) < 0) {
        return_code = FAILURE_NETWORK_FUNCTION;
        goto end;
    }    
    while (true) {
        socklen_t client_len = sizeof(client_addr);
        int conn_fd = accept(listen_fd, (struct sockaddr *)&client_addr, &client_len);
        if (conn_fd < 0) {
            return_code = FAILURE_NETWORK_FUNCTION;
            goto end;
        }
        if (sizeof(client_addr) != client_len) {
            return_code = FAILURE_BUFFER_TOO_SMALL;
            goto end;
        }
        char client_hostname[NI_MAXHOST] = {0};
        if (0 != getnameinfo((struct sockaddr *)&client_addr, client_len, client_hostname, NI_MAXHOST, NULL, 0, 0)) {
            return_code = FAILURE_NETWORK_FUNCTION;
            goto end;
        }
        char client_addr_str[INET6_ADDRSTRLEN] = {0};
        if (NULL == inet_ntop(AF_INET6, &client_addr.sin6_addr, client_addr_str, INET6_ADDRSTRLEN)) {
            return_code = FAILURE_NETWORK_FUNCTION;
            goto end;
        }
        printf("Server established connection with %s (%s)\n", client_hostname, client_addr_str);
        char recv_buf[BUFSIZ] = {0};
        // TODO handle partial read/write?
        // TODO need timeout or new thread to prevent resources getting taken up
        int bytes_received = recv(conn_fd, recv_buf, BUFSIZ, 0);
        if (bytes_received < 0) {
            return_code = FAILURE_NETWORK_FUNCTION;
            goto end;
        }
        printf("Server received %d bytes: %s\n", bytes_received, recv_buf);
        // TODO call deserialize_command_register_peer
        // TODO send back peer list or error message


        // TODO malloc/realloc as the message gets filled in to handle messages longer than BUFSIZ
        char send_buf[BUFSIZ] = {0};
        size_t send_message_len = 0;
        size_t command_prefix_len = strlen(COMMAND_PREFIX);
        send_message_len += command_prefix_len;
        strncpy(send_buf, COMMAND_PREFIX, command_prefix_len);
        if (0 != strncmp(recv_buf, COMMAND_PREFIX, command_prefix_len)) {
            printf("Invalid command prefix\n");
            send_buf[send_message_len] = COMMAND_ERROR;
            send_message_len++;
        } else {
            size_t recv_buf_idx = command_prefix_len;
            uint16_t command = ntohs(*(uint16_t *)(recv_buf + recv_buf_idx));
            if (COMMAND_REGISTER_PEER != command) {
                printf("Received command %d, but expected command %d\n", command, COMMAND_REGISTER_PEER);
                send_buf[send_message_len] = COMMAND_ERROR;
                send_message_len++;
            } else {
                peer_info_t *peer_info = malloc(sizeof(peer_info_t));
                if (NULL == peer_info) {
                    return_code = FAILURE_COULD_NOT_MALLOC;
                    goto end;
                }

            }
        }
        
        // TODO handle partial read/write?
        int bytes_sent = send(conn_fd, recv_buf, strlen(recv_buf), 0);
        if (bytes_sent < 0) {
            return_code = FAILURE_NETWORK_FUNCTION;
            goto end;
        }
        # ifdef _WIN32
            closesocket(conn_fd);
        # else
            close(conn_fd);
        # endif
    }
    #ifdef _WIN32
        closesocket(listen_fd);
        WSACleanup();
    # else
        close(listen_fd);
    #endif
end:
    return return_code;
}
