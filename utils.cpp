#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <termios.h>
#include <fstream>

#include "ptree.h"

Tree manualInsert(Tree t, char* w){
    printf("Please, enter the word for insert: ");
    scanf(" %s", w);
    return insertTree(t, w);
}

bool manualSearch(Tree t, char* w){
    printf("Please, enter the word to look for: ");
    scanf(" %s", w);
    return searchTree(t, w, true);
}

Tree manualDelete(Tree t, char* w){
    printf("Please, enter the input you want to delete: ");
    scanf(" %s", w);
    return deleteTree(t, w);
}

void loadFiles(const char* filename, Tree t, char* w){
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
    printf("Gimme something to start looking: ");
    scanf(" %s", w);
    return suggest(t, w);
}