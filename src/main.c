#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "buffer.h"
#include "hashmap.h"
#include "lexer.h"
#include "tokens.h"
#include "parser.h"
int main(int argc, char* argv[]) {
    BUF b = read_file("hi.txt");

    HASHMAP table = create_keyword_table();

    while (current(b) != EOF) {
        clear_saves(b);
        int n = push_state(b);
        int token = try_all(b, table);
        if (token == -1) {
            printf("Unknown: %s\n", string_from(b, n));
            // printf("Error at %c (Hex: %x, Dec: %d)\n", current(b), current(b), current(b));
            break;
        } else {
            printf("Found: %s, %s\n", token_to_string(token), string_from(b, n));
        }
        // printf("Now at %c (Hex: %x, Dec: %d, Diff: %ld)\n", *str, *str, *str, str - start);
        skip_whitespace(b);
        // printf("After whitespace at %c (Hex: %x, Dec: %d, Diff: %ld)\n\n", *str, *str, *str, str - start);
    }


    // GrammarTable Testing Code
    // HASHMAP grammarIndex = hash_symbols();
    // int** grammar_rules = get_grammar_table();
    // get_grammar_rules(grammar_rules, grammarIndex);

    // for(int i=0; i<90; i++){
    //     printf("%d ", i);
    //     for(int j=0; j<15; j++){
    //         if(grammar_rules[i][j] == -1)
    //             continue;
    //         printf("%d ", grammar_rules[i][j]);
    //     }
    //     printf("\n");
    // }
    

    return 0;
}