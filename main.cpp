#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <termios.h>
#include <fstream>

#include "ptree.h"
#include "utils.h"

using namespace std;
int main (int argc, char *agrv[]){
    char o, temp;
    char *input = (char*)malloc(sizeof(MAX_SEARCH_LENGTH));
    char *last = (char*)malloc(sizeof(MAX_SEARCH_LENGTH));
    Tree t = createTree(NULL);
    pthread_t thread;
    pthread_attr_t attr;
    int rc, idx;
    void *status;
    struct termios old_tio, new_tio;
    bool finish = false;

    data* d = (data*)malloc(sizeof(data));
    d->t = t;
    d->buffer = (char*)malloc(sizeof(MAX_SEARCH_LENGTH));;
    d->sugg = (char*)malloc(sizeof(MAX_SEARCH_LENGTH));;

    loadFiles("insert.txt", t, input);

    while(!finish){
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

        // Desliga o modo canonico do terminal 
        tcgetattr(STDIN_FILENO,&old_tio);
        new_tio=old_tio;
        new_tio.c_lflag &=(~ICANON & ~ECHO);
        tcsetattr(STDIN_FILENO,TCSANOW,&new_tio);

        printf("Press [Enter] when you finish your search?\n");

        idx = 0;
        finish = false;

        while(true) {
            temp = (char) getchar();
            if (temp != '\n'){
                d->buffer[idx++] = temp;
                printf("%s\n", d->buffer);

                rc = pthread_create(&(thread), &attr, suggest, (void*)d);
                if (rc) printf("Error creating thread, code: %d\n", rc);

                rc = pthread_join(thread, &status);
                if (rc) printf("Error joining thread, code: %d\n", rc); 

                if (d->sugg != NULL && strcmp(d->sugg,last) != 0){
                    printf("\nAre you looking for '%s'? [y/n]\n", d->sugg);
                    strcpy(last, d->sugg);
                    if(((char)getchar()) == 'y') break;
                }
            } else if (temp == '\b'){
                d->buffer[--idx] = '\0';
                printf("%s\n", d->buffer);
            } else if (temp == '\e'){
                finish = true;
                break;
            } else {
                if (!searchTree(t, d->buffer)){
                    printf("I didn't find '%s' in the tree. Do you wanna add it? ([y]/n)\n\n", d->buffer);
                    temp = (char)getchar();
                    if (temp == 'y' || temp == '\n')
                        t = insertTree(t, d->buffer);
                    break;
                }
            }

        }

        if (d->buffer != NULL)
            memset(d->buffer, 0, MAX_SEARCH_LENGTH);
        if (d->sugg != NULL)
            memset(d->sugg, 0, MAX_SEARCH_LENGTH);
        if (last != NULL)
            memset(last, 0, MAX_SEARCH_LENGTH);

        tcsetattr(STDIN_FILENO,TCSANOW,&old_tio);

        pthread_attr_destroy(&attr);
    }

    delete [] input;
    delete [] last;
    free(d->buffer);
    free(d->sugg);
    free(d);
    freeTree(t);

    return 0;
}
