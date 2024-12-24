#include "snapDB.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define HASH_TABLE_SIZE 1337

static KeyValue* hashTable[HASH_TABLE_SIZE] = {NULL};

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
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        KeyValue curr = {NULL, NULL, NULL};
        hashTable[i] = &curr;
        printf("Struct at %i key is %s and val is %s \n", i, curr.key, curr.value);
    }
    // load persisted data from file into memory
    printf("Database sucessfully initialized.\n");
    return 0;
}

int put(const char* key, const char* value) {
    printf("Key %s Value %s\n", key, value);
    int index = hashIndex(key);
    KeyValue* curr = hashTable[index];
    printf("Struct at %i key is %s and val is %s \n", index, curr->key, curr->value);

    while (curr->key != NULL) {
        if (strcmp(curr->key, key) == 0) {
            printf("Am I here? Why? Key is %s and val is %s \n", curr->key, key);
            free(curr->value);
            curr->value = strdup(value);
            return 0;
        }
        curr = curr->next;
    }
    curr->key = strdup(key);
    curr->value = strdup(value);
    printf("Key now is %s Value now is %s\n", curr->key, curr->value);
    KeyValue next = {NULL, NULL, NULL};
    curr->next = &next;
    
    return 0;
}

int remove(const char* key) {
    int index = hashIndex(key);
    KeyValue* curr = hashTable[index];
    KeyValue* prev = NULL;

    while (curr->key != NULL) {
        if (strcmp(curr->key, key) == 0) {
            if (prev == NULL && curr->next != NULL) {
                hashTable[index] = curr->next;
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

char* get(const char* key) {
    int index = hashIndex(key);
    KeyValue* curr = hashTable[index];

    while (curr->key != NULL) {
        if (strcmp(curr->key, key) == 0) {
            printf("VALUE IS: %s\n", curr->value);
            return curr->value;
        }
        curr = curr->next;
    }

    return NULL;
}