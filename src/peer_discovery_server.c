/**
 * @brief Runs the peer discovery server.
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <unistd.h> // TODO is this available on Windows in general?
// TODO the only reason I support cross-platform builds is to be able to test locally without using a container. Is there a less messy way to do that?
#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
#else
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
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
    struct sockaddr_in server_addr = {0};
    struct sockaddr_in client_addr = {0};
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
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
        sizeof(int))) {
        return_code = FAILURE_NETWORK_FUNCTION;
        goto end;
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons((unsigned short)port);
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
        // TODO may need to multi-thread this loop? But probably for our small application, using the main thread is fine.
        int client_len = sizeof(client_addr);
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
        char client_addr_str[INET_ADDRSTRLEN] = {0};
        if (NULL == inet_ntop(AF_INET, &client_addr.sin_addr, client_addr_str, INET_ADDRSTRLEN)) {
            return_code = FAILURE_NETWORK_FUNCTION;
            goto end;
        }
        printf("Server established connection with %s (%s)\n", client_hostname, client_addr_str);
        char buf[BUFSIZ] = {0};
        // TODO handle partial read/write
        int message_len = recv(conn_fd, buf, BUFSIZ, 0); // TODO not sure the return code can be negative
        if (message_len < 0) {
            return_code = FAILURE_NETWORK_FUNCTION;
            goto end;
        }
        printf("Server received %d bytes: %s", message_len, buf);
        // TODO handle partial read/write
        message_len = send(conn_fd, buf, strlen(buf), 0); // TODO not sure the return code can be negative
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
