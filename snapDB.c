#include "snapDB.h"
#include <stdio.h>

unsigned long hash(const char *str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }

    return hash;
}

int initializeDatabase() {
    printf("Initializing database...\n");
    printf("Database sucessfully initialized.\n");
    return 0;
}

int put();
int remove();
char* get();