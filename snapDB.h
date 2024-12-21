#ifndef SNAPDB_H
#define SNAPDB_H

typedef struct {
    char* key;
    char* value;
    struct KeyValue* next;
} KeyValue;

int initializeDatabase();
int put(char* key, char* value);
int remove();
char* get();

#endif