/**
 * @brief Runs the peer discovery server.
 */

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

int main(int argc, char **argv) {
    return_code_t return_code = SUCCESS;
    printf("Hello peer discovery server\n");
    // TODO accepts connections, adds IP info to peer list, sends current peer list to peer that just connected, closes connection with peer
    // TODO IP information in peer list expires after 60 seconds, and peers are responsible for reconnecting with the server every now and then to refresh.
    // TODO just before sending the current peer list, filter out any expired peer addresses.

    // TODO clean this code up
#ifdef _WIN32
    WSADATA wsaData; //TODO unused variable?
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed.\n");
        return 1;
    }
#endif
    int listenfd; /* listening socket */
    int connfd; /* connection socket */
    int portno; /* port to listen on */
    int clientlen; /* byte size of client's address */
    struct sockaddr_in serveraddr = {0}; /* server's addr */
    struct sockaddr_in clientaddr = {0}; /* client addr */
    struct hostent *hostp = {0}; /* client host info */
    char buf[BUFSIZ]; /* message buffer */
    char *hostaddrp; /* dotted decimal host addr string */
    int optval; /* flag value for setsockopt */
    int n; /* message byte size */

    /* check command line args */
    if (argc != 2) {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(1);
    }
    portno = atoi(argv[1]);

    /* socket: create a socket */
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd < 0) {
        printf("Failed to create socket\n");
        return_code = FAILURE_NETWORK_FUNCTION;
        goto end;
    }

    /* setsockopt: Handy debugging trick that lets 
    * us rerun the server immediately after we kill it; 
    * otherwise we have to wait about 20 secs. 
    * Eliminates "ERROR on binding: Address already in use" error. 
    */
    optval = 1;
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, 
            (const void *)&optval , sizeof(int));

    serveraddr.sin_family = AF_INET; /* we are using the Internet */
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY); /* accept reqs to any IP addr */
    serveraddr.sin_port = htons((unsigned short)portno); /* port to listen on */

    /* bind: associate the listening socket with a port */
    if (bind(listenfd, (struct sockaddr *) &serveraddr, 
        sizeof(serveraddr)) < 0) {
            return_code = FAILURE_NETWORK_FUNCTION;
            goto end;
        }

    /* listen: make it a listening socket ready to accept connection requests */
    /* allow 5 requests to queue up */ 
    if (listen(listenfd, 5) < 0) {
        return_code = FAILURE_NETWORK_FUNCTION;
        goto end;
    }

    /* main loop: wait for a connection request, echo input line, 
        then close connection. */
    clientlen = sizeof(clientaddr);
    while (1) {

        /* accept: wait for a connection request */
        connfd = accept(listenfd, (struct sockaddr *) &clientaddr, &clientlen);
        if (connfd < 0) {
            return_code = FAILURE_NETWORK_FUNCTION;
            goto end;
        }
        
        /* gethostbyaddr: determine who sent the message */
        hostp = gethostbyaddr((const char *)&clientaddr.sin_addr.s_addr, 
                sizeof(clientaddr.sin_addr.s_addr), AF_INET);
        if (hostp == NULL) {
            return_code = FAILURE_NETWORK_FUNCTION;
            goto end;
        }
        hostaddrp = inet_ntoa(clientaddr.sin_addr);
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
        
        /* write: echo the input string back to the client */
        n = send(connfd, buf, strlen(buf), 0); // TODO not sure the return code can be negative
        if (n < 0) {
            return_code = FAILURE_NETWORK_FUNCTION;
            goto end;
        }

        // TODO conditional compile
        //close(connfd);
        closesocket(connfd);
    }
end:
#ifdef _WIN32
    WSACleanup();
#endif
    return return_code;
}
