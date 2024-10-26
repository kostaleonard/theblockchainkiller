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
#include "include/return_codes.h"

#define LISTEN_BACKLOG 5

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
        char buf[BUFSIZ] = {0};
        // TODO handle partial read/write?
        int message_len = recv(conn_fd, buf, BUFSIZ, 0);
        if (message_len < 0) {
            return_code = FAILURE_NETWORK_FUNCTION;
            goto end;
        }
        printf("Server received %d bytes: %s", message_len, buf);
        // TODO handle partial read/write?
        message_len = send(conn_fd, buf, strlen(buf), 0);
        if (message_len < 0) {
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
