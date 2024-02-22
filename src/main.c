#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "buffer.h"
#include "hashmap.h"
#include "lexer.h"
#include "tokens.h"

int main(int argc, char* argv[]) {
    BUF b = read_file("hi.txt");

    HASHMAP table = create_keyword_table();

    while (current(b) != EOF) {
        clear_saves(b);
        int n = push_state(b);
        int token = try_all(b, table);
        if (token == -1) {
            printf("Error at %c (Hex: %x, Dec: %d)\n", current(b), current(b), current(b));
            break;
        }
        printf("Found: %s, %s\n", token_to_string(token), string_from(b, n));
        // printf("Now at %c (Hex: %x, Dec: %d, Diff: %ld)\n", *str, *str, *str, str - start);
        skip_whitespace(b);
        // printf("After whitespace at %c (Hex: %x, Dec: %d, Diff: %ld)\n\n", *str, *str, *str, str - start);
    }

    return 0;
}