#ifndef LEXER
#define LEXER
#include "buffer.h"
#include "hashmap.h"
#include "tokens.h"

enum LexerError {
    WRONG_PATTERN = -1,
    WRONG_PATH = -2,
    WRONG_SYMBOL = -3,
    VAR_LEN_EXCEED = -4,
    FUN_LEN_EXCEED = -5
};

Token get_next_token(BUF b, HASHMAP table);
Token try_special(BUF b);
Token try_chained(BUF b);
Token try_id(BUF b, HASHMAP table);
Token try_number(BUF b);
Token try_multipath(BUF b);
void remove_comments(char* testcaseFile);
void print_lexer_output(char* testcaseFile);

#endif