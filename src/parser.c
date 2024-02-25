#include <stdio.h>
#include <stdlib.h>
#include <hashmap.h>

const int NON_TERMINAL_LEN=50;

static const char non_terminals[][30]={
    "program", "otherFunctions", "mainFunction", "function", "input_par", "output_par",
    "parameter_list", "dataType", "dataType", "primitiveDatatype", "constructedDatatype",
    "remaining_list", "stmts", "typeDefinitions", "actualOrRedefined", "typeDefinition", 
    "fieldDefinition", "moreFields", "declarations", "declaration", "global_or_not", "otherStmts", "stmt",
    "assignmentStmt", "singleOrRecId", "ifRecField", "funCallStmt", "outputParameters",
    "inputParamaters", "iterativeStmt", "conditionalStmt", "elsePart", "ioStmt",
    "arithmeticExpression", "expression'", "term", "term'", "factor", "lowPrecedenceOperators", 
    "highPrecedenceOperators", "booleanExpression", "var", "logicalOp", "relationalOp", "returnStmt",
    "optionalReturn", "idList", "more_ids", "definetypestmt", "A"
};


//Access row index in parse table by using NTindex["NT_name"]
HASHMAP hash_non_terminals(){
    HASHMAP NTindex = create_hashmap(50);
    for(int i=0; i<NON_TERMINAL_LEN; i++){
        insert(NTindex, non_terminals[i], i);
    }
    return NTindex;
}