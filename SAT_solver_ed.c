#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int *literais;
    int tamanho;
} Clausula;

typedef struct {
    int num_variaveis;
    int num_clausulas;
    Clausula *clausulas;
} Formula;

static int *atribuicao;
static int *historico;
static int head_historico = 0;

// valor de um literal: 1 = verdadeiro; -1 = falso; 0 = livre
static int valor_literal (int literal){
    int v = atribuicao[abs(literal)];
    if (v == 0) return 0;
    return (literal > 0) ? v : -v;
}

//funcao faz o literal ficar verdadeiro e guarda no historico
static void tornar_lit_verdadeiro (int literal){
    atribuicao[abs(literal)] = (literal > 0) ? 1 : -1;
    historico[head_historico++] = abs(literal);
}

static void desfazer_ate(int marca) {
    while (head_historico > marca) {
        atribuicao[historico[--head_historico]] = 0;
    }
}

//leitura do arquivo DIMACS
static int ler_dimacs(const char *caminho, Formula *f){
    FILE *arquivo = fopen(caminho, "r");
    if (!arquivo){
    perror("Erro ao abrir o arquivo");
    return 0;
    }

    char linha [4096];
    int capacidade = 0, contagem = 0;
    f->clausulas = NULL;
    f->num_variaveis = f->num_clausulas = 0;

    //buffer temp. que guarda a clausula lida atual
    int *buffer = NULL, tam_buffer = 0, cap_buffer = 0;

    while (fgets(linha, sizeof(linha), arquivo)){
        if (linha[0] == 'c' || linha[0] == '\n' || linha[0] =='\r') continue;
        if (linha [0] == '%') break;

        // cabecalho e leitura do numero de variaveis e clausulas
        if (linha[0] == 'p'){
            if (sscanf(linha, "p cnf %d %d", &f->num_variaveis, &f->num_clausulas) != 2){
                fprintf(stderr, "Cabecalho invalido\n");
                fclose(arquivo);
                return 0;
            }
            capacidade = f->num_clausulas > 0 ? f->num_clausulas : 16;
            f->clausulas = malloc(capacidade * sizeof(Clausula));
            continue;
        }

        // le um numero por vez, o 0 marca o fim da clausula
        char *p = linha;
        char *fim;
        long numero;
        while ((numero = strtol(p, &fim, 10)), fim != p){
            p = fim;
            if (numero == 0){
                    //copia o buffer para uma nova clausula quando termina
                if (contagem == capacidade){
                    capacidade *= 2;
                    f->clausulas = realloc(f->clausulas, capacidade * sizeof(Clausula));
                    }

                    f->clausulas[contagem].literais = malloc((tam_buffer > 0 ? tam_buffer : 1) * sizeof(int));
                    memcpy(f->clausulas[contagem].literais, buffer, tam_buffer * sizeof(int));
                    f->clausulas[contagem].tamanho = tam_buffer;
                    contagem ++;
                    tam_buffer = 0;
                }
                else{ //guarda no buffer se ainda faz parte da clausula
                    if (tam_buffer == cap_buffer){
                        cap_buffer = cap_buffer ? cap_buffer * 2 : 8;
                    buffer = realloc(buffer, cap_buffer * sizeof(int));
                    } 
                    buffer [tam_buffer++] = (int)numero;
                }
        }
    }
    free(buffer);
    fclose(arquivo);
    f->num_clausulas = contagem;
    return 1;
}

//DPLL 

static int propagar(Formula *f){
    int mudou = 1;
    while(mudou){
        mudou = 0;
        for (int i = 0; i < f->num_clausulas; i++)
        {
            Clausula *c = &f->clausulas[i];
            int satisfeita = 0, qtd_livres = 0, ultimo_livre = 0;

            for (int j = 0; j < c->tamanho; j++){
                int v = valor_literal(c->literais[j]);
                if (v == 1) {satisfeita = 1; break;}
                if (v == 0) {qtd_livres++; ultimo_livre = c->literais[j];}
            }
            
            if(satisfeita) continue; //clausula ja resolvida
            if (qtd_livres == 0) return 0; // Conflito
            if (qtd_livres == 1) { //clausula unitaria
                tornar_lit_verdadeiro(ultimo_livre);
                mudou = 1;
            }
        }  
    }
    return 1;
}

static int dpll (Formula *f){
    int marca = head_historico;
    
    if (!propagar(f)){
        desfazer_ate(marca);
        return 0;
    }

    int variavel = 0;
    for(int v = 1; v <= f->num_variaveis; v++){
        if (atribuicao[v] == 0) {variavel = v; break;}
    }
    if (variavel == 0) return 1; // tudo atribuido sem conflito -> SAT

    for (int try = 0; try < 2; try++){
        int marca2 = head_historico;
        tornar_lit_verdadeiro(try == 0 ? variavel : -variavel);
        if (dpll (f)) return 1;
        desfazer_ate(marca2);
    }

    desfazer_ate(marca);
    return 0;
}

int main (int argc, char **argv){
    if (argc < 2){
        fprintf(stderr, "Uso: %s arquivo.cnf\n", argv[0]);
        return 1;
    }

    Formula f;
    if (!ler_dimacs(argv[1], &f)) return 1;

    atribuicao = calloc(f.num_variaveis + 1, sizeof(int));
    historico = malloc((f.num_variaveis + 1) * sizeof(int));

    if (dpll(&f)){
        printf("s SATISFIABLE\nv");
        for (int v = 1; v <= f.num_variaveis; v++){
            printf(" %d", atribuicao[v] == 1 ? v : -v);
        }
        printf(" 0\n");
    }
    else{
        printf("s UNSATISFIABLE\n");
    }

    for (int i = 0; i < f.num_clausulas; i++) free(f.clausulas[i].literais);
    free(f.clausulas);
    free(atribuicao);
    free(historico);
    return 0;
}

