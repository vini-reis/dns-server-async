# Servidor de Nomes Assíncrono, utilizando árvore PATRICIA

**Universidade Federal do ABC**
*Algortimos e Estruturas de Dados II*

**Professor:** Fernando Teubl
**Autor:** Vinícius Reis
**RA:** 11041416

## Introdução

O Servidor de Nomes Assíncrono foi criado como Projeto Final da disciplina **Algoritmos e Estruturas de Dados II**.

## Descrição

A ideia do projeto é desenvolver um Servidor de Nomes que utilizasse uma estrutura de dados que é a árvore PATRICIA. A aplicação fornece sugestões de nomes baseado na frequência com que cada nó foi buscado. É levado em consideração a passagem por cada trecho do nó para entregar uma sugestão e, ao final da busca, se a palavra não for encontrada, ela pode ser inserida na árvore.

## Manual de uso

Para o compilar a aplicação a partir do código fonte basta executar o comando:

```bash
gcc main.cpp ptree.cpp utils.cpp -o ptree.cc -lpthread
```

Em seguida, para rodar a aplicação, pode-se utilizar o comando:

```bash
./ptree.cc
```
