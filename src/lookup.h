#ifndef LOOKUP_H
#define LOOKUP_H

typedef struct Entry {
    char* key;
    void* value;
} Entry;

typedef struct Entry* ENTRY;

typedef struct HashMap {
    ENTRY* entries;
    int size;
    int capacity;
} HashMap;

typedef struct HashMap* HASHMAP;

#endif