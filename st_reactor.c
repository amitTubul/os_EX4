#include "reactor.h"


void *runReactor(reactor *reactor){
    if(reactor == NULL){
        perror("reactor pointer not initialized.\n");
        EXIT_FAILURE;
    }
    
    while (reactor->isActive){   
        int poll_count = poll(reactor->fds, reactor->size, -1);

        if (poll_count == -1) {
            perror("poll");
            exit(1);
        }

        for (int i = 0; i < reactor->size && reactor->isActive; i++){
            if(reactor->fds[i].fd & POLLIN){
                reactor->fd_handlers[i].handler(reactor->fds[i].fd, reactor);
            }
        }    
    }
    return NULL;
    
}

reactor* createReactor(){
    reactor *reactorPtr = (reactor *) malloc (sizeof(reactor));
    if (reactorPtr == NULL){
        perror("Reactor allocation failiure.\n");
        EXIT_FAILURE;
    }
    reactorPtr->size = 0;
    reactorPtr->isActive = 0;
    reactorPtr->thread = 0;
    reactorPtr->listener = -1;
    printf("Allocation succesful.\n");
    return reactorPtr;

}

void startReactor(reactor* reactor){
    if(reactor == NULL){
        perror("reactor pointer not initialized.\n");
        EXIT_FAILURE;
    }

    if(reactor->isActive){
        printf("Reactor is already runnig, no action needed.\n");
        return;
    }

    if(reactor->size == 0){
        printf ("No FDs asgined to the reactor, no operations can be done.\n");
        return;
    }

    int createResult = pthread_create(&reactor->thread, NULL, runReactor, NULL);
    if (createResult != 0){
        printf("Thread creation failed. Error code: %d\n", createResult);
        EXIT_FAILURE;
    }

    reactor->isActive = 1;
    printf("Thread created successfully.\n");

}

void stopReactor(reactor* reactor){
    if(reactor == NULL){
        perror("reactor pointer not initialized.\n");
        EXIT_FAILURE;
    }

    if(!reactor->isActive){
        printf("Reactor not runnig, no action needed.\n");
        return;
    }

    reactor->isActive = 0;
    printf("Reactor thread stopped.\n");
}

void addFd(reactor* reactor, int fd, handler_t handler){
    if(reactor == NULL){
        perror("reactor pointer not initialized.\n");
        EXIT_FAILURE;
    }
    if (reactor->size == CONNECTIONS_NUM){
        printf("No more connections can be added.\n");
    }
    int posToAdd = reactor->size;
    reactor->fd_handlers[posToAdd].fd = fd;
    reactor->fd_handlers[posToAdd].handler = handler;

    reactor->fds[posToAdd].fd = fd;
    reactor->fds[posToAdd].events = POLLIN;

    reactor->size++;

}

void waitFor(reactor* reactor){
    if (reactor == NULL){
        perror("reactor pointer not initialized.\n");
        EXIT_FAILURE;
    }

    if (!reactor->isActive){
        return;
    }
    void *thread_return = NULL;

    if(pthread_join(reactor->thread, &thread_return) != 0){
        perror("Error joining the thread.\n");
        EXIT_FAILURE;
    }
    printf("Reactor thread finished\n");
}

int findPosOfFd(reactor* reactor, int fd){
    for (int i = 0; i < reactor->size; i++){
        if (reactor->fds[i].fd == fd){
            return i;
        }
    }
    return -1;
}


void removeFd(reactor* reactor, int fd){
    if (reactor == NULL){
        perror("reactor pointer not initialized.\n");
        EXIT_FAILURE;
    }

    int posOfFd = findPosOfFd(reactor, fd);

    for (int i = posOfFd; i < reactor->size -1; i++){
        reactor->fd_handlers[i] = reactor->fd_handlers[i+1];
        reactor->fds[i] = reactor->fds[i+1];
    }

    reactor->size--;
}