#include "snapDB.h"
#include <stdio.h>

#define HASH_TABLE_SIZE 1337;

unsigned long hashIndex(const char* key) {
    unsigned long hash = 5381;
    int c;
    while ((c = *key++)) {
        hash = ((hash << 5) + hash) + c;
    }
    int hashIndex = hash % HASH_TABLE_SIZE;
    
    return hashIndex;
}

unsigned long hash_index(const char *str, unsigned long table_size) {
    unsigned long hash = hash_djb2(str);
    return hash % table_size;  // Use modulo to ensure the index is within table bounds
}

int initializeDatabase() {
    printf("Initializing database...\n");
    printf("Database sucessfully initialized.\n");
    return 0;
}

int put();
int remove();
char* get();