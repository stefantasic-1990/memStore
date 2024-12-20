#include "snapDB.h"
#include <stdio.h>
#include <string.h>

#define HASH_TABLE_SIZE 1337

static KeyValue hashTable[HASH_TABLE_SIZE];

unsigned long hashIndex(const char* key) {
    int c;
    unsigned long hash = 5381;

    while ((c = *key++)) {
        hash = ((hash << 5) + hash) + c;
    }
    int hashIndex = hash % HASH_TABLE_SIZE;

    return hashIndex;
}

int initializeDatabase() {
    printf("Initializing database...\n");
    // initialize hashTable structure
    // load persisted data from file into memory
    printf("Database sucessfully initialized.\n");
    return 0;
}

int put(const char* key, const char* value) {
    int index = hashIndex(key);
    KeyValue* curr = &hashTable[index];

    while (curr->key != NULL) {
        if (strcmp(curr->key, key) == 0) {
            free(curr->value);
            curr->value = strdup(value);
            return 0;
        }
        curr = curr->next;
    }
    curr->key = strdup(key);
    curr->value = strdup(value);
    curr->next = malloc(sizeof(KeyValue));

    // initialize next empty slot
    curr = curr->next;
    curr->key = NULL;
    curr->value = NULL;
    curr->next = NULL;
    
    return 0;
}

int remove(const char* key) {
    int index = hashIndex(key);
    KeyValue* curr = &hashTable[index];
    KeyValue* prev = NULL;

    while (curr->key != NULL) {
        if (strcmp(curr->key, key) == 0) {
            
        }
        prev = curr;
        curr = curr->next;
    }
}

char* get() {
    
}