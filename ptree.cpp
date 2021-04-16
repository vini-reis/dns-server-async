#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ptree.h>

Tree treeCtr() {
    Node *root = (Node*) malloc(sizeof(Node));
    root->key = (bool*) malloc(MAX_KEY_SIZE);
    root->left = root->right = root;
    root->digit = -1;
    return root;
}

Node* search (Tree r, Key key){
    Node *target;
    target = searchR(r->left, key, -1);
    return target->key == key ? target : NULL;
}

Node* searchR(Tree r, Key key, int prev_digit){
    if (r->digit <= prev_digit) return r;
}
