#ifndef PTREE_H
#define PTREE_H

#define MAX_KEY_SIZE 64

struct Node
{
    Key key;
    char* content;
    int digit;
    struct Node *right;
    struct Node *left;
};

typedef Node *Tree;
typedef bool* Key;

int bin2dec(bool* b, int size){
    int n = 0;

    for (int i = 1; i <= size; i++)
        n += b[i] ? i*10 : 0;

    return n;
}

bool cmpKeys (Key k1, Key k2);

#endif