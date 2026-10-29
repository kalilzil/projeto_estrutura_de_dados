#include <stdio.h>
#include <stdlib.h>

#define V 4
#define E 5

typedef struct {
    int origem;
    int destino;
    int peso;
} Aresta;

int comparar(const void *a, const void *b) {
    Aresta *arestaA = (Aresta *)a;
    Aresta *arestaB = (Aresta *)b;

    return arestaA->peso - arestaB->peso;
}

int encontrar(int pai[], int x) {
    if (pai[x] == x)
        return x;

    return encontrar(pai, pai[x]);
}

void unir(int pai[], int a, int b) {
    int raizA = encontrar(pai, a);
    int raizB = encontrar(pai, b);

    if (raizA != raizB)
        pai[raizA] = raizB;
}

int main(void) {
    Aresta arestas[E] = {
        {0, 1, 1},  // A - B
        {1, 2, 2},  // B - C
        {0, 2, 3},  // A - C
        {2, 3, 4},  // C - D
        {1, 3, 5}   // B - D
    };

    int pai[V];
    int total = 0;
    int selecionadas = 0;

    for (int i = 0; i < V; i++)
        pai[i] = i;

    qsort(arestas, E, sizeof(Aresta), comparar);

    printf("Arestas da arvore geradora minima:\n");

    for (int i = 0; i < E && selecionadas < V - 1; i++) {
        int u = arestas[i].origem;
        int v = arestas[i].destino;

        if (encontrar(pai, u) != encontrar(pai, v)) {
            printf("%c - %c (peso %d)\n", 'A' + u, 'A' + v, arestas[i].peso);

            unir(pai, u, v);
            total += arestas[i].peso;
            selecionadas++;
        }
    }

    if (selecionadas != V - 1) {
        printf("O grafo nao e conexo.\n");
        return 1;
    }

    printf("Custo total: %d\n", total);

    return 0;
}
