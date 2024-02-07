#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum Token Token;

enum Token {
    TK_ASSIGNOP,
    TK_COMMENT,
    TK_FIELDID,
    TK_ID,
    TK_NUM,
    TK_RNUM,
    TK_FUNID,
    TK_RUID,
    TK_WITH,
    TK_PARAMETERS,
    TK_END,
    TK_WHILE,
    TK_UNION,
    TK_ENDUNION,
    TK_DEFINETYPE,
    TK_AS,
    TK_TYPE,
    TK_MAIN,
    TK_GLOBAL,
    TK_PARAMETER,
    TK_LIST,
    TK_SQL,
    TK_SQR,
    TK_INPUT,
    TK_OUTPUT,
    TK_INT,
    TK_REAL,
    TK_COMMA,
    TK_SEM,
    TK_COLON,
    TK_DOT,
    TK_ENDWHILE,
    TK_OP,
    TK_CL,
    TK_IF,
    TK_THEN,
    TK_ENDIF,
    TK_READ,
    TK_WRITE,
    TK_RETURN,
    TK_PLUS,
    TK_MINUS,
    TK_MUL,
    TK_DIV,
    TK_CALL,
    TK_RECORD,
    TK_ENDRECORD,
    TK_ELSE,

    TK_AND,
    TK_OR,
    TK_NOT,
    TK_LT,
    TK_LE,
    TK_EQ,
    TK_GT,
    TK_GE,
    TK_NE,
};

int error() {
    printf("Error");
    exit(1);
}

char next(char* str) {
    // Return current char, and move to next
    // TODO: Handle EOF
    return *(str++);
}

char current(char* str) {
    // Return current char
    // TODO: Handle EOF
    return *str;
}
int try_special(char* str) {
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

int try_chained(char* str) {
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
                while (islower(next(str)))
                    ;
                return TK_RUID;
            }
    }
    return error();
}

int try_number(char* str) {
    // [0-9][0-9]*
    // [0-9][0-9]*[.][0-9][0-9]
    // [0-9][0-9]*[.][0-9][0-9][E][+|-|e][0-9][0-9]
    if (!isdigit(next(str))) return error();
    while (isdigit(next(str)))
        ;
    if (next(str) == '.' && isdigit(next(str)) && isdigit(next(str))) {
        if (next(str) == 'E' && (next(str) == '+' || current(str) == '-') && isdigit(next(str)) && isdigit(next(str))) {
            return TK_RNUM;
        }
        return TK_RNUM;
    }
    return TK_NUM;
}

int try_multipath(char* str) {
    // < <= <--- > >= _main _[a-z|A-Z][a-z|A-Z]*[0-9]*
    switch (next(str)) {
        case '<':
            if (next(str) == '=') return TK_LE;
            if (current(str) == '-' && next(str) == '-' && next(str) == '-')
                return TK_ASSIGNOP;
            return TK_LT;
        case '>':
            if (next(str) == '=') return TK_GE;
            return TK_GT;
        case '_':
            int save = &str;
            if (next(str) == 'm' && next(str) == 'a' && next(str) == 'i' && next(str) == 'n')
                return TK_MAIN;
            str = save;
            if (!isalpha(next(str))) return error();
            while (isalpha(next(str)))
                ;
            while (isdigit(next(str)))
                ;
            return TK_ID;
    }
    return error();
}

int try_id(char* str) {
    // Anything starting with [a-z]
}