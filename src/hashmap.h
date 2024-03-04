#ifndef HASHMAP_H
#define HASHMAP_H

typedef struct Entry {
    char* key;
    int value;
} Entry;

typedef struct Entry* ENTRY;

typedef struct HashMap {
    ENTRY* entries;
    int size;
    int capacity;
} HashMap;

typedef struct HashMap* HASHMAP;

HASHMAP create_hashmap(int capacity);
void delete_hashmap(HASHMAP hashmap);
int get(HASHMAP hashmap, char* key, int keylen);
void insert(HASHMAP hashmap, char* key, int value);
void delete(HASHMAP hashmap, char* key, int keylen);
void print(HASHMAP hashmap);
void delete_hashmap(HASHMAP hashmap);

#endif