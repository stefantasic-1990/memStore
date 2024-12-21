#ifndef SNAPDB_H
#define SNAPDB_H

typedef struct KeyValue {
    char* key;
    char* value;
    struct KeyValue* next;
} KeyValue;

int initializeDatabase();
int put(const char* key, const char* value);
int remove(const char* key);
char* get(const char* key);

#endif