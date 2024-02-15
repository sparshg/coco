#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "lookup.h"



LOOKUP _InitTable(){
    //Creating a new LookupTable struct
    LOOKUP lookup = (LOOKUP)malloc(sizeof(struct LookupTable));


    //Initialising LookupTable fields
    lookup->table = (string*)malloc(29 * sizeof(string));
    lookup->length=0;
    lookup->size=29;

    //Keywrords
    int p=0;
    lookup->table[p++]="with";
    lookup->table[p++]="end";
    lookup->table[p++]="parameters";
    lookup->table[p++]="while";
    lookup->table[p++]="union";
    lookup->table[p++]="endunion";
    lookup->table[p++]="definetype";
    lookup->table[p++]="as";
    lookup->table[p++]="type";
    lookup->table[p++]="_main";
    lookup->table[p++]="global";
    lookup->table[p++]="parameter";
    lookup->table[p++]="list";
    lookup->table[p++]="input";
    lookup->table[p++]="output";
    lookup->table[p++]="int";
    lookup->table[p++]="endwhile";
    lookup->table[p++]="real";
    lookup->table[p++]="if";
    lookup->table[p++]="then";
    lookup->table[p++]="endif";
    lookup->table[p++]="read";
    lookup->table[p++]="write";
    lookup->table[p++]="return";
    lookup->table[p++]="call";
    lookup->table[p++]="record";
    lookup->table[p++]="endrecord";
    lookup->table[p++]="else";
    
    lookup->length=p;


    return lookup;
}

bool _FindLexeme(LOOKUP lookup, string lexeme){
    bool found = false;

    for(int i=0;i<lookup->length;i++){
        if(strcmp(lookup->table[i], lexeme)==0){
            found = true;
            break;
        }
    }

    return found;
}

void _InsertLexeme(LOOKUP lookup, string lexeme){
    if(_FindLexeme(lookup, lexeme)){
        return;
    }
    else{
        if(lookup->length < lookup->size){
            lookup->table[lookup->length++]=lexeme;
        }
        else{
            // Reallocation
            int new_size = (lookup->size * 3)/2;
            string* temp = lookup->table;
            lookup->table = (string*)realloc(lookup->table, new_size * sizeof(string));
            lookup->size = new_size;

            lookup->table[lookup->length++]=lexeme;
        }
    }
}

// int main(){
//     LOOKUP lookup = _InitTable();


//     //Code for Debugging LOOKUP_TABLE
//     // printf("%d\n", lookup->length);
//     // printf("%d\n", lookup->size);
//     // for(int i=0;i<lookup->length;i++){
//     //     printf("%s\n", lookup->table[i]);
//     // }

//     // printf("\n");
//     // _InsertLexeme(lookup, "d2b4");
//     // printf("%d\n", lookup->length);
//     // printf("%d\n", lookup->size);
//     // for(int i=0;i<lookup->length;i++){
//     //     printf("%s\n", lookup->table[i]);
//     // }

//     // printf("\n");
//     // _InsertLexeme(lookup, "abcd");
//     // printf("%d\n", lookup->length);
//     // printf("%d\n", lookup->size);
//     // for(int i=0;i<lookup->length;i++){
//     //     printf("%s\n", lookup->table[i]);
//     // }

//     return 0;
// }