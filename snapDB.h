#ifndef SNAPDB_H
#define SNAPDB_H

typedef struct {
    char* key;
    char* value;
} KeyValue;

int initializeDatabase();
int put();
int remove();
char* get();

#endif