#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "ptree.h"

int main (int argc, char *agrv[]){
    char o, *word;
    Tree t = createTree(NULL);
    // t = NULL;

    while (o != 'e'){
        printf("Select an option: ");
        scanf(" %c", &o); 
        switch (o){
            case 'i':
                printf("Please, enter the word for input: ");
                scanf(" %s", word);
                t = insertTree(t, word);
            break;
            case 'b':
                printf("Please, enter the word to look for: ");
                scanf(" %s", word);
                if (buscaTree(t, word))
                    printf("Found it!\n");
            break;
            case 'p':
                printTree(t);
            break;
        }
    }

    return 0;
}