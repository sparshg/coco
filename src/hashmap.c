
#include "hashmap.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tokens.h"

ENTRY create_entry(char* key, int value) {
    ENTRY entry = malloc(sizeof(struct Entry));
    entry->key = malloc(strlen(key) + 1);
    strcpy(entry->key, key);
    entry->value = value;
    return entry;
}

void delete_entry(ENTRY entry) {
    free(entry->key);
    free(entry);
}

HASHMAP create_hashmap(int capacity) {
    HASHMAP hashmap = malloc(sizeof(struct HashMap));
    hashmap->size = 0;
    hashmap->capacity = capacity;
    hashmap->entries = calloc(hashmap->capacity, sizeof(ENTRY));
    return hashmap;
}

void delete_hashmap(HASHMAP hashmap) {
    for (int i = 0; i < hashmap->size; i++) {
        if (hashmap->entries[i] != NULL)
            delete_entry(hashmap->entries[i]);
    }
    free(hashmap->entries);
    free(hashmap);
}

unsigned long hash(char* str, int keylen) {
    unsigned long hash = 5381;
    for (int i = 0; i < keylen; i++) {
        hash = ((hash << 5) + hash) + *str++;
    }
    return hash;
}

int get(HASHMAP hashmap, char* key, int keylen) {
    unsigned long index = hash(key, keylen) % hashmap->capacity;
    while (hashmap->entries[index] != NULL) {
        if (strncmp(hashmap->entries[index]->key, key, keylen) == 0) {
            return hashmap->entries[index]->value;
        }
        index = (index + 1) % hashmap->capacity;
    }
    return -1;
}

void insert(HASHMAP hashmap, char* key, int value) {
    if (hashmap->size >= hashmap->capacity) {
        printf("Hashmap overflowed\n");
        exit(1);
    }
    unsigned long index = hash(key, strlen(key)) % hashmap->capacity;
    while (hashmap->entries[index] != NULL) {
        index = (index + 1) % hashmap->capacity;
    }
    hashmap->entries[index] = create_entry(key, value);
    hashmap->size++;
}

void delete(HASHMAP hashmap, char* key, int keylen) {
    unsigned long index = hash(key, keylen) % hashmap->capacity;
    while (hashmap->entries[index] != NULL) {
        if (strcmp(hashmap->entries[index]->key, key) == 0) {
            delete_entry(hashmap->entries[index]);
            hashmap->entries[index] = NULL;
            hashmap->size--;
            return;
        }
        index = (index + 1) % hashmap->capacity;
    }
}

void print(HASHMAP hashmap) {
    for (int i = 0; i < hashmap->capacity; i++) {
        if (hashmap->entries[i] != NULL) {
            printf("(%d) %s: %d\n", i, hashmap->entries[i]->key, hashmap->entries[i]->value);
        }
    }
}
