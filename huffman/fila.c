#include <stdio.h>
#include <stdlib.h>
#include "fila.h"

// --- no ---

No *criar_no(unsigned char byte, int frequencia, No *esquerda, No *direita)
{
    No *novo = malloc(sizeof(No));

    unsigned char *item = malloc(sizeof(unsigned char));
    *item = byte;

    novo->item = item;
    novo->frequencia = frequencia;
    novo->proximo = NULL;
    novo->esquerda = esquerda;
    novo->direita = direita;
    return novo;
}

// --- fila ---

Fila *criar_fila(int (*comparar)(void *a, void *b))
{
    Fila *fila = malloc(sizeof(Fila));
    fila->inicio = NULL;
    fila->tamanho = 0;
    fila->comparar = comparar;
    return fila;
}

int comparar_frequencia(void *a, void *b)
{
    No *no_a = (No *)a;
    No *no_b = (No *)b;

    if (no_a->frequencia < no_b->frequencia) {
        return -1;
    }
    if (no_a->frequencia > no_b->frequencia) {
        return 1;
    }
    return 0;
}

void inserir_ordenado(Fila *fila, No *novo)
{
    // se a fila ta vazia ou o novo for menor ou igual ao primeiro, ele entra no comeco
    if (fila->inicio == NULL || fila->comparar(novo, fila->inicio) <= 0) {
        novo->proximo = fila->inicio;
        fila->inicio = novo;
    } else {
        // senao, anda ate achar o lugar certo e encaixa ele ali
        No *atual = fila->inicio;
        while (atual->proximo != NULL && fila->comparar(atual->proximo, novo) < 0) {
            atual = atual->proximo;
        }
        novo->proximo = atual->proximo;
        atual->proximo = novo;
    }
    fila->tamanho++;
}

No *remover_inicio(Fila *fila)
{
    No *removido = fila->inicio;
    fila->inicio = removido->proximo;
    removido->proximo = NULL;
    fila->tamanho--;
    return removido;
}
