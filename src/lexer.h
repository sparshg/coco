#ifndef LEXER
#define LEXER
#include "tokens.h"

void skip_whitespace(char** str);
Token try_all(char** str, HASHMAP table);
Token current(char** str);
Token next(char** str);
Token try_special(char** str);
Token try_chained(char** str);
Token try_id(char** str, HASHMAP table);
Token try_number(char** str);
Token try_multipath(char** str);

#endif