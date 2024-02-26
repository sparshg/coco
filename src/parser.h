#ifndef PARSER_H
#define PARSER_H   

#include "tokens.h"

#define PROD_RULE_LEN 90
#define SYMBOLS_LEN 108
#define ERROR -1

static const char symbols[][30] = {
    "TK_ASSIGNOP", "TK_COMMENT", "TK_FIELDID", "TK_ID", "TK_NUM", "TK_RNUM", "TK_FUNID", "TK_RUID", "TK_SQL", "TK_SQR", "TK_COMMA",
    "TK_SEM", "TK_COLON", "TK_DOT", "TK_OP", "TK_CL", "TK_PLUS", "TK_MINUS", "TK_MUL", "TK_DIV", "TK_AND", "TK_OR", "TK_NOT", "TK_LT", "TK_LE", "TK_EQ", "TK_GT",
    "TK_GE", "TK_NE", "TK_WITH", "TK_PARAMETERS", "TK_END", "TK_WHILE", "TK_UNION", "TK_ENDUNION", "TK_DEFINETYPE", "TK_AS", "TK_TYPE", "TK_MAIN", "TK_GLOBAL",
    "TK_PARAMETER", "TK_LIST", "TK_INPUT", "TK_OUTPUT", "TK_INT", "TK_REAL", "TK_ENDWHILE", "TK_IF", "TK_THEN", "TK_ENDIF",
    "TK_READ", "TK_WRITE", "TK_RETURN", "TK_CALL", "TK_RECORD", "TK_ENDRECORD", "TK_ELSE", "#",

    "program", "otherFunctions", "mainFunction", "function", "input_par", "output_par",
    "parameter_list", "dataType", "dataType", "primitiveDatatype", "constructedDatatype",
    "remaining_list", "stmts", "typeDefinitions", "actualOrRedefined", "typeDefinition",
    "fieldDefinition", "moreFields", "declarations", "declaration", "global_or_not", "otherStmts", "stmt",
    "assignmentStmt", "singleOrRecId", "ifRecField", "funCallStmt", "outputParameters",
    "inputParamaters", "iterativeStmt", "conditionalStmt", "elsePart", "ioStmt",
    "arithmeticExpression", "expression'", "term", "term'", "factor", "lowPrecedenceOperators",
    "highPrecedenceOperators", "booleanExpression", "var", "logicalOp", "relationalOp", "returnStmt",
    "optionalReturn", "idList", "more_ids", "definetypestmt", "a",

};

HASHMAP hash_symbols();
int** create_parse_table();
int** get_grammar_table();
void get_grammar_rules(int** grammar_rules, HASHMAP grammarIndex);



#endif