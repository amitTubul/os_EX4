#ifndef OS_EX4_REACTOR_H
#define OS_EX4_REACTOR_H

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
#include <pthread.h>

#define CONNECTIONS_NUM 2064
#define PORT 9034

typedef struct reactor reactor,*reactorPtr;

typedef void (*handler_t)(int, reactor *react);

typedef struct fd_handler{
    int fd;
    handler_t handler;
}fd_handler;

struct reactor{
    pthread_t thread;
    struct pollfd fds[CONNECTIONS_NUM];
    int size;
    int isActive;
    int listener;
    fd_handler fd_handlers[CONNECTIONS_NUM];
};


reactor* createReactor();

void startReactor(reactor *reactor);

void stopReactor(reactor* reactor);

void addFd(reactor* reactor, int fd, handler_t handler);

void waitFor(reactor* reactor);

void removeFd(reactor* reactor, int fd);


#endif