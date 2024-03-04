#include "tokens.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hashmap.h"

HASHMAP create_keyword_table() {
    HASHMAP table = create_hashmap(1024);
    for (int i = 0; i < KEYWORDS_LEN; i++) {
        insert(table, (char*)keywords[i], i + TOKENS_LEN - KEYWORDS_LEN);
    }
    return table;
}

HASHMAP create_symbol_map() {
    HASHMAP map = create_hashmap(1024);
    for (int i = 0; i < SYMBOLS_LEN; i++) {
        insert(map, (char*)symbols[i], i);
    }
    return map;
}

Token string_to_symbol(char* str, HASHMAP symbol_map) {
    return get(symbol_map, str, strlen(str));
}

char* token_to_string(Token token) {
    return (char*)symbols[token];
}

int is_non_terminal(int symbol) {
    return symbol >= SYMBOLS_LEN - NT_LEN;
}

int is_end_symbol(int symbol) {
    return symbol == TOKENS_LEN;
}