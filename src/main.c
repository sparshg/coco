#include <stdio.h>

#include "lexer.h"

char tokens[][14] = {
    "TK_ASSIGNOP", "TK_COMMENT", "TK_FIELDID", "TK_ID", "TK_NUM", "TK_RNUM", "TK_FUNID", "TK_RUID", "TK_WITH", "TK_PARAMETERS", "TK_END", "TK_WHILE", "TK_UNION", "TK_ENDUNION", "TK_DEFINETYPE", "TK_AS", "TK_TYPE", "TK_MAIN", "TK_GLOBAL", "TK_PARAMETER", "TK_LIST", "TK_SQL", "TK_SQR", "TK_INPUT", "TK_OUTPUT", "TK_INT", "TK_REAL", "TK_COMMA", "TK_SEM", "TK_COLON", "TK_DOT", "TK_ENDWHILE", "TK_OP", "TK_CL", "TK_IF", "TK_THEN", "TK_ENDIF", "TK_READ", "TK_WRITE", "TK_RETURN", "TK_PLUS", "TK_MINUS", "TK_MUL", "TK_DIV", "TK_CALL", "TK_RECORD", "TK_ENDRECORD", "TK_ELSE", "TK_AND", "TK_OR", "TK_NOT", "TK_LT", "TK_LE", "TK_EQ", "TK_GT", "TK_GE", "TK_NE"};

int main(int argc, char* argv[]) {
    char* str = "203\n203\n<---_main>=#a00.00E+09";
    char* start;
    int count = 0;
    while (*str != '\0' && count++ < 20) {
        start = str;
        int token = try_all(&str);
        if (token == -1) {
            printf("Error at %c (Hex: %x, Dec: %d, Diff: %ld)\n", *str, *str, *str, str - start);
            break;
        }
        printf("Found: %s, %.*s\n", tokens[token], (int)(str - start), start);
        printf("Now at %c (Hex: %x, Dec: %d, Diff: %ld)\n", *str, *str, *str, str - start);
        skip_whitespace(&str);
        printf("After whitespace at %c (Hex: %x, Dec: %d, Diff: %ld)\n\n", *str, *str, *str, str - start);
    }
    // start = str;
    // str += 24;
    // printf("%c %x", *str, *str);
    return 0;
}