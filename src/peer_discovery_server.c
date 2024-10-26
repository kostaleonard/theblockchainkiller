/**
 * @brief Runs the peer discovery server.
 */

#include <stdbool.h>
#include <stdio.h>
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

    // TODO clean this code up
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
    int clientlen; /* byte size of client's address */
    struct sockaddr_in server_addr = {0};
    struct sockaddr_in client_addr = {0};
    // TODO gethostbyname is deprecated: https://man7.org/linux/man-pages/man3/gethostbyaddr.3.html
    struct hostent *hostp = {0}; /* client host info */
    char buf[BUFSIZ];
    char *hostaddrp; /* dotted decimal host addr string */
    int n; /* message byte size */

    
    
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

    if (bind(listen_fd, (struct sockaddr *) &server_addr, 
        sizeof(server_addr)) < 0) {
            return_code = FAILURE_NETWORK_FUNCTION;
            goto end;
        }

    if (listen(listen_fd, LISTEN_BACKLOG) < 0) {
        return_code = FAILURE_NETWORK_FUNCTION;
        goto end;
    }

    clientlen = sizeof(client_addr);
    while (true) {
        // TODO may need to multi-thread this loop? But probably for our small application, using the main thread is fine.
        int connfd = accept(listen_fd, (struct sockaddr *) &client_addr, &clientlen);
        if (connfd < 0) {
            return_code = FAILURE_NETWORK_FUNCTION;
            goto end;
        }
        
        // TODO gethostbyaddr is deprecated
        hostp = gethostbyaddr((const char *)&client_addr.sin_addr.s_addr, 
                sizeof(client_addr.sin_addr.s_addr), AF_INET);
        if (hostp == NULL) {
            return_code = FAILURE_NETWORK_FUNCTION;
            goto end;
        }
        hostaddrp = inet_ntoa(client_addr.sin_addr);
        if (hostaddrp == NULL) {
            return_code = FAILURE_NETWORK_FUNCTION;
            goto end;
        }
        printf("server established connection with %s (%s)\n", 
        hostp->h_name, hostaddrp);
        
        memset(buf, 0, BUFSIZ);
        n = recv(connfd, buf, BUFSIZ, 0); // TODO not sure the return code can be negative
        if (n < 0) {
            return_code = FAILURE_NETWORK_FUNCTION;
            goto end;
        }
        printf("server received %d bytes: %s", n, buf);
        
        n = send(connfd, buf, strlen(buf), 0); // TODO not sure the return code can be negative
        if (n < 0) {
            return_code = FAILURE_NETWORK_FUNCTION;
            goto end;
        }

        // TODO conditional compile (closesocket is Windows)
        //close(connfd);
        closesocket(connfd);
    }
end:
#ifdef _WIN32
    WSACleanup();
#endif
    return return_code;
}
