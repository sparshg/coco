#ifndef TREE_H
#define TREE_H

#include <stdio.h>
#include <stdlib.h>

typedef struct TreeNode TreeNode;
typedef struct TreeNode* TREENODE;

struct TreeNode{
    int symbolId;
    int isLeaf;
    TREENODE* children;
};






#endif