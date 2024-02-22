#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "buffer.h"
#include "hashmap.h"
#include "tokens.h"

Token error() {
    return -1;
}

Token try_special(BUF b) {
    // % [ ] , ; : . ( ) + - * / ~
    // clang-format off
    switch (next(b)) {
        case '%': { while (next(b) != '\n') {}; return TK_COMMENT; }
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

Token try_chained(BUF b) {
    Token token;
    // #[a-z][a-z]* &&& @@@ != ==

    switch (next(b)) {
        case '!':
            if (next(b) == '=')
                return TK_NE;
        case '=':
            if (next(b) == '=')
                return TK_EQ;
        case '@':
            if (next(b) == '@' && next(b) == '@')
                return TK_OR;
        case '&':
            if (next(b) == '&' && next(b) == '&' && next(b) == '&')
                return TK_AND;
        case '#':
            if (islower(next(b))) {
                while (islower(current(b))) next(b);
                return TK_RUID;
            }
    }
    return error();
}

Token try_number(BUF b) {
    // [0-9][0-9]*
    // [0-9][0-9]*[.][0-9][0-9]
    // [0-9][0-9]*[.][0-9][0-9][E][+|-|e][0-9][0-9]
    if (!isdigit(next(b))) return error();
    while (isdigit(current(b))) next(b);
    push_state(b);
    if (next(b) == '.' && isdigit(next(b))) {
        if (!isdigit(next(b))) return error();
        push_state(b);
        if (next(b) == 'E' && (next(b) == '+' || current(b) == '-') && isdigit(next(b)) && isdigit(next(b))) {
            return TK_RNUM;
        }
        pop_state(b);
        return TK_RNUM;
    }
    pop_state(b);
    return TK_NUM;
}

Token try_multipath(BUF b) {
    // < <= <--- > >= _main _[a-z|A-Z][a-z|A-Z]*[0-9]*
    switch (next(b)) {
        case '<': {
            push_state(b);
            if (next(b) == '=') return TK_LE;
            if (current(b) == '-' && next(b) == '-') {
                if (next(b) == '-')
                    return TK_ASSIGNOP;
                return error();
            }
            pop_state(b);
            return TK_LT;
        }
        case '>': {
            push_state(b);
            if (next(b) == '=') return TK_GE;
            pop_state(b);
            return TK_GT;
        }
        case '_': {
            push_state(b);
            if (next(b) == 'm' && next(b) == 'a' && next(b) == 'i' && next(b) == 'n')
                return TK_MAIN;
            pop_state(b);
            if (!isalpha(next(b))) return error();
            while (isalpha(current(b))) next(b);
            while (isdigit(current(b))) next(b);
            return TK_FUNID;
        }
    }
    return error();
}

Token try_id(BUF b, HASHMAP table) {
    // Anything starting with [a-z]
    if (!isalpha(current(b))) return error();
    int n = push_state(b), t;
    if (current(b) >= 'b' && next(b) <= 'd' && isdigit(next(b))) {
        while (current(b) >= 'b' && current(b) <= 'd') next(b);
        while (isdigit(current(b))) next(b);
        return TK_ID;
    }
    while (isalpha(current(b))) next(b);
    char* str = string_from(b, n);
    if ((t = get(table, str, strlen(str)))) {
        return t;
    }
    return TK_FIELDID;
}

Token try_all(BUF b, HASHMAP table) {
    // Try all tokens
    Token token = -1;
    int n = push_state(b);
    if ((token = try_special(b)) != -1) return token;
    pop_nth(b, n);
    push_state(b);
    if ((token = try_chained(b)) != -1) return token;
    pop_nth(b, n);
    push_state(b);
    if ((token = try_number(b)) != -1) return token;
    pop_nth(b, n);
    push_state(b);
    if ((token = try_multipath(b)) != -1) return token;
    pop_nth(b, n);
    push_state(b);
    if ((token = try_id(b, table)) != -1) return token;
    pop_nth(b, n);
    return error();
}