#include "tree.h"

TREENODE newNode(){
    TREENODE node = (TREENODE)malloc(sizeof(TreeNode));
    node->symbolId = -1;
    node->isLeaf = 0;
    node->children = (TREENODE*)malloc(sizeof(TREENODE)*15);
    for (int i = 0; i < 15; i++)
    {
        node->children[i] = NULL;
    }
    return node;
}


TREENODE createNode(int symbolId){
    TREENODE node = newNode();
    node->symbolId = symbolId;
    return node;
}

TREENODE populateNode(int* rules_used, int *idx, int** grammar_rules){
    TREENODE node = createNode(grammar_rules[*idx][1]);
    -
    
    return node;
}