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
    char temp;                                                                              // Caractere de leitura do terminal
    char *input = (char*)malloc(sizeof(MAX_SEARCH_LENGTH));                                 // Variável de entrada para carga do arquivo inicial de nomes
    char *last = (char*)malloc(sizeof(MAX_SEARCH_LENGTH));                                  // Variável para comparação de sugestão, a fim de sugerir um nome uma única vez
    Tree t = createTree(NULL);                                                              // Cria árvore PATRICIA com o conteúdo vazio
    pthread_t thread;                                                                       // Instancia uma nova thread para a sugestão de nomes
    pthread_attr_t attr;                                                                    // Instancia atributos para a thread
    int rc, idx;                                                                            // Cód. de erro da criação de threads e índice do buffer
    void *status;                                                                           // Status do 'join' da thread
    struct termios old_tio, new_tio;                                                        // Configs do terminal
    bool outer_end = false, inner_end = false;                                              // Flags de fim de leitura de nomes
        
    data* d = (data*)malloc(sizeof(data));                                                  // Estrutura de dados para a passagem da thread 
    d->t = t;                                                                               // Ponteiro para árvore PATRICIA
    d->buffer = (char*)malloc(sizeof(MAX_SEARCH_LENGTH));;                                  // Buffer de leitura
    d->sugg = (char*)malloc(sizeof(MAX_SEARCH_LENGTH));;                                    // Sugestão
        
    loadFiles("insert.txt", t, input);                                                      // Carrega os dados de exemplo com os sites mais acessados do mundo (Wikipedia)
        
    while(!outer_end){                                                                      // Loop de execução de pesquisa
        pthread_attr_init(&attr);                                                           // Inicialização dos atributos da thread
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);                        // Setando estado nos atributos que a thread pode ser vinculada
        
        tcgetattr(STDIN_FILENO,&old_tio);                                                   // Desliga o modo canonico do terminal para
        new_tio=old_tio;                                                                    // retornar o buffer sem precisar da tecla [Enter]
        new_tio.c_lflag &=(~ICANON & ~ECHO);                                                // e mostrar uma sugestão enquanto o usuário digita
        tcsetattr(STDIN_FILENO,TCSANOW,&new_tio);       

        system("clear");                                                                    // Limpa o terminal
        printf("Press [Enter] when you finish searching, or [Esc] to exit.\n");

        inner_end = false;                                                                  // Inicialização para loop de pesquisa
        idx = 0;

        while(!inner_end) {
            temp = (char) getchar();                                                        // Pega um caractere digitado no terminal

            switch (temp){
                case 127:                                                                   // Caso [Bksp] seja pressionado
                    d->buffer[--idx] = '\0';                                                // Deleta um caractere do buffer
                    system("clear");                                                        // Limpa o terminal
                    printf("%s", d->buffer);                                                // Imprime buffer
                    break;

                case '\e':                                                                  // Caso [Esc] seja pressionado
                    outer_end = inner_end = true;                                           // encerra todos os loops e o programa
                    break;

                case '\n':                                                                  // Caso [Enter] seja pressionado, encerra uma pesquisa
                    if (!searchTree(t, d->buffer)){                                         // Se a palavra buscada não existir na árvore
                        printf("\n\nI didn't find '%s' in the tree. Do you wanna add it? [y]/n ", d->buffer);
                        temp = (char)getchar();
                        if (temp == 'y' || temp == '\n')
                            t = insertTree(t, d->buffer);                                   // Podemos inseri-la
                    }
                    inner_end = true;                                                       // Encerramos o loop de uma pesquisa
                    break;                                                                  // para inicialização das variáveis

                default:                                                                    // Se nenhuma tecla especial for pressionada
                    d->buffer[idx++] = temp;                                                // Adiciona o caractere digitado ao buffer
                    system("clear");                                                        // Limpa o terminal e
                    printf("%s", d->buffer);                                                // mostra o buffer

                    rc = pthread_create(&(thread), &attr, suggest, (void*)d);               // Cria thread de sugestão
                    if (rc) printf("Error creating thread, code: %d\n", rc);

                    rc = pthread_join(thread, &status);                                     // Vinculamos a thread para garantir que ela precisa ser finalizada
                    if (rc) printf("Error joining thread, code: %d\n", rc); 

                    if (d->sugg != NULL && strcmp(d->sugg,last) != 0){                      // Caso haja uma solução que seja diferente
                        printf("\n\nAre you looking for '%s'? [y/n] ", d->sugg);            // da última encontrada, mostramos a solução
                        strcpy(last, d->sugg);                                              // Guarda última sugestão
                        if(((char)getchar()) == 'y')                                        // Se for a palavra buscada
                            inner_end = true;                                               // Encerramos a busca
                        else {
                            system("clear");                                                // Se não, seguimos com a pesquisa
                            printf("%s", d->buffer);
                        }
                    }
            }
        }

        if (d->buffer != NULL)                                                              // Limpeza as váriaveis de texto
            memset(d->buffer, 0, MAX_SEARCH_LENGTH);
        if (d->sugg != NULL)
            memset(d->sugg, 0, MAX_SEARCH_LENGTH);
        if (last != NULL)
            memset(last, 0, MAX_SEARCH_LENGTH);

        tcsetattr(STDIN_FILENO,TCSANOW,&old_tio);                                           // Resetando configurações do terminal

        pthread_attr_destroy(&attr);                                                        // Liberação dos atributos da thread
    }

    input = NULL;
    last = NULL;
    d->sugg = NULL;
    d->buffer = NULL;

    delete [] input;
    delete [] last;
    delete [] d->buffer;
    delete [] d->sugg;

    d = NULL;
    freeTree(t);
    free(d);

    return 0;
}
