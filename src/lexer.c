#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tokens.h"



struct Entry {
    char* name;
    Token tk;
};

Token error() {
    return -1;
}

char next(char** str) {
    // Return current char, and move to next
    // TODO: Handle EOF
    return *((*str)++);
}

char current(char** str) {
    // Return current char
    // TODO: Handle EOF
    return **str;
}

void skip_whitespace(char** str) {
    while (isspace(current(str)))
        (*str)++;
}

Token try_special(char** str) {
    // % [ ] , ; : . ( ) + - * / ~
    // clang-format off
    switch (next(str)) {
        case '%': return TK_COMMENT;
        case '[': return TK_SQL;
        case ']': return TK_SQR;
        case ',': return TK_COMMA;
        case ';': return TK_SEM;
        case ':': return TK_COLON;
        case '.': return TK_DOT;
        case '(': return TK_OP;
        case ')': return TK_CL;
        case '+': return TK_PLUS;
        case '-': return TK_MINUS;
        case '*': return TK_MUL;
        case '/': return TK_DIV;
        case '~': return TK_NOT;
    }
    // clang-format on
    return error();
}

Token try_chained(char** str) {
    Token token;
    // #[a-z][a-z]* &&& @@@ != ==

    switch (next(str)) {
        case '!':
            if (next(str) == '=')
                return TK_NE;
        case '=':
            if (next(str) == '=')
                return TK_EQ;
        case '@':
            if (next(str) == '@' && next(str) == '@')
                return TK_OR;
        case '&':
            if (next(str) == '&' && next(str) == '&' && next(str) == '&')
                return TK_AND;
        case '#':
            if (islower(next(str))) {
                while (islower(current(str))) (*str)++;
                return TK_RUID;
            }
    }
    return error();
}

Token try_number(char** str) {
    // [0-9][0-9]*
    // [0-9][0-9]*[.][0-9][0-9]
    // [0-9][0-9]*[.][0-9][0-9][E][+|-|e][0-9][0-9]
    if (!isdigit(next(str))) return error();
    while (isdigit(current(str))) (*str)++;
    char* save = *str;
    if (next(str) == '.' && isdigit(next(str)) && isdigit(next(str))) {
        char* save2 = *str;
        if (next(str) == 'E' && (next(str) == '+' || current(str) == '-') && isdigit(next(str)) && isdigit(next(str))) {
            return TK_RNUM;
        }
        *str = save2;
        return TK_RNUM;
    }
    *str = save;
    return TK_NUM;
}

Token try_multipath(char** str) {
    // < <= <--- > >= _main _[a-z|A-Z][a-z|A-Z]*[0-9]*
    switch (next(str)) {
        case '<': {
            char* save = *str;
            if (next(str) == '=') return TK_LE;
            if (current(str) == '-' && next(str) == '-' && next(str) == '-')
                return TK_ASSIGNOP;
            *str = save;
            return TK_LT;
        }
        case '>': {
            char* save = *str;
            if (next(str) == '=') return TK_GE;
            *str = save;
            return TK_GT;
        }
        case '_': {
            char* save = *str;
            if (next(str) == 'm' && next(str) == 'a' && next(str) == 'i' && next(str) == 'n')
                return TK_MAIN;
            *str = save;
            if (!isalpha(next(str))) return error();
            while (isalpha(current(str))) (*str)++;
            while (isdigit(current(str))) (*str)++;
            return TK_FUNID;
        }
    }
    return error();
}

Token try_id(char** str) {
    // Anything starting with [a-z]
    if (!isalpha(next(str))) return error();
    return error();
}

Token try_all(char** str) {
    // Try all tokens
    Token token = -1;
    char* save = *str;
    if ((token = try_special(str)) != -1) return token;
    *str = save;
    if ((token = try_chained(str)) != -1) return token;
    *str = save;
    if ((token = try_number(str)) != -1) return token;
    *str = save;
    if ((token = try_multipath(str)) != -1) return token;
    *str = save;
    if ((token = try_id(str)) != -1) return token;
    *str = save;
    return error();
}