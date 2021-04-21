#ifndef PTREE_H
#define PTREE_H

#define MAX_KEY_SIZE 64
struct Node
{
    int fwd;
    char cmp;
    char* content;
    struct Node *right;
    struct Node *left;
};

typedef Node *Tree;

int strcomp(char* s1, char* s2);
Tree createTree(char* content);
bool insertTree(Tree t, char* w, int start=0);
bool buscaTree(Tree t, char* w, int pos=0);
void printTree(Tree t);

#endif
