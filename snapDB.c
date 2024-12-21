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
    // load persisted data from file into memory
    printf("Database sucessfully initialized.\n");
    return 0;
}

int put(const char* key, const char* value) {
    int index = hashIndex(key);
    KeyValue* curr = &hashTable[index];

    if (curr->key == NULL) {
        curr->key = strdup(key);
        curr->value = strdup(value);
        curr->next = NULL;
    } else {
        while (curr->next != NULL) {
            if (strcmp(curr->key, key) == 0) {
                free(curr->value);
                curr->value = strdup(value);
                return 0;
            }
            curr = curr->next;
        }
        if (strcmp(curr->key, key) == 0) {
            free(curr->value);
            curr->value = strdup(value);
            return 0;
        } else {
            curr->next = malloc(sizeof(KeyValue));
            curr = curr->next;
            curr->key = strdup(key);
            curr->value = strdup(value);
            curr->next = NULL;
            return 0;
        }
    }
}

int remove() {

}

char* get() {
    
}