#ifndef LEXER
#define LEXER

void skip_whitespace(char** str);
int try_all(char** str);
int current(char** str);
int next(char** str);
int try_special(char** str);
int try_chained(char** str);
int try_id(char** str);
int try_number(char** str);
int try_multipath(char** str);

#endif