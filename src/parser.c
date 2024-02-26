#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"

const int NT_LEN = SYMBOLS_LEN - TOKENS_LEN;



// Access row index in parse table by using GrammarIndex["NT_name"]
HASHMAP hash_symbols(){
    HASHMAP grammarIndex = create_hashmap(256);
    for (int i = 0; i < SYMBOLS_LEN; i++) {
        insert(grammarIndex, symbols[i], i);
    }
    return grammarIndex;
}

int** create_parse_table() {
    int** parse_table = (int**)malloc(NT_LEN * sizeof(int*));
    for (int i = 0; i < NT_LEN; i++) {
        parse_table[i] = (int*)malloc(TOKENS_LEN * sizeof(int));
        for (int j = 0; j < TOKENS_LEN; j++) {
            parse_table[i][j] = -1;
        }
    }
    return parse_table;
}

int** get_grammar_table(){
    int** grammar_rules = (int**)malloc(PROD_RULE_LEN * sizeof(int*));
    for(int i=0; i<PROD_RULE_LEN; i++){
        grammar_rules[i] = (int*)malloc(15 * sizeof(int));
        for(int j=0; j<15; j++){
            grammar_rules[i][j] = -1;
        }
    }
    return grammar_rules;
}



void get_grammar_rules(int** grammar_rules, HASHMAP grammarIndex){
    FILE* fd = fopen("grammar2.txt", "r");
    if(fd == NULL){
        printf("Error opening grammar file\n");
        return;
    }
    int RULE_SIZE=100;
    char line[RULE_SIZE];
    int row = 0;
    char* delimiters[1] = { ' '};
    while(fgets(line, RULE_SIZE, fd) != NULL){
        int i = 0;
        char* token = strtok(line, " \n");
        while(token != NULL){
            int symbolID = get(grammarIndex, token, strlen(token));
            grammar_rules[row][i++] = symbolID;
            token = strtok(NULL, " \n");
        }
        printf("\n");
        row++;
    }
    fclose(fd);
    return;
}

