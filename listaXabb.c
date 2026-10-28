    #include <stdio.h>
    #include <stdlib.h>
    #include <time.h>

    typedef struct node node; // Definimos o nó da lista

    struct node {
        int item;
        node *next;
    };

    typedef struct binary_tree binary_tree; // Definimos o nó da árvore

    struct binary_tree {
        int item;
        binary_tree *left;
        binary_tree *right;
    };

    void print_linked_list(node *head) // Printar a lista
    {
        while (head != NULL)
        {
            printf("%d\n", head->item);
            head = head->next;
        }
    }

    int search_list(node *head, int item) // Retorna a quantidade de comparações até achar na lista
    {
        int comparisons = 0;

        while (head != NULL)
        {
            comparisons++;

            if (head->item == item)
            {
                return comparisons;
            }
            
            head = head->next;
        }
        
        return -1; // Não achou
    }

    node* add_list(node *head, int item) // Adiciona um novo nó à cabeça da lista
    {
        node *new_node = (node*) malloc(sizeof(node));
        new_node->item = item;
        new_node->next = head;
        return new_node;
    }

    int is_empty(node *head) // Verifica se a lista está vazia
    {
        return (head == NULL);
    }

    node* create_linked_list() // Cria uma lista vazia
    {
        return NULL;
    }

    int search_tree(binary_tree *bt, int item) // Comparações na árvore binária
    {
        int comparisons = 0;

       while (bt != NULL)
        {
            comparisons++;

            if (bt->item == item)
            {
                return comparisons;
            }

            if (bt->item > item)
            {
                bt = bt->left;
            }
            else
            {
                bt = bt->right;
            }
        }

        return -1; // Não achou
    }

    binary_tree* create_binary_tree(int item, binary_tree *left, binary_tree *right) // Cria uma (sub)árvore a partir de uma folha ou árvore vazia
    {
        binary_tree *new_tree = (binary_tree*) malloc(sizeof(binary_tree));
        new_tree->item = item;
        new_tree->left = left;
        new_tree->right = right;
        return new_tree; 
    }

    binary_tree* add_tree(binary_tree *bt, int item) // Insere um nó de árvore em uma posição vazia
    {
        if (bt == NULL) // Caso base (Chegamos em uma folha?)
        {
            bt = create_binary_tree(item, NULL, NULL); // Se sim, cria uma subárvore a partir dela
        }
        else if (bt->item > item) // Caso não, o número a ser inserido é menor que o nó onde estamos?
        {
            bt->left = add_tree(bt->left, item); // Chama a função recursivamente passando o nó à esquerda como parâmetro
        }
        else
        {
            bt->right = add_tree(bt->right, item); // Chama recursivamente passando o nó à direita
        }
        
        return bt; // Retorna o endereço que aponta para a raiz da subárvore
    }

    binary_tree* create_empty_binary_tree() // Cria uma árvore vazia (sem nós)
    {
        return NULL;
    }

void free_list(node *head)
{
    while (head != NULL)
    {
        node *next = head->next;   // guarda o próximo ANTES de liberar
        free(head);
        head = next;
    }
}

void free_tree(binary_tree *bt)
{
    if (bt != NULL)
    {
        free_tree(bt->left);       // filhos primeiro
        free_tree(bt->right);
        free(bt);                  // pai por último (post-order)
    }
}

    int main()
    {
        int N_MAX = 20000;   // tamanho final
        int PASSO = 500;     // a cada quantos dados medimos
        int S = 500;         // sorteios por medição

    node *list = create_linked_list();
    binary_tree *tree = create_empty_binary_tree();
    int *values = malloc(N_MAX * sizeof(int));

    srand(time(NULL));

    FILE *f = fopen("comparacoes_n.csv", "w");
    if (f == NULL)
    {
        return 1;
    }
    fprintf(f, "N,lista,abb\n");

    for (int n = o; n < N_MAX; n++)
    {
        // insere mais um dado nas duas estruturas
        values[n] = rand();
        list = add_list(list, values[n]);
        tree = add_tree(tree, values[n]);

        // a cada PASSO dados, mede
        if (n % PASSO == 0)
        {
            long soma_lista = 0, soma_abb = 0;

            for (int i = 0; i < S; i++)
            {
                int v = values[rand() % n];   // só entre os n já inseridos
                soma_lista += search_list(list, v);
                soma_abb   += search_tree(tree, v);
            }

            fprintf(f, "%d,%.2f,%.2f\n", n,
                    (double) soma_lista / S, (double) soma_abb / S);
        }
    }

    fclose(f);
    free_list(list);
    free_tree(tree);
    free(values);
    return 0;
}
    