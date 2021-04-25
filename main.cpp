#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <termios.h>
#include <fstream>

#include "ptree.h"

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
    fstream file;
    struct termios old_tio, new_tio;
    bool finish = false;

    data* d = (data*)malloc(sizeof(data));
    d->t = t;
    d->buffer = (char*)malloc(sizeof(MAX_SEARCH_LENGTH));;
    d->sugg = (char*)malloc(sizeof(MAX_SEARCH_LENGTH));;

    while (o != 'e'){
        printf("Select an option: ");
        scanf(" %c", &o); 
        getchar();
        switch (o){
            case 'i':
                printf("Please, enter the word for input: ");
                scanf(" %s", input);
                t = insertTree(t, input);
            break;

            case 'b':
                printf("Please, enter the word to look for: ");
                scanf(" %s", input);
                if (searchTree(t, input, true))
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
                scanf(" %s", input);
                printf("Você procura por '%s'?\n", suggest(t, input));
            break;

            case 'e':
                
            break;

            case 's':
                while(1){
                    pthread_attr_init(&attr);
                    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

                    // Desliga o modo canonico do terminal 
                    tcgetattr(STDIN_FILENO,&old_tio);
                    new_tio=old_tio;
                    new_tio.c_lflag &=(~ICANON & ~ECHO);
                    tcsetattr(STDIN_FILENO,TCSANOW,&new_tio);

                    printf("Press [Enter] when you finish your search?\n");

                    // d = (data*) malloc(sizeof(data));
                    idx = 0;
                    // d->buffer[idx] = '\0';
                    // d->sugg[idx] = '\0';
                    // last[idx] = '\0';
                    finish = false;

                    while(!finish) {
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
                        } else {
                            if (!searchTree(t, d->buffer)){
                                printf("I didn't find '%s' in the tree. Do you wanna add it? ([y]/n)\n\n", d->buffer);
                                temp = (char)getchar();
                                if (temp == 'y' || temp == '\n')
                                    t = insertTree(t, d->buffer);
                                finish = true;
                            }
                        }

                    }

                    // free(d->sugg);
                    // // d->sugg = NULL;
                    // free(d->buffer);
                    // // free(d);
                    // // d->buffer = NULL;
                    // free(last);
                    // last = NULL;
                    if (d->buffer != NULL)
                        memset(d->buffer, 0, sizeof(d->buffer));
                    if (d->sugg != NULL)
                        memset(d->sugg, 0, sizeof(d->sugg));
                    if (last != NULL)
                        memset(last, 0, sizeof(last));

                    tcsetattr(STDIN_FILENO,TCSANOW,&old_tio);

                    pthread_attr_destroy(&attr);
                }
            break;
        }
    }

    delete [] input;
    delete [] last;
    free(d->buffer);
    free(d->sugg);
    free(d);
    freeTree(t);

    return 0;
}
