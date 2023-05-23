#include "reactor.h"

int hash(int key, int capacity) {
    return abs(key) % capacity;
}

// Create a new key-value pair
KeyValuePair* create_pair(int key, handler_t handler) {
    KeyValuePair* pair = malloc(sizeof(KeyValuePair));
    pair->key = key;
    pair->value = value;
    return pair;
}

// Initialize the hash map
HashMap* create_hashmap(int capacity) {
    HashMap* map = malloc(sizeof(HashMap));
    map->data = malloc(capacity * sizeof(KeyValuePair*));
    map->capacity = capacity;
    for (int i = 0; i < capacity; i++) {
        map->data[i] = NULL;
    }
    return map;
}

// Insert a key-value pair into the hash map
void insert(HashMap* map, int key, handler_t handler) {
    int index = hash(key, map->capacity);
    KeyValuePair* pair = create_pair(key, value);
    map->data[index] = pair;
}

// Retrieve the value associated with a key
FunctionPtr get(HashMap* map, int key) {
    int index = hash(key, map->capacity);
    KeyValuePair* pair = map->data[index];
    if (pair != NULL && pair->key == key) {
        return pair->value;
    }
    return NULL;  // Key not found
}

// Remove a key-value pair from the hash map
void remove_pair(HashMap* map, int key) {
    int index = hash(key, map->capacity);
    KeyValuePair* pair = map->data[index];
    if (pair != NULL && pair->key == key) {
        free(pair);
        map->data[index] = NULL;
    }
}

// Free the memory allocated for the hash map
void free_hashmap(HashMap* map) {
    for (int i = 0; i < map->capacity; i++) {
        KeyValuePair* pair = map->data[i];
        if (pair != NULL) {
            free(pair);
        }
    }
    free(map->data);
    free(map);
}

// Example function to be stored in the hash map
void example_function(int fd) {printf("Called example_function with fd: %d\n", fd);}

void* createReactor(){}

void startReactor(void* reactor);

void stopReactor(void* reactor);

void addFd(void* reactor, int fd, handler_t handler);

void waitFor(void* reactor);



v