#ifndef ARVORE_H
#define ARVORE_H

#include <stdio.h>
#include "fila.h"

// retorna o byte que ta guardado no item do no
unsigned char pegar_byte(No *no);

// retorna 1 se o no nao tiver filhos
int eh_folha(No *no);

// tira os dois nos de menor frequencia da fila e cria um pai '*'
No *montar_arvore(Fila *fila);

// conta quantos bytes a arvore ocupa no arquivo
int tamanho_arvore(No *raiz);

// escreve a arvore em pre-ordem no arquivo
void escrever_arvore(No *raiz, FILE *arquivo);

// le a arvore em pre-ordem do arquivo
No *ler_arvore(FILE *arquivo);

// libera todos os nos da arvore
void liberar_arvore(No *raiz);

#endif
