#ifndef ARVORE_H
#define ARVORE_H

#include <stdio.h>
#include "fila.h"

// retorna o byte que ta guardado no item do no
unsigned char pegar_byte(No *no);

// retorna 1 se o no nao tiver filhos
int eh_folha(No *no);

// tira os dois nos de menor frequencia da fila e cria um pai '*' pra eles,
// com frequencia igual a soma dos dois. o pai volta pra fila e isso
// se repete ate sobrar so um no, que vira a raiz
No *montar_arvore(Fila *fila);

// conta quantos bytes a arvore ocupa no arquivo (contando os '\' de escape)
// esse numero vai no cabecalho
int tamanho_arvore(No *raiz);

// escreve a arvore em pre-ordem no arquivo
// se uma folha for '*' ou '\', escreve um '\' antes pra nao confundir com no interno
void escrever_arvore(No *raiz, FILE *arquivo);

// le a arvore em pre-ordem do arquivo (ela vem logo depois do cabecalho)
// e remonta ela na memoria
No *ler_arvore(FILE *arquivo);

// libera todos os nos da arvore
void liberar_arvore(No *raiz);

#endif
