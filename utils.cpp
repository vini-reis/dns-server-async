#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <termios.h>
#include <fstream>

#include "ptree.h"

Tree manualInsert(Tree t, char* w){
/* Inserção manual de palavras para teste. */
    printf("Please, enter the word for insert: ");
    scanf(" %s", w);
    return insertTree(t, w);
}

bool manualSearch(Tree t, char* w){
/* Pesquisa manual de palavras para teste. */
    printf("Please, enter the word to look for: ");
    scanf(" %s", w);
    return searchTree(t, w, true);
}

Tree manualDelete(Tree t, char* w){
/* Remoção manual de palavras para teste. */
    printf("Please, enter the input you want to delete: ");
    scanf(" %s", w);
    return deleteTree(t, w);
}

void loadFiles(const char* filename, Tree t, char* w){
/* Carga de arquivos de exemplo na árvore */
    std::fstream file;
    file.open(filename, std::ios::in);
    if (!file) printf("Not found!\n");
    else {
        while (!file.eof()){
            file >> w;
            t = insertTree(t, w);
        }
    }
    file.close();
}

char* forceSuggestion(Tree t, char* w){
/* Busca uma sugestão com base na frequência */
    printf("Gimme something to start looking: ");
    scanf(" %s", w);
    return suggest(t, w, 0);
}

int strcomp (char* word, char* cont, int start=0){
/* 
Retorna a posição do primeiro caracter diferente entre
duas strings. Posição relativa a primeira string, e o
parâmetro 'start' define onde começa a busca na segunda string.
*/
    int i;

    for (i = start; i < strlen(word) && i < strlen(cont); i++)
        if (word[i] != cont[i-start])
            return i;
    return i;
}

char* substring (char* w, int start, int end){
/* Retorna o conjunto de caracteres [start,end) de w. */
    if (start > end || w == NULL) return NULL;
    char* r = (char*) malloc(end-start+1);
    int i = 0;

    for (i = start; i < end && i < strlen(w); i++)
        r[i-start] = w[i];
    r[i] = '\0';

    return r;
}

char* concat (char* s1, char* s2){
/* Retorna a concatenação entre duas strings. */

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