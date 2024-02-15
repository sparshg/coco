#ifndef LOOKUP_H
#define LOOKUP_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef char* string;


struct LookupTable {
    string* table;
    int length;
    int size;
};

typedef struct LookupTable* LOOKUP;

LOOKUP _InitTable();
bool _FindLexeme(LOOKUP lookup, string lexeme);
void _InsertLexeme(LOOKUP lookup, string lexeme);

#endif