#include <stdio.h>
#include <stdlib.h>
#include "arvore.h"

// --- auxiliares ---

unsigned char pegar_byte(No *no)
{
    return *(unsigned char *)no->item;
}

int eh_folha(No *no)
{
    return no->esquerda == NULL && no->direita == NULL;
}

// --- montagem (compactar) ---

No *montar_arvore(Fila *fila)
{
    // se o arquivo so tem um tipo de byte (tipo "AAAA"), a arvore seria so uma folha
    // e o byte ficaria sem codigo. entao cria uma copia da folha pra raiz ter
    // dois filhos. o byte acaba com o codigo "1", porque a copia da direita
    // sobrescreve o codigo da esquerda (as duas folhas sao o mesmo byte)
    if (fila->tamanho == 1) {
        No *unico = remover_inicio(fila);
        No *copia = criar_no(pegar_byte(unico), 0, NULL, NULL);
        return criar_no('*', unico->frequencia, unico, copia);
    }

    while (fila->tamanho > 1) {
        No *esquerda = remover_inicio(fila);
        No *direita = remover_inicio(fila);
        No *pai = criar_no('*', esquerda->frequencia + direita->frequencia, esquerda, direita);
        inserir_ordenado(fila, pai);
    }
    return remover_inicio(fila);
}

// --- escrita no arquivo (compactar) ---

int tamanho_arvore(No *raiz)
{
    if (eh_folha(raiz)) {
        if (pegar_byte(raiz) == '*' || pegar_byte(raiz) == '\\') {
            return 2; // o byte + o '\' de escape
        }
        return 1;
    }
    return 1 + tamanho_arvore(raiz->esquerda) + tamanho_arvore(raiz->direita);
}

void escrever_arvore(No *raiz, FILE *arquivo)
{
    if (eh_folha(raiz)) {
        if (pegar_byte(raiz) == '*' || pegar_byte(raiz) == '\\') {
            fputc('\\', arquivo);
        }
        fputc(pegar_byte(raiz), arquivo);
    } else {
        fputc('*', arquivo);
        escrever_arvore(raiz->esquerda, arquivo);
        escrever_arvore(raiz->direita, arquivo);
    }
}

// --- leitura do arquivo (descompactar) ---


No *ler_arvore(FILE *arquivo)
{
    unsigned char byte = fgetc(arquivo);

    if (byte == '*') {
        // no interno: primeiro vem a subarvore da esquerda, depois a da direita
        No *esquerda = ler_arvore(arquivo);
        No *direita = ler_arvore(arquivo);
        return criar_no('*', 0, esquerda, direita);
    }

    if (byte == '\\') {
        // '\' indica escape: o byte seguinte vira folha, mesmo sendo '*' ou '\'
        byte = fgetc(arquivo);
    }
    return criar_no(byte, 0, NULL, NULL);
}

// --- memoria ---

void liberar_arvore(No *raiz)
{
    if (raiz == NULL) {
        return;
    }
    liberar_arvore(raiz->esquerda);
    liberar_arvore(raiz->direita);
    free(raiz->item);
    free(raiz);
}
