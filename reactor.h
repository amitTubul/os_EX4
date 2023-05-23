#ifndef OS_EX4_REACTOR_H
#define OS_EX4_REACTOR_H

#include <stdio.h>
#include <stdlib.h>


#define HASHMAP_SIZE 10

// Define the function signature for the function stored in the hash map
typedef void (*handler_t)(int);

// Define the key-value pair struct
typedef struct {
    int key;
    FunctionPtr value;
} KeyValuePair;

// Define the hash map struct
typedef struct {
    KeyValuePair** data;
    int capacity;
} HashMap;

// Hash function
int hash(int key, int capacity);

// Create a new key-value pair
KeyValuePair* create_pair(int key,  handler_t handler);

// Initialize the hash map
HashMap* create_hashmap(int capacity) ;

// Insert a key-value pair into the hash map
void insert(HashMap* map, int key, handler_t handler);

// Retrieve the value associated with a key
FunctionPtr get(HashMap* map, int key);

// Remove a key-value pair from the hash map
void remove_pair(HashMap* map, int key);

// Free the memory allocated for the hash map
void free_hashmap(HashMap* map);

// Example function to be stored in the hash map
void example_function(int fd);



void* createReactor();

void startReactor(void* reactor);

void stopReactor(void* reactor);

void addFd(void* reactor, int fd, handler_t handler);

void waitFor(void* reactor);


#endif
