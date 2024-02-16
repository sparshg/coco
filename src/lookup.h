#ifndef LOOKUP_H
#define LOOKUP_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef char* string;

typedef struct TableEntry {
    string lexeme;
    int token;
} TableEntry;

typedef struct TableEntry* Record;

typedef struct LookupTable {
    Record table;
    int length;
    int size;
} LookupTable;

typedef struct LookupTable* LOOKUP;

LOOKUP _InitTable();
int _FindLexeme(LOOKUP lookup, string lexeme);
int _InsertLexeme(LOOKUP lookup, string lexeme, int token);

#endif