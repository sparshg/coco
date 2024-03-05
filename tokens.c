// Group 26
// Rishi Gupta     (2021A7PS0690P)
// Sparsh Goenka   (2021A7PS2413P)
// Utkarsh Sharma  (2021A7PS0693P)
// Saumya Sharma   (2021A7PS0544P)
// Akshat Bajpai   (2021A7PS0573P)
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

int string_to_symbol(char* str, HASHMAP symbol_map) {
    return get(symbol_map, str, strlen(str));
}

char* symbol_to_string(int symbol) {
    if (symbol < 0 || symbol >= SYMBOLS_LEN) {
        return NULL;
    }
    return (char*)symbols[symbol];
}

int rule_to_nt(int rule_no, int** grammar_rules) {
    return grammar_rules[rule_no][1] - SYMBOLS_LEN + NT_LEN;
}

int symbol_to_nt(int symbol) {
    return symbol - SYMBOLS_LEN + NT_LEN;
}

int is_epsilon(int symbol) {
    return symbol == TOKENS_LEN + 1;
}

int nt_to_symbol(int nt) {
    return nt + SYMBOLS_LEN - NT_LEN;
}

char* nt_to_string(int nt) {
    return (char*)symbols[nt + SYMBOLS_LEN - NT_LEN];
}