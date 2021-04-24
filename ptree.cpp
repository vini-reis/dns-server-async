#include "ptree.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>

int strcomp (char* word, char* cont, int start=0){
    int i;

    for (i = start; i < strlen(word) && i < strlen(cont); i++)
        if (word[i] != cont[i-start])
            return i;
    return i;
}

char* substring (char* w, int start, int end){
    if (start > end || w == NULL) return NULL;
    char* r = (char*) malloc(end-start+1);
    int i = 0;

    for (i = start; i < end && i < strlen(w); i++)
        r[i-start] = w[i];
    r[i] = '\0';

    return r;
}

char* concat (char* s1, char* s2){
    if (s1 == NULL && s2 == NULL) return NULL;

    int size;
    if (s2 == NULL)
        size = strlen(s1);
    else 
        size = strlen(s1) + strlen(s2);

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
    root->freq = 0;
    free(content);
    return root;
}

Tree insertTree (Tree t, char *w){

    // Tree is empty or node is leaf
    if (t->fwd < 0 && t->content == NULL){
        t->content = (char*) malloc(sizeof(w));
        strcpy(t->content,w);
        return t;
    }

    if (searchTree(t,w)) return t;

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
    // delete [] w;
    return t;
}

bool searchTree(Tree t, char* word, bool inc){
    if (t == NULL) return false;

    int fwd = strcomp(word, t->content);

    if (fwd < t->fwd && t->fwd >= 0) return false;

    if (inc) t->freq++; // Incrementa a frequência pois a busca passou por este nó

    if (fwd >= t->fwd && t->fwd >= 0){
        if (word[t->fwd] == t->cmp){
            return searchTree(t->left, substring(word,fwd,strlen(word)));
        }
        else {
            return searchTree(t->right, substring(word,fwd,strlen(word)));
        }
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

Tree deleteTree (Tree t, char* w){
    if (!searchTree(t, w)) return t;

    int fwd = strcomp(w, t->content);

    if (fwd >= t->fwd && t->fwd >= 0){
        if (w[t->fwd] == t->cmp)
            if (!searchNode(t->left, substring(w,fwd,strlen(w)))){
                t->left = deleteTree(t->left, substring(w,fwd,strlen(w)));
                return t;
            }
            else {
                t->right->content = concat(t->content, t->right->content);
                t->right->fwd = t->right->fwd < 0 ? -1 : t->right->fwd + t->fwd;
                // free(t->left->content);
                delete [] t->left->content;
                free(t->left);
                t = t->right;
            }
        else {
            if (!searchNode(t->right, substring(w,fwd,strlen(w)))){
                t->right = deleteTree(t->right, substring(w,fwd,strlen(w)));
                return t;
            }
            else {
                t->left->content = concat(t->content, t->left->content);
                t->left->fwd = t->left->fwd < 0 ? -1 : t->left->fwd + t->fwd;
                // free(t->right->content);
                delete [] t->right->content;
                free(t->right);
                t = t->left;
            }
        }

        printf("Deleted!\n");
        return t;
    } else if (searchNode(t,w)){
        // free(t->content);
        delete [] t->content;
        printf("Deleted!\n");
        return t;
    }

    printf("Check!!!\n");
    return t;
}

void freeTree(Tree t){
    if (t->left != NULL) freeTree(t->left);
    if (t->right != NULL) freeTree(t->right);
    // free(t->content);
    if (t->content != NULL)
        delete [] t->content;
    free(t);
}

char* complete(Tree t, char* w){
    w = concat(w,t->content);

    if (t->fwd < 0) return w;

    if (t->left->freq >= t->right->freq)
        w = complete(t->left,w); 
    else
        w = complete(t->right,w); 
    return w;
}

void* suggest(void* td){
    data* d = (data*) td;
    d->sugg = suggest(d->t, d->buffer);

    d->sugg = d->sugg != NULL && searchTree(d->t, d->sugg)? d->sugg : NULL;

    pthread_exit(NULL);
}

char* suggest(Tree t, char* w, int start){
    // FIXME: Resolver casos de sugestão...
    if (t == NULL) return NULL;

    if (searchTree(t, w)) return NULL; // False para inc freq

    // Busca em qual nó a palavra termina
    int fwd = strcomp(w, t->content, start);

    if (fwd == 0 && t->fwd != 0 && start == 0 && t->content != NULL) return NULL;

    if (start+t->fwd <= strlen(w)){
        t->freq++; // Incrementa a frequencia de busca

        if (w[start+t->fwd] == t->cmp)
            w = suggest(t->left, w, start+t->fwd);
        else
            w = suggest(t->right, w, start+t->fwd);
        return w;
    } else {
        w = concat(w,substring(t->content,fwd-start,strlen(t->content)));
        if (t->left->freq >= t->right->freq)
            w = complete(t->left, w);
        else
            w = complete(t->right, w);
        return w;
    }
}