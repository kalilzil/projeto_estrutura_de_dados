#ifndef FILA_H
#define FILA_H

// --- tipos ---

// no da arvore de huffman
typedef struct no {
    void *item;
    int frequencia;
    struct no *proximo;
    struct no *esquerda;
    struct no *direita;
} No;

// fila de prioridade: lista encadeada sempre ordenada, com a menor frequencia no inicio
typedef struct fila {
    No *inicio;
    int tamanho;
    int (*comparar)(void *a, void *b);
} Fila;

// cria um no novo; pra criar uma folha, passa NULL na esquerda e na direita
No *criar_no(unsigned char byte, int frequencia, No *esquerda, No *direita);

// cria a fila vazia
Fila *criar_fila(int (*comparar)(void *a, void *b));

// compara dois nos pela frequencia
int comparar_frequencia(void *a, void *b);

// insere na fila em ordem crescente
void inserir_ordenado(Fila *fila, No *novo);

// tira o primeiro da fila (menor frequencia) e retorna ele
No *remover_inicio(Fila *fila);

#endif
