
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define LINHAS 6
#define COLUNAS 8
#define TOTAL (LINHAS * COLUNAS)

#define INF 999999

typedef struct {
    int x;
    int y;
} Posicao;

int mapa[LINHAS][COLUNAS] = {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 1, 1, 1, 0, 1, 1, 0},
    {0, 0, 0, 1, 0, 0, 0, 0},
    {1, 1, 0, 1, 1, 1, 0, 1},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 1, 1, 1, 1, 1, 1, 0}
};

int dx[] = {-1, 1, 0, 0};
int dy[] = {0, 0, -1, 1};

int converter(int x, int y) {
    return y * COLUNAS + x;
}

Posicao coordenadas(int no) {
    Posicao p;
    p.x = no % COLUNAS;
    p.y = no / COLUNAS;
    return p;
}

int manhattan(int atual, int destino) {
    Posicao a = coordenadas(atual);
    Posicao b = coordenadas(destino);

    return abs(a.x - b.x) + abs(a.y - b.y);
}

void reconstruirCaminho(int pai[], int inicio, int destino) {
    int caminho[TOTAL];
    int tamanho = 0;
    int atual = destino;

    while (atual != -1) {
        caminho[tamanho++] = atual;

        if (atual == inicio)
            break;

        atual = pai[atual];
    }

    if (caminho[tamanho - 1] != inicio) {
        printf("Caminho invalido.\n");
        return;
    }

    printf("Caminho encontrado:\n");

    for (int i = tamanho - 1; i >= 0; i--) {
        Posicao p = coordenadas(caminho[i]);
        printf("(%d, %d)", p.x, p.y);

        if (i > 0)
            printf(" -> ");
    }

    printf("\n");
}

void mostrarMapa(int inicio, int destino, int pai[]) {
    char visual[LINHAS][COLUNAS];

    for (int i = 0; i < LINHAS; i++) {
        for (int j = 0; j < COLUNAS; j++) {
            visual[i][j] = mapa[i][j] ? '#' : '.';
        }
    }

    int atual = destino;

    while (atual != -1) {
        Posicao p = coordenadas(atual);
        visual[p.y][p.x] = '*';

        if (atual == inicio)
            break;

        atual = pai[atual];
    }

    Posicao a = coordenadas(inicio);
    Posicao b = coordenadas(destino);

    visual[a.y][a.x] = 'S';
    visual[b.y][b.x] = 'D';

    printf("\nMapa:\n");

    for (int i = 0; i < LINHAS; i++) {
        for (int j = 0; j < COLUNAS; j++) {
            printf("%c ", visual[i][j]);
        }
        printf("\n");
    }

    printf("S = inicio | D = destino | # = obstaculo\n");
    printf("* = caminho encontrado | . = livre\n");
}

void aEstrela(int inicio, int destino) {
    int g[TOTAL];
    int h[TOTAL];
    int f[TOTAL];
    int pai[TOTAL];
    int fechado[TOTAL] = {0};

    for (int i = 0; i < TOTAL; i++) {
        g[i] = INF;
        h[i] = manhattan(i, destino);
        f[i] = INF;
        pai[i] = -1;
    }

    g[inicio] = 0;
    f[inicio] = h[inicio];

    while (1) {
        int atual = -1;
        int menorF = INF;

        for (int i = 0; i < TOTAL; i++) {
            if (!fechado[i] && f[i] < menorF) {
                menorF = f[i];
                atual = i;
            }
        }

        if (atual == -1) {
            printf("Nao existe caminho ate o destino.\n");
            return;
        }

        if (atual == destino) {
            reconstruirCaminho(pai, inicio, destino);
            mostrarMapa(inicio, destino, pai);
            printf("Custo total: %d\n", g[destino]);
            return;
        }

        fechado[atual] = 1;

        Posicao p = coordenadas(atual);

        for (int i = 0; i < 4; i++) {
            int nx = p.x + dx[i];
            int ny = p.y + dy[i];

            if (nx < 0 || nx >= COLUNAS ||
                ny < 0 || ny >= LINHAS)
                continue;

            if (mapa[ny][nx] == 1)
                continue;

            int vizinho = converter(nx, ny);

            if (fechado[vizinho])
                continue;

            int novoG = g[atual] + 1;

            if (novoG < g[vizinho]) {
                pai[vizinho] = atual;
                g[vizinho] = novoG;
                f[vizinho] = g[vizinho] + h[vizinho];
            }
        }
    }
}

int main(void) {
    int sx = 0, sy = 0;
    int dxDestino = 7, dyDestino = 5;

    int inicio = converter(sx, sy);
    int destino = converter(dxDestino, dyDestino);

    aEstrela(inicio, destino);

    return 0;
}
