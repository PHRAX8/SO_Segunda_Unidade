#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>

// maximo limite individual de recursos
#define MAXLIMIT 15

// numero de clientes
int n;

// numero de recursos
int m;

// estruturas utilizadas pelo algoritmo

// recursos disponiveis
int *disp;
// maximo de recursos para cada processo
int **max;
// quantidade de recursos alocadas por processo
int **aloc;
// quantidade de recursos que cada processo ainda precisa
int **ne;

// threads dos n clientes
pthread_t *ncli;

sem_t mutex;

// funcoes utilizadas

// verifica se o estado eh seguro
int seguranca();

// verifica se uma requisicao eh valida
int requisicao(int, int *);

// a ser rodada pelas threads dos clientes
void cliente(void *);

// inicializa os valores disponiveis dos recursos
void inicia_recursos();

// inicializa os valores dos clientes:
//  - valores de max[][]
//  - valores de aloc[][]
//  - valores de ne[][]
void inicia_clientes();

// verifica se um cliente ja finalizou, se tem todos os recursos necessario
// se sim, libera seus recursos alocados
int finaliza_cliente(int);

// gera uma requisicao aleatoria
int *gera_requisicao();

// gera um valor aleatorio
// de 1 a lim, se zero = 1
// de 0 a lim, se zero = 0
int gera_rand(int lim, int zero);

// imprime o estado do banco
void print_banco();

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        printf("Uso: %s num_clientes num_recursos\n", argv[0]);
        return 1;
    }

    int i;

    // iniciando a semente do random
    srand(time(NULL));

    // num clients
    n = atoi(argv[1]);

    // num resources
    m = atoi(argv[2]);

    // inicializando o semáforo
    sem_init(&mutex, 0, 1);

    // iniciando os recursos
    inicia_recursos();

    // iniciando os clientes
    inicia_clientes();

    print_banco();

    // liberando a memoria alocada
    for (i = 0; i < n; i++)
    {
        free(max[i]);
        free(aloc[i]);
        free(ne[i]);
    }
    free(disp);
    free(max);
    free(aloc);
    free(ne);
    free(ncli);

    sem_destroy(&mutex);

    return 0;
}

void inicia_recursos()
{
    int i;
    // gerando um vetor de recursos disponiveis
    disp = (int *)malloc(m * sizeof(int));
    if (disp == NULL)
    {
        perror("Erro ao alocar memoria para disp");
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < m; i++)
    {
        disp[i] = gera_rand(MAXLIMIT, 1);
    }
}

void inicia_clientes()
{
    int i, j;

    // criando as matrizes do banqueiro
    max = (int **)malloc(n * sizeof(int *));
    if (max == NULL)
    {
        perror("Erro ao alocar memoria para max");
        exit(EXIT_FAILURE);
    }
    aloc = (int **)malloc(n * sizeof(int *));
    if (aloc == NULL)
    {
        free(max); // Libera a memória alocada para max antes de sair
        perror("Erro ao alocar memoria para aloc");
        exit(EXIT_FAILURE);
    }
    ne = (int **)malloc(n * sizeof(int *));
    if (ne == NULL)
    {
        free(max);  // Libera a memória alocada para max antes de sair
        free(aloc); // Libera a memória alocada para aloc antes de sair
        perror("Erro ao alocar memoria para ne");
        exit(EXIT_FAILURE);
    }

    // gerando uma lista de threads de clientes
    ncli = malloc(n * sizeof(pthread_t));
    if (ncli == NULL)
    {
        perror("Erro ao alocar memoria para ncli");
        free(max);
        free(aloc);
        free(ne);
        exit(EXIT_FAILURE);
    }
    // iniciando os valores maximos de cada cliente
    for (i = 0; i < n; i++)
    {
        max[i] = (int *)malloc(m * sizeof(int));
        aloc[i] = (int *)malloc(m * sizeof(int));
        ne[i] = (int *)malloc(m * sizeof(int));

        if (max[i] == NULL || aloc[i] == NULL || ne[i] == NULL)
        {
            perror("Erro ao alocar memoria para uma das matrizes");

            for (j = 0; j <= i; j++)
            {
                free(max[j]);
                free(aloc[j]);
                free(ne[j]);
            }
            free(max);
            free(aloc);
            free(ne);
            free(ncli);
            exit(EXIT_FAILURE);
        }

        for (j = 0; j < m; j++)
        {
            max[i][j] = gera_rand(disp[j], 1);
            aloc[i][j] = 0;
            ne[i][j] = max[i][j];
        }

        // iniciando as threads dos clientes
        pthread_create(&ncli[i], NULL, (void *)cliente, (void *)(intptr_t)i);
    }

    // esperando as threads
    for (i = 0; i < n; i++)
    {
        pthread_join(ncli[i], NULL);
    }
}

int *gera_requisicao(int cliente)
{
    static int *req;
    req = malloc(m * sizeof(int));

    int i;
    for (i = 0; i < m; i++)
    {
        req[i] = gera_rand(ne[cliente][i], 0);
    }

    return req;
}

int requisicao(int i, int *req)
{
    int j, k;
    sem_wait(&mutex);
    //  verifica se req <= ne
    //  verifica se req <= disp
    for (j = 0; j < m; j++)
    {
        if (req[j] > ne[i][j])
        {
            sem_post(&mutex);
            return 0;
        }
        if (req[j] > disp[j])
        {
            sem_post(&mutex);
            return 0;
        }
    }
    // simula a requisicao
    for (j = 0; j < m; j++)
    {
        disp[j] -= req[j];
        aloc[i][j] += req[j];
        ne[i][j] -= req[j];
    }
    // testa se o banco esta em estado seguro
    if (seguranca())
    {
        // retorna verdadeiro se a requisicao eh valida
        sem_post(&mutex);
        return 1;
    }
    else
    {
        // desfaz simulacao (roll back) em caso de requisicao invalida
        for (j = 0; j < m; j++)
        {
            disp[j] += req[j];
            aloc[i][j] -= req[j];
            ne[i][j] += req[j];
        }
        // requisicao negada
        sem_post(&mutex);
        return 0;
    }
}

