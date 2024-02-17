#include "first.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tokens.h"

GRAMMAR createGrammar(char** terminals, char** non_terminals, char* start_symbol, char** productions, int num_productions, int num_terminals, int num_non_terminals) {
    GRAMMAR G = malloc(sizeof(struct Grammar));
    G->num_terminals = num_terminals;
    G->num_non_terminals = num_non_terminals;
    G->num_productions = num_productions;
    G->start_symbol = start_symbol;

    G->terminals = malloc(num_terminals * sizeof(Token));
    for (int i = 0; i < num_terminals; i++) {
        G->terminals[i] = string_to_token(terminals[i]);
    }

    G->non_terminals = malloc(num_non_terminals * sizeof(char*));
    for (int i = 0; i < num_non_terminals; i++) {
        G->non_terminals[i] = non_terminals[i];
    }

    G->productions = malloc(num_productions * sizeof(Token*));
    for (int i = 0; i < num_productions; i++) {
        // G->productions[i] = malloc(MAX_PRODUCTION_LEN * sizeof(Token));
        // char* production = productions[i];
        // int j = 0;
        // while (*production != '\0') {
        //     char* start = production;
        //     Token token = try_all(&production);
        //     if (token == -1) {
        //         printf("Error at %c (Hex: %x, Dec: %d, Diff: %ld)\n", *production, *production, *production, production - start);
        //         break;
        //     }
        //     G->productions[i][j++] = token;
        //     skip_whitespace(&production);
        // }
        // G->productions[i][j] = -1;
    }
}