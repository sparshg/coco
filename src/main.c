#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"
#include "lookup.h"
#include "tokens.h"

int main(int argc, char* argv[]) {
    char* str = "203\n203\n<--->===_hi%/_main([])>=#a00.00E+09 <--";
    char* start;

    while (*str != '\0') {
        start = str;
        int token = try_all(&str);
        if (token == -1) {
            printf("Error at %c (Hex: %x, Dec: %d, Diff: %ld)\n", *str, *str, *str, str - start);
            break;
        }
        printf("Found: %s, %.*s\n", token_to_string(token), (int)(str - start), start);
        // printf("Now at %c (Hex: %x, Dec: %d, Diff: %ld)\n", *str, *str, *str, str - start);
        skip_whitespace(&str);
        // printf("After whitespace at %c (Hex: %x, Dec: %d, Diff: %ld)\n\n", *str, *str, *str, str - start);
    }

    return 0;
}