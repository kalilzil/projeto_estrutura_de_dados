#ifndef FILA_H
#define FILA_H

// --- tipos ---

// no da arvore de huffman
// o mesmo no tambem serve de elemento da fila, ligado pelo campo proximo
// item aponta pro byte da folha (nos nos internos guarda '*')
typedef struct no {
    void *item;
    int frequencia;
    struct no *proximo;
    struct no *esquerda;
    struct no *direita;
} No;

// fila de prioridade: lista encadeada sempre ordenada, com a menor frequencia no inicio
// comparar retorna negativo se a < b, 0 se forem iguais e positivo se a > b
typedef struct fila {
    No *inicio;
    int tamanho;
    int (*comparar)(void *a, void *b);
} Fila;

// --- no ---

// cria um no novo; pra criar uma folha, passa NULL na esquerda e na direita
No *criar_no(unsigned char byte, int frequencia, No *esquerda, No *direita);

// --- fila ---

// cria a fila vazia
Fila *criar_fila(int (*comparar)(void *a, void *b));

// compara dois nos pela frequencia (funcao passada pro criar_fila)
int comparar_frequencia(void *a, void *b);

// insere na fila em ordem crescente
// se empatar, o novo fica na frente dos que tem a mesma frequencia
void inserir_ordenado(Fila *fila, No *novo);

// tira o primeiro da fila (menor frequencia) e retorna ele
No *remover_inicio(Fila *fila);

#endif
