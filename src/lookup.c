
#include "lookup.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tokens.h"

ENTRY create_entry(char* key, void* value) {
    ENTRY entry = malloc(sizeof(struct Entry));
    entry->key = key;
    entry->value = value;
    return entry;
}

void delete_entry(ENTRY entry) {
    free(entry->key);
    free(entry->value);
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
        delete_entry(hashmap->entries[i]);
    }
    free(hashmap->entries);
    free(hashmap);
}

unsigned long hash(char* str) {
    unsigned long hash = 5381;
    int c;
    while (c = *str++)
        hash = ((hash << 5) + hash) + c;
    return hash;
}

void* get(HASHMAP hashmap, char* key) {
    unsigned long index = hash(key) % hashmap->capacity;
    while (hashmap->entries[index] != NULL) {
        if (strcmp(hashmap->entries[index]->key, key) == 0) {
            return hashmap->entries[index]->value;
        }
        index = (index + 1) % hashmap->capacity;
    }
    return NULL;
}

void insert(HASHMAP hashmap, char* key, void* value) {
    if (hashmap->size >= hashmap->capacity) {
        printf("Hashmap overflowed\n");
        exit(1);
        return;
    }
    unsigned long index = hash(key) % hashmap->capacity;
    while (hashmap->entries[index] != NULL) {
        index = (index + 1) % hashmap->capacity;
    }
    hashmap->entries[index] = create_entry(key, value);
    hashmap->size++;
}

void remove_entry(HASHMAP hashmap, char* key) {
    unsigned long index = hash(key) % hashmap->capacity;
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

void print_hashmap(HASHMAP hashmap) {
    for (int i = 0; i < hashmap->capacity; i++) {
        if (hashmap->entries[i] != NULL) {
            printf("%s: %s\n", hashmap->entries[i]->key, hashmap->entries[i]->value);
        }
    }
}