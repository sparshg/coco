#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "buffer.h"
#include "hashmap.h"
#include "lexer.h"
#include "tokens.h"

int main(int argc, char* argv[]) {
    char* str = "203\n203\n<--->===_hi/_main([])>=#a00.00E+09 <--- with end hi ";
    char* start;

    BUF b = read_file("hi.txt");
    for (int i = 0; i < 30; i++) {
        printf("%d\n", b->b[b->curr][i]);
    }

    // HASHMAP table = create_keyword_table();

    // while (*str != '\0') {
    //     start = str;
    //     int token = try_all(&str, table);
    //     if (token == -1) {
    //         printf("Error at %c (Hex: %x, Dec: %d, Diff: %ld)\n", *str, *str, *str, str - start);
    //         break;
    //     }
    //     printf("Found: %s, %.*s\n", token_to_string(token), (int)(str - start), start);
    //     // printf("Now at %c (Hex: %x, Dec: %d, Diff: %ld)\n", *str, *str, *str, str - start);
    //     skip_whitespace(&str);
    //     // printf("After whitespace at %c (Hex: %x, Dec: %d, Diff: %ld)\n\n", *str, *str, *str, str - start);
    // }

    return 0;
}