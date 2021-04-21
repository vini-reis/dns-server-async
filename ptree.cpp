#include "ptree.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int strcomp (char* s1, char* s2, int start=0){
    // Se retornar 0 as palavras já começam diferentes
    // Se retornar tamanho de s1 
    int i;
    for (i = start; i < strlen(s1) && i < strlen(s2); i++)
        if (s1[i] != s2[i])
            return i;
    return i;
}

char* substring (char* w, int start, int end); // TODO

Tree createTree(char* content){
    Node* root = (Node*) malloc(sizeof(Node));
    root->content = NULL;
    if (content!=NULL){
        root->content = (char*) malloc(sizeof(content));
        strcpy(root->content,content);
    }
    root->fwd = -1;
    root->cmp = '$';
    root->left = root->right = NULL;
    return root;
}

bool insertTree (Tree t, char *w, int start){
    // Tree is empty or node is leaf
    if (t == NULL){
        t = createTree(w);
        return true;
    }

    int fwd = strcomp(w, t->content, start);
    char* longest_word = strlen(w) > strlen(t->content) ? w : t->content;

    // Uma palavras está totalmente inclusa na outra
    if (fwd < strlen(longest_word) && (start+fwd) < strlen(w)){
        t->fwd = fwd;
        t->cmp = w[fwd];
        insertTree(t->left, substring(w,start+fwd,strlen(w)));
        insertTree(t->right, substring(t->content,start+fwd,strlen(t->content)));
        t->content = substring(t->content, start, start+fwd);
        return true;
    } 
    // 
    else if (fwd == strlen(longest_word) && (start+fwd) >= strlen(w)){
        // TODO:
    }

    return false;
}

bool buscaTree(Tree t, char* w, int pos){
    if (t->fwd >= 0){
        if (w[t->fwd + pos] == t->cmp)
            return buscaTree(t->right, w, t->fwd);
        else
            return buscaTree(t->left, w, t->fwd);
    } else { // Folha
        return strcmp(t->content,w) == 0;
    }
}

void printTree (Tree t){
    if (t->fwd < 0)
        printf("\t%s\n", t->content);
    if (t->left != NULL){
        printf("Left->");
        printTree(t->left);
    }
    if (t->right != NULL){
        printf("\tRight->");
        printTree(t->right);
    }
}