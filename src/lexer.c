#include "lexer.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "buffer.h"
#include "hashmap.h"
#include "tokens.h"

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
    return WRONG_PATH;
}

Token try_chained(BUF b) {
    Token token;
    // #[a-z][a-z]* &&& @@@ != ==

    switch (next(b)) {
        case '!':
            if (next(b) == '=')
                return TK_NE;
            return WRONG_PATTERN;
        case '=':
            if (next(b) == '=')
                return TK_EQ;
            return WRONG_PATTERN;
        case '@':
            if (next(b) == '@' && next(b) == '@')
                return TK_OR;
            return WRONG_PATTERN;
        case '&':
            if (next(b) == '&' && next(b) == '&')
                return TK_AND;
            return WRONG_PATTERN;
        case '#':
            if (islower(next(b))) {
                while (islower(current(b))) next(b);
                return TK_RUID;
            }
            return WRONG_PATTERN;
    }
    return WRONG_PATH;
}

Token try_number(BUF b) {
    // [0-9][0-9]*
    // [0-9][0-9]*[.][0-9][0-9]
    // [0-9][0-9]*[.][0-9][0-9][E][+|-|e][0-9][0-9]
    if (!isdigit(next(b))) return WRONG_PATH;
    while (isdigit(current(b))) next(b);
    push_state(b);
    if (next(b) == '.' && isdigit(next(b))) {
        if (!isdigit(next(b))) return WRONG_PATTERN;
        push_state(b);
        if (next(b) == 'E') {
            if (current(b) == '+' || current(b) == '-') next(b);
            if (isdigit(next(b)) && isdigit(next(b))) return TK_RNUM;
            return WRONG_PATTERN;
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
                return WRONG_PATTERN;
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
            if (!isalpha(next(b))) return WRONG_PATTERN;
            int len = 2;
            while (isalpha(current(b))) {
                next(b);
                len++;
            }
            while (isdigit(current(b))) {
                next(b);
                len++;
            }
            if (len > 30) 
                return FUN_LEN_EXCEED;
                // printf("Function Identifier is longer than the prescribed length of 30 characters.\n");
            return TK_FUNID;
        }
    }
    return WRONG_PATH;
}

Token try_id(BUF b, HASHMAP table) {
    // Anything starting with [a-z]
    if (!isalpha(current(b))) return WRONG_PATH;
    int t, len = 2;
    push_state(b);
    if (current(b) >= 'b' && next(b) <= 'd' && isdigit(next(b))) {
        while (current(b) >= 'b' && current(b) <= 'd') {
            next(b);
            len++;
        }
        while (isdigit(current(b))) {
            next(b);
            len++;
        }
        if (len > 20) 
            return VAR_LEN_EXCEED;
            // printf("Variable Identifier is longer than the prescribed length of 20 characters.\n");
        return TK_ID;
    }
    pop_state(b);
    int n = push_state(b);
    while (isalpha(current(b))) next(b);
    char* str = string_from(b, n);
    if ((t = get(table, str, strlen(str))) != -1) {
        return t;
    }
    return TK_FIELDID;
}

Token try_all(BUF b, HASHMAP table) {
    // Try all tokens
    Token token = WRONG_PATH;
    int n = push_state(b);
    if ((token = try_special(b)) != WRONG_PATH) return token;
    pop_nth(b, n);
    push_state(b);
    if ((token = try_chained(b)) != WRONG_PATH) return token;
    pop_nth(b, n);
    push_state(b);
    if ((token = try_number(b)) != WRONG_PATH) return token;
    pop_nth(b, n);
    push_state(b);
    if ((token = try_multipath(b)) != WRONG_PATH) return token;
    pop_nth(b, n);
    push_state(b);
    if ((token = try_id(b, table)) != WRONG_PATH) return token;
    pop_nth(b, n);
    return WRONG_SYMBOL;
}

void remove_comments(char* testcaseFile, char* cleanFile) {
    FILE *src_file, *dest_file;
    char buffer[1024];

    src_file = fopen(testcaseFile, "r");
    dest_file = fopen(cleanFile, "w");

    if (!src_file || !dest_file) {
        printf("Error in Opening Files!\nComment Removal Aborted!\n");
        exit(1);
    }

    while (fgets(buffer, 1024, src_file)) {
        int i = 0;
        while (buffer[i] != '\0') {
            if (buffer[i] == '%') {
                buffer[i] = '\n';
                buffer[i + 1] = '\0';
                break;
            }
            i++;
        }
        fprintf(dest_file, "%s", buffer);
    }
    fclose(src_file);
    fclose(dest_file);
}