#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "buffer.h"
#include "hashmap.h"
#include "lexer.h"
#include "parser.h"
#include "tokens.h"
#include "stack.h"

int main(int argc, char* argv[]) {
    BUF b = read_file("hi.txt");

    HASHMAP table = create_keyword_table();
    HASHMAP symbol_map = create_symbol_map();

    // remove_comments("hi.txt", "clean.txt");

    while (current(b) != EOF) {
        clear_saves(b);
        int n = push_state(b);
        int token = try_all(b, table);
        if (token == -1) {
            // printf("Unknown: %s\n", string_from(b, n));
            // printf("Error at %c (Hex: %x, Dec: %d)\n", current(b), current(b), current(b));
            break;
        } else {
            // printf("Found: %s, %s\n", token_to_string(token), string_from(b, n));
        }
        // printf("Now at %c (Hex: %x, Dec: %d, Diff: %ld)\n", *str, *str, *str, str - start);
        skip_whitespace(b);
        // printf("After whitespace at %c (Hex: %x, Dec: %d, Diff: %ld)\n\n", *str, *str, *str, str - start);
    }

    // GrammarTable Testing Code
    int** grammar_rules = get_grammar_rules(symbol_map);

    // for (int i = 0; i < PROD_RULES_LEN; i++) {
    // printf("%d ", i);
    //     for (int j = 0; j < PROD_RULE_LINE_LEN; j++) {
    //         if (grammar_rules[i][j] == -1)
    //             break;
    //         printf("%d ", grammar_rules[i][j]);
    //     }
    //     printf("\n");
    // }
    // int** parse_table = get_parse_table(grammar_rules, symbol_map);
    // print_parse_table(parse_table);

    // for(int i=0;i<PROD_RULES_LEN;i++){
    //     printf("%d: ",i);
    //     for(int j=0;j<PROD_RULE_LINE_LEN;j++){
    //         printf("%d ",grammar_rules[i][j]);
    //     }
    //     printf("\n");
    // }

    return 0;
}