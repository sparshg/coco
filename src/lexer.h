#ifndef LEXER
#define LEXER
#include "tokens.h"

Token try_all(BUF b, HASHMAP table);
Token try_special(BUF b);
Token try_chained(BUF b);
Token try_id(BUF b, HASHMAP table);
Token try_number(BUF b);
Token try_multipath(BUF b);
void remove_comments(char* testcaseFile, char* cleanFile);

#endif