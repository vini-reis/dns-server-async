#include "ptree.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int strcomp (char* word, char* cont, int start=0){
    // Se retornar 0 as palavras já começam diferentes
    // Se retornar tamanho de word 
    int i;
    for (i = start; i < strlen(word) && i < strlen(cont); i++)
        if (word[i] != cont[i-start])
            return i;
    return i;
}

char* substring (char* w, int start, int end){
    if (start >= end || w == NULL) return NULL;

    char* r = (char*) malloc(sizeof(char) * (end-start));

    for (int i = start; i < end && i < strlen(w); i++)
        r[i-start] = w[i];

    return r;
}

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

Tree insertTree (Tree t, char *w, int start){
    // Tree is empty or node is leaf
    if (t->fwd < 0 && t->content == NULL){
        t->content = (char*) malloc(sizeof(w));
        strcpy(t->content,w);
        return t;
    }

    int fwd = strcomp(w, t->content, start);

    // Inserir num nó não folha
    if (t->fwd >= 0){
        if (t->fwd <= fwd){ // Verifica se as palavras se diferem depois da posição de comparação
            if (w[t->fwd] == t->cmp)
                insertTree(t->left,substring(w,t->fwd,strlen(w)));
            else
                insertTree(t->right,substring(w,t->fwd,strlen(w)));
        }
         else {
            Tree new_node = createTree(NULL);
            new_node->left = createTree(NULL);
            insertTree(new_node->left, substring(w,fwd,strlen(w)));
            new_node->right = t;
            new_node->right->content = substring(new_node->right->content,fwd,strlen(new_node->right->content));
            new_node->right->fwd -= fwd;
            t = new_node;
        }
    }

    // Inserir na folha
    else if (t->fwd < 0){
        t->left = createTree(NULL);
        t->right = createTree(NULL);
        insertTree(t->left, substring(w,fwd,strlen(w)));
        insertTree(t->right, substring(t->content,fwd,strlen(t->content)));
    } 

    t->fwd = fwd;
    t->cmp = w[fwd];
    t->content = substring(w, start, fwd);
    return t;
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
    printf("\t%s\n", t->content);
    if (t->left != NULL){
        // printf("Left->");
        printTree(t->left);
    }
    if (t->right != NULL){
        // printf("\tRight->");
        printTree(t->right);
    }
}