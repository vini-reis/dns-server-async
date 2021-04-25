#include "ptree.h"

#ifndef UTILS_H
#define UTILS_H

Tree manualInsert(Tree t, char* w);
bool manualSearch(Tree t, char* w);
Tree manualDelete(Tree t, char* w);
void loadFiles(const char* filename, Tree t, char* w);
char* forceSuggestion(Tree t, char* w);

#endif