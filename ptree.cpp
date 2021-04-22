#include "ptree.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int strcomp (char* word, char* cont){
    int i;

    for (i = 0; i < strlen(word) && i < strlen(cont); i++)
        if (word[i] != cont[i])
            return i;
    return i;
}

char* substring (char* w, int start, int end){
    if (start > end || w == NULL) return NULL;
    if (start == end) return "";

    char* r = (char*) malloc(end-start);

    for (int i = start; i < end && i < strlen(w); i++)
        r[i-start] = w[i];

    return r;
}

char* concat (char* s1, char* s2){
    if (s1 == NULL && s2 == NULL) return NULL;

    int size = strlen(s1) + strlen(s2);
    char* r = (char*) malloc(sizeof(size));

    for (int i = 0; i < strlen(s1); i++)
        r[i] = s1[i];
    for (int i = strlen(s1); i < size; i++)
        r[i] = s2[i-strlen(s1)];

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

Tree insertTree (Tree t, char *w){

    // Tree is empty or node is leaf
    if (t->fwd < 0 && t->content == NULL){
        t->content = (char*) malloc(sizeof(w));
        strcpy(t->content,w);
        return t;
    }

    if (buscaTree(t,w)) return t;

    int fwd = strcomp(w, t->content);

    // Inserir num nó não folha
    if (t->fwd >= 0){
        if (t->fwd <= fwd){ // Verifica se as palavras se diferem depois da posição de comparação
            if (w[t->fwd] == t->cmp)
                t->left = insertTree(t->left,substring(w,t->fwd,strlen(w)));
            else
                t->right = insertTree(t->right,substring(w,t->fwd,strlen(w)));
            return t;
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
    t->content = substring(w, 0, fwd);
    return t;
}

bool buscaTree(Tree t, char* word){
    if (t == NULL) return false;

    int fwd = strcomp(word, t->content);

    // if (fwd < 0 && t->content[0] == '\0') return true;
    if (fwd < t->fwd && t->fwd >= 0) return false;

    else if (fwd >= t->fwd && t->fwd >= 0){
        if (word[t->fwd] == t->cmp)
            return buscaTree(t->left, substring(word,fwd,strlen(word)));
        else
            return buscaTree(t->right, substring(word,fwd,strlen(word)));
    } 

    else if (t->fwd < 0) // Folha
        return strcmp(word,t->content) == 0;

    return false;
}

void printTree (Tree t){
    printf(" %s\n", t->content);
    if (t->left != NULL)
        printTree(t->left);
    if (t->right != NULL)
        printTree(t->right);
}

bool searchNode(Node* t, char* w){
    return strcmp(t->content,w) == 0;
}

bool deleteTree (Tree t, char* w){
    if (!buscaTree(t, w)) return false;

    int fwd = strcomp(w, t->content);

    if (fwd >= t->fwd && t->fwd >= 0){
        if (w[t->fwd] == t->cmp)
            if (!searchNode(t->left, substring(w,fwd,strlen(w))))
                return deleteTree(t->left, substring(w,fwd,strlen(w)));
            else {
                t->content = concat(t->content, t->right->content);
                t->fwd += t->right->fwd; // FIXME: Se o nó filho for folha, estraga o valor do paii
                free(t->left->content);
                free(t->left);
                t = t->right;
                return true;
            }
        else {
            if (!searchNode(t->right, substring(w,fwd,strlen(w))))
                return deleteTree(t->right, substring(w,fwd,strlen(w)));
            else {
                t->content = concat(t->content, t->left->content);
                t->fwd += t->left->fwd;
                free(t->left->content);
                free(t->left);
                t->left = NULL;
                return true;
            }
        }
    } else if (searchNode(t,w)){
        free(t->content);
        t->content = NULL;
        return true;
    }

    printf("Check!!!\n");
    return false;
}