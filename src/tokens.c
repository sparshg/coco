#include "tokens.h"

Token string_to_token(char* str) {
    for (int i = 0; i < TOKENS_LEN; i++) {
        if (strcmp(str, tokens[i]) == 0) {
            return i;
        }
    }
    return -1;
}

char* token_to_string(Token token) {
    return tokens[token];
}