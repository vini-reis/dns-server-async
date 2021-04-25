#include "ptree.h"

#ifndef UTILS_H
#define UTILS_H

struct data {
    Tree t;
    char* buffer;
    char* sugg;
};

// Funções de teste manuais
Tree manualInsert(Tree t, char* w);
bool manualSearch(Tree t, char* w);
Tree manualDelete(Tree t, char* w);
void loadFiles(const char* filename, Tree t, char* w);
char* forceSuggestion(Tree t, char* w);

// Funções utilitárias
int strcomp (char* word, char* cont, int start=0);
char* substring (char* w, int start, int end);
char* concat (char* s1, char* s2);

#endif