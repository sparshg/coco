#include "lookup.h"
#include "tokens.h"



LOOKUP _InitTable(){
    //Creating a new LookupTable struct
    LOOKUP lookup = (LOOKUP)malloc(sizeof(LookupTable));


    //Initialising LookupTable fields
    lookup->table = (Record)malloc(29 * sizeof(TableEntry));
    lookup->length=0;
    lookup->size=29;

    //Keywords
    int p=0;
    
    lookup->table[p++] = (TableEntry){.lexeme = "with", .token = TK_WITH};
    lookup->table[p++] = (TableEntry){.lexeme = "parameters", .token = TK_PARAMETERS};
    lookup->table[p++] = (TableEntry){.lexeme = "end", .token = TK_END};
    lookup->table[p++] = (TableEntry){.lexeme = "while", .token = TK_WHILE};
    lookup->table[p++] = (TableEntry){.lexeme = "union", .token = TK_UNION};
    lookup->table[p++] = (TableEntry){.lexeme = "endunion", .token = TK_ENDUNION};
    lookup->table[p++] = (TableEntry){.lexeme = "definetype", .token = TK_DEFINETYPE};
    lookup->table[p++] = (TableEntry){.lexeme = "as", .token = TK_AS};
    lookup->table[p++] = (TableEntry){.lexeme = "type", .token = TK_TYPE};
    lookup->table[p++] = (TableEntry){.lexeme = "_main", .token = TK_MAIN};
    lookup->table[p++] = (TableEntry){.lexeme = "global", .token = TK_GLOBAL};
    lookup->table[p++] = (TableEntry){.lexeme = "parameter", .token = TK_PARAMETER};
    lookup->table[p++] = (TableEntry){.lexeme = "list", .token = TK_LIST};
    lookup->table[p++] = (TableEntry){.lexeme = "input", .token = TK_INPUT};
    lookup->table[p++] = (TableEntry){.lexeme = "output", .token = TK_OUTPUT};
    lookup->table[p++] = (TableEntry){.lexeme = "int", .token = TK_INT};
    lookup->table[p++] = (TableEntry){.lexeme = "real", .token = TK_REAL};
    lookup->table[p++] = (TableEntry){.lexeme = "endwhile", .token = TK_ENDWHILE};
    lookup->table[p++] = (TableEntry){.lexeme = "if", .token = TK_IF};
    lookup->table[p++] = (TableEntry){.lexeme = "then", .token = TK_THEN};
    lookup->table[p++] = (TableEntry){.lexeme = "endif", .token = TK_ENDIF};
    lookup->table[p++] = (TableEntry){.lexeme = "read", .token = TK_READ};
    lookup->table[p++] = (TableEntry){.lexeme = "write", .token = TK_WRITE};
    lookup->table[p++] = (TableEntry){.lexeme = "return", .token = TK_RETURN};
    lookup->table[p++] = (TableEntry){.lexeme = "call", .token = TK_CALL};
    lookup->table[p++] = (TableEntry){.lexeme = "record", .token = TK_RECORD};
    lookup->table[p++] = (TableEntry){.lexeme = "else", .token = TK_ELSE};
    lookup->table[p++] = (TableEntry){.lexeme = "with", .token = TK_WITH};
    

    
    lookup->length=p;


    return lookup;
}

int _FindLexeme (LOOKUP lookup, string lexeme) {

    for(int i=0;i<lookup->length;i++){

        if(strcmp(lookup->table[i].lexeme, lexeme)==0){
            return i;
        }
    }

    return -1;
}

int _InsertLexeme(LOOKUP lookup, string lexeme, int token){
    int index = _FindLexeme(lookup, lexeme);
    if(index >= 0){
        return index;
    }
    else{
        if(lookup->length >= lookup->size){
            // Reallocation
            printf("R\n");
            int new_size = (lookup->size * 3)/2;
            // string* temp = lookup->table;
            lookup->table = (Record)realloc(lookup->table, new_size * sizeof(TableEntry));
            lookup->size = new_size;

        }

        lookup->table[lookup->length++] = (TableEntry){.lexeme = lexeme, .token = token};
        return lookup->length - 1;
    }
}

// int main(){
//     LOOKUP lookup = _InitTable();


//     // // Code for Debugging LOOKUP_TABLE
//     printf("%d\n", lookup->length);
//     printf("%d\n", lookup->size);
//     for(int i=0;i<lookup->length;i++){
//         printf("%s %d\n", lookup->table[i].lexeme, lookup->table[i].token);
//     }

//     printf("\n");
//     _InsertLexeme(lookup, "d2b4", TK_ID);
//     printf("%d\n", lookup->length);
//     printf("%d\n", lookup->size);
//     for(int i=0;i<lookup->length;i++){
//         printf("%s %d\n", lookup->table[i].lexeme, lookup->table[i].token);
//     }

//     printf("\n");
//     printf("%d\n", _InsertLexeme(lookup, "abcd", TK_FUNID));
//     printf("%d\n", lookup->length);
//     printf("%d\n", lookup->size);
//     for(int i=0;i<lookup->length;i++){
//         printf("%s %d\n", lookup->table[i].lexeme, lookup->table[i].token);
//     }

//     // Testing _FindLexeme
//     printf("%d\n", _FindLexeme(lookup, "call"));
//     printf("%d\n", _FindLexeme(lookup, "calls"));

//     return 0;
// }