int seguranca()
{
    int i, j;

    int ne_menor;

    int *trab = malloc(m * sizeof(int));
    if (trab == NULL)
    {
        perror("Erro ao alocar memoria para trab");
        exit(EXIT_FAILURE);
    }
    int *fim = malloc(n * sizeof(int));
    if (fim == NULL)
    {
        free(trab);
        perror("Erro ao alocar memoria para fim");
        exit(EXIT_FAILURE);
    }

    //  copia disp para trab
    for (j = 0; j < m; j++)
    {
        trab[j] = disp[j];
    }

    // iniciando todos de fim como falso
    for (j = 0; j < n; j++)
    {
        fim[j] = 0;
    }

    // buscando algum cliente que possa acabar
    int seguro = 0;
    while (!seguro)
    {
        seguro = 1;
        for (i = 0; i < n; i++)
        {
            // se o fim eh falso
            if (!fim[i])
            {
                int pode_finalizar = 1;
                for (j = 0; j < m; j++)
                {
                    // verifica se ne[i] e aloc[i] foram alocados corretamente
                    if (ne[i] == NULL || aloc[i] == NULL)
                    {
                        perror("Erro: apontador nulo em ne ou aloc");
                        free(trab);
                        free(fim);
                        exit(EXIT_FAILURE);
                    }
                    // se ne <= trab
                    if (ne[i][j] > trab[j])
                    {
                        pode_finalizar = 0;
                        break;
                    }
                }
                if (pode_finalizar)
                {
                    for (j = 0; j < m; j++)
                    {
                        // se ne <= trab, acrescenta seu aloc em trab
                        trab[j] += aloc[i][j];
                    }
                    // define que esse cliente pode terminar
                    fim[i] = 1;
                    seguro = 0;
                }
            }
        }
    }
    // verifica se todos os clientes terminaram
    int todos_finalizados = 1;
    for (j = 0; j < n; j++)
    {
        // se algum cliente nao terminou
        if (!fim[j])
        {
            // retorna estado inseguro
            todos_finalizados = 0;
            break;
        }
    }
    // se todos terminam, retorna estado seguro
    free(trab);
    free(fim);
    return todos_finalizados;
}

int finaliza_cliente(int i)
{
    int j;
    sem_wait(&mutex);
    // verifica se cliente nao tem mais necessidades
    for (j = 0; j < m; j++)
    {
        // cliente ainda nao terminou
        if (ne[i][j] != 0)
        {
            sem_post(&mutex);
            return 0;
        }
    }

    // cliente terminou

    // limpa seus recursos
    for (j = 0; j < m; j++)
    {
        // devolve para os disponiveis
        disp[j] += aloc[i][j];
        aloc[i][j] = 0;
    }
    sem_post(&mutex);
    return 1;
}

void cliente(void *param_i)
{
    int i = (intptr_t)param_i;

    int executa = 1, j;

    int *req;
    // fica em looping, gerando requisições, até conseguir todos os recursos
    // que precisa, quando conseguir todos os recursos, irá finalizar
    while (executa)
    {

        printf(": Cliente %d gerou a requisição: ", i);

        req = gera_requisicao(i);

        for (j = 0; j < m; j++)
        {
            printf(" %2d ", req[j]);
        }

        printf("\n");

        printf(": Cliente %d ganhou acesso\n", i);

        // enviando a requisição ao banco
        if (requisicao(i, req) == 1)
        {
            printf(": Cliente %d executou sua requisicao\n", i);

            // verifica se o cliente terminou
            if (finaliza_cliente(i) == 1)
            {

                printf(": Cliente %d finalizou\n", i);

                // cliente ja terminou, seus recursos foram liberados
                executa = 0;
            }
        }
        else
        {
            printf(": Cliente %d teve sua requisicao negada\n", i);
        }

        print_banco();
        free(req);
    }
}

int gera_rand(int limit, int zero)
{
    if (zero == 1)
    {
        if (limit == 0)
        {
            return 1;
        }
        // 1 a limit
        return (rand() % limit) + 1;
    }

    // 0 a limit
    return rand() % (limit + 1);
}

void print_banco()
{
    int i, j;

    printf("\t== BANCO ==\n");

    printf("Recursos disponiveis\n");
    printf("\t    i: ");
    for (j = 0; j < m; j++)
    {
        printf(" %2d ", j);
    }

    printf("\n\t       ");
    for (j = 0; j < m; j++)
    {
        printf(" %2d ", disp[j]);
    }

    printf("\n");

    printf("MAX dos clientes\n");
    for (i = 0; i < n; i++)
    {
        printf("\ti: %d [ ", i);

        for (j = 0; j < m; j++)
        {
            printf(" %2d ", max[i][j]);
        }

        printf(" ]\n");
    }

    printf("Aloc dos clientes\n");
    for (i = 0; i < n; i++)
    {
        printf("\ti: %d [ ", i);

        for (j = 0; j < m; j++)
        {
            printf(" %2d ", aloc[i][j]);
        }

        printf(" ]\n");
    }

    printf("Ne dos clientes\n");
    for (i = 0; i < n; i++)
    {
        printf("\ti: %d [ ", i);

        for (j = 0; j < m; j++)
        {
            printf(" %2d ", ne[i][j]);
        }

        printf(" ]\n");
    }

    printf("========================\n");
    printf("\n");
}