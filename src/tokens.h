#ifndef TOKENS_H
#define TOKENS_H

#include "hashmap.h"
typedef enum Token Token;

#define MAX_KEYWORD_LEN 30 + 1
#define TOKENS_LEN 57
#define KEYWORDS_LEN 28

static const char tokens[][14] = {
    "TK_ASSIGNOP", "TK_COMMENT", "TK_FIELDID", "TK_ID", "TK_NUM", "TK_RNUM", "TK_FUNID", "TK_RUID", "TK_SQL", "TK_SQR", "TK_COMMA", "TK_SEM", "TK_COLON", "TK_DOT", "TK_OP", "TK_CL", "TK_PLUS", "TK_MINUS", "TK_MUL", "TK_DIV", "TK_AND", "TK_OR", "TK_NOT", "TK_LT", "TK_LE", "TK_EQ", "TK_GT", "TK_GE", "TK_NE", "TK_WITH", "TK_PARAMETERS", "TK_END", "TK_WHILE", "TK_UNION", "TK_ENDUNION", "TK_DEFINETYPE", "TK_AS", "TK_TYPE", "TK_MAIN", "TK_GLOBAL", "TK_PARAMETER", "TK_LIST", "TK_INPUT", "TK_OUTPUT", "TK_INT", "TK_REAL", "TK_ENDWHILE", "TK_IF", "TK_THEN", "TK_ENDIF", "TK_READ", "TK_WRITE", "TK_RETURN", "TK_CALL", "TK_RECORD", "TK_ENDRECORD", "TK_ELSE"};

static const char keywords[][21] = {
    "with", "parameters", "end", "while", "union", "endunion", "definetype", "as", "type", "_main", "global", "parameter", "list", "input", "output", "int", "real", "endwhile", "if", "then", "endif", "read", "write", "return", "call", "record", "endrecord", "else"};

char* token_to_string(Token token);
Token string_to_token(char* str);
HASHMAP create_keyword_table();

enum Token {
    TK_ASSIGNOP,
    TK_COMMENT,
    TK_FIELDID,
    TK_ID,
    TK_NUM,
    TK_RNUM,
    TK_FUNID,
    TK_RUID,
    TK_SQL,
    TK_SQR,
    TK_COMMA,
    TK_SEM,
    TK_COLON,
    TK_DOT,
    TK_OP,
    TK_CL,
    TK_PLUS,
    TK_MINUS,
    TK_MUL,
    TK_DIV,
    TK_AND,
    TK_OR,
    TK_NOT,
    TK_LT,
    TK_LE,
    TK_EQ,
    TK_GT,
    TK_GE,
    TK_NE,
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
    TK_INPUT,
    TK_OUTPUT,
    TK_INT,
    TK_REAL,
    TK_ENDWHILE,
    TK_IF,
    TK_THEN,
    TK_ENDIF,
    TK_READ,
    TK_WRITE,
    TK_RETURN,
    TK_CALL,
    TK_RECORD,
    TK_ENDRECORD,
    TK_ELSE,
};

#endif