#ifndef PTREE_H
#define PTREE_H

#define MAX_SEARCH_LENGTH 50

struct Node
{
    int fwd;
    int freq;
    char cmp;
    char* content;
    struct Node *right;
    struct Node *left;
};

typedef Node* Tree;

struct data {
    Tree t;
    char* buffer;
    char* sugg;
};


Tree createTree(char* content);
Tree insertTree(Tree t, char* w);
bool searchTree(Tree t, char* w, bool inc=false);
void printTree(Tree t);
Tree deleteTree(Tree t, char* w);
void freeTree(Tree t);
void* suggest(void* td);
char* suggest(Tree t, char* w, int start=0);

#endif
