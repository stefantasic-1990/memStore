#include "snapDB.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define HASH_TABLE_SIZE 1337

static KeyValue* hashTable[HASH_TABLE_SIZE] = {NULL};

unsigned long getHashIndex(const char* key) {
    int c;
    unsigned long hash = 5381;

    while ((c = *key++)) {
        hash = ((hash << 5) + hash) + c;
    }
    int getHashIndex = hash % HASH_TABLE_SIZE;

    return getHashIndex;
}

int initializeDatabase() {
    printf("Initializing database...\n");
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        KeyValue* curr = malloc(sizeof(KeyValue));
        if (curr == NULL) {
            perror("Failed to allocate memory");
        }
        curr->key = NULL;
        curr->value = NULL;
        curr->next = NULL;
        hashTable[i] = curr;
    }
    printf("Database initialized\n");
    return 0;
}

char* get(const char* key) {
    KeyValue* curr = hashTable[getHashIndex(key)];
    while (curr->key != NULL) {
        if (strcmp(curr->key, key) == 0) {
            return curr->value;
        }
        curr = curr->next;
    }

    return NULL;
}

int put(const char* key, const char* value) {
    KeyValue* curr = hashTable[getHashIndex(key)];
    while (curr->key != NULL) {
        if (strcmp(curr->key, key) == 0) {
            free(curr->value);
            curr->value = strdup(value);
            return 0;
        }
        curr = curr->next;
    }
    KeyValue* next = malloc(sizeof(KeyValue));
    curr->key = strdup(key);
    curr->value = strdup(value);
    curr->next = next;
    next->key = NULL;
    next->value = NULL;
    next->next = NULL;
    
    return 0;
}

int remove(const char* key) {
    KeyValue* curr = hashTable[getHashIndex(key)];
    KeyValue* prev = NULL;

    while (curr->key != NULL) {
        if (strcmp(curr->key, key) == 0) {
            if (prev == NULL && curr->next != NULL) {
                hashTable[getHashIndex(key)] = curr->next;
                free(curr->key);
                free(curr->value);
                free(curr);
                return 0;
            } else if (prev != NULL && curr->next != NULL) {
                prev->next = curr->next;
                free(curr->key);
                free(curr->value);
                free(curr);
                return 0;
            } else {
                prev->next = NULL;
                free(curr->key);
                free(curr->value);
                free(curr);
                return 0;
            }
        }
        prev = curr;
        curr = curr->next;
    }

    return 0;
}