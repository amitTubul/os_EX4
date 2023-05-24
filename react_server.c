#include "reactor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <poll.h>

// Get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

// Return a listening socket
int get_listener_socket() {
    int listenerSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (listenerSocket == -1) {
        perror("Error creating listener socket");
        exit(1);
    }

    // Set up the server address
    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(PORT); // Change port number if needed

    // Bind the socket to the server address
    if (bind(listenerSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1) {
        perror("Error binding listener socket");
        exit(1);
    }

    // Start listening for incoming connections
    if (listen(listenerSocket, 10) == -1) {
        perror("Error listening on listener socket");
        exit(1);
    }

    printf("Listener socket created and set up successfully.\n");

    return listenerSocket;
}

void clientHandler(int fd, reactor *reactor){
    char buf[1024];
    int nbytes = recv(fd, buf, sizeof buf, 0);

    if (nbytes <= 0) {
        // Got error or connection closed by client
        if (nbytes == 0) {
            // Connection closed
            printf("pollserver: socket %d hung up\n", fd);
        } else {
            perror("recv");
        }
        close(fd); // Bye!
        removeFd(reactor, fd);
        exit(1);
    }
    buf[nbytes] = '\0';
    printf("Eecived: %s\n", buf);
}

void listenerHandler(int fd, reactor *reactor){
    int newfd;      // Newly accept()ed socket descriptor
    struct sockaddr_storage remoteaddr; // Client address
    socklen_t addrlen;

    char remoteIP[INET6_ADDRSTRLEN];
    addrlen = sizeof remoteaddr;
    newfd = accept(fd,(struct sockaddr *)&remoteaddr,&addrlen);
    if (newfd == -1) {
        perror("accept");
        exit(1);
    } 
    addFd(reactor, newfd, &clientHandler);
    printf("pollserver: new connection from %s on "
                            "socket %d\n",
                            inet_ntop(remoteaddr.ss_family,
                            get_in_addr((struct sockaddr*)&remoteaddr),
                            remoteIP, INET6_ADDRSTRLEN),
                            newfd);
}

int main(){
    int listener;     // Listening socket descriptor

    // Set up and get a listening socket
    listener = get_listener_socket();

    if (listener == -1) {
        fprintf(stderr, "error getting listening socket\n");
        exit(1);
    }

    reactor *reactor = createReactor();
    reactor->listener = listener;
    addFd(reactor, listener, &listenerHandler);

    startReactor(reactor);
    waitFor(reactor);
    
}