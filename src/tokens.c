#include "tokens.h"

#include <stdlib.h>
#include <string.h>

#include "hashmap.h"

HASHMAP create_keyword_table() {
    HASHMAP table = create_hashmap(1024);
    for (int i = 0; i < KEYWORDS_LEN; i++) {
        insert(table, (char*)keywords[i], string_to_token((char*)tokens[i + TOKENS_LEN - KEYWORDS_LEN]));
    }
    return table;
}

Token string_to_token(char* str) {
    for (int i = 0; i < TOKENS_LEN; i++) {
        if (strcmp(str, tokens[i]) == 0) {
            return i;
        }
    }
    return -1;
}

char* token_to_string(Token token) {
    return (char*)tokens[token];
}