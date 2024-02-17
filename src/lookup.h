#ifndef LOOKUP_H
#define LOOKUP_H

typedef struct TableEntry {
    char* lexeme;
    int token;
} TableEntry;

typedef struct TableEntry* TABLE_ENTRY;

typedef struct LookupTable {
    TABLE_ENTRY table;
    int length;
    int size;
} LookupTable;

typedef struct LookupTable* LOOKUP_TABLE;

LOOKUP_TABLE _InitTable();
int _FindLexeme(LOOKUP_TABLE lookup, char* lexeme);
int _InsertLexeme(LOOKUP_TABLE lookup, char* lexeme, int token);

#endif