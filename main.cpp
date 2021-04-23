#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <ncurses.h>
#include <fstream>

#include "ptree.h"

void* readInput(void* input){
    char* in = (char*) input;
    int i = strlen(in);

    in[i] = getch();

    pthread_exit(NULL);
}

using namespace std;
int main (int argc, char *agrv[]){
    char o, *input = (char*)malloc(sizeof(MAX_SEARCH_LENGTH));
    Tree t = createTree(NULL);
    pthread_t thread [2];
    pthread_attr_t attr;
    int rc;
    void *status;
    fstream file;
    
    dt* data = (dt*)malloc(sizeof(dt));
    data->t = t;
    data->w = input;

    while (o != 'e'){
        printf("Select an option: ");
        scanf(" %c", &o); 
        switch (o){
            case 'i':
                printf("Please, enter the word for input: ");
                scanf(" %s", input);
                t = insertTree(t, input);
            break;

            case 'b':
                printf("Please, enter the word to look for: ");
                scanf(" %s", input);
                if (searchTree(t, input))
                    printf("Found it!\n");
            break;

            case 'd':
                printf("Please, enter the input you want to delete: ");
                scanf(" %s", input);
                t = deleteTree(t, input);
            break;

            case 'p':
                printTree(t);
            break;

            case 'l':
                file.open("insert.txt", ios::in);
                if (!file) printf("Not found!\n");
                else {
                    while (!file.eof()){
                        file >> input;
                        t = insertTree(t, input);
                    }
                }
            break;

            case 't':

            break;

            case 's':
                pthread_attr_init(&attr);
                if (rc) printf("Error creating thread %d", rc);
                pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

                rc = pthread_create(&thread[0], &attr, readInput, (void*)input);
                if (rc) printf("Error creating thread %d", rc);

                rc = pthread_create(&thread[1], &attr, suggest, (void*)data);
                if (rc) printf("Error creating thread %d", rc);

                pthread_attr_destroy(&attr);

                rc = pthread_join(thread[0], &status);
                if (rc) printf("Error joining thread %d", rc);

                rc = pthread_join(thread[1], &status);
                if (rc) printf("Error joining thread %d", rc);


            break;
        }
    }

    freeTree(t);
    delete [] input;
    free(data);

    return 0;
}
