#include "snapDB.h"
#include <stdio.h>

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

int put(char* key, char* value) {
    int index = hashIndex(key);
    hashTable[index]; 
}

int remove() {

}

char* get() [

]