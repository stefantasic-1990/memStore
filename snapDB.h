#ifndef SNAPDB_H
#define SNAPDB_H

typedef struct {
    char* key;
    char* value;
} KeyValue;

int initializeDatabase();
int put(char* key, char* value);
int remove();
char* get();

#endif