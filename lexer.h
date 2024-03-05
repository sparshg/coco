// Group 26
// Rishi Gupta     (2021A7PS0690P)
// Sparsh Goenka   (2021A7PS2413P)
// Utkarsh Sharma  (2021A7PS0693P)
// Saumya Sharma   (2021A7PS0544P)
// Akshat Bajpai   (2021A7PS0573P)
#ifndef LEXER_H
#define LEXER_H
#include "buffer.h"
#include "hashmap.h"
#include "tokens.h"

enum LexerError {
    WRONG_PATTERN = -1,
    WRONG_PATH = -2,
    WRONG_SYMBOL = -3,
    VAR_LEN_EXCEED = -4,
    FUN_LEN_EXCEED = -5,
    REACHED_EOF = -6
};

Token get_next_token(BUF b, HASHMAP keyword_table, int* line, int* last_state, int prints_output);
Token try_special(BUF b);
Token try_chained(BUF b);
Token try_id(BUF b, HASHMAP table);
Token try_number(BUF b);
Token try_multipath(BUF b);
void remove_comments(char* testcaseFile);

#endif