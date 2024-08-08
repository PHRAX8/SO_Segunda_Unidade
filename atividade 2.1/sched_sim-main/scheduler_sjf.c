#include <stdio.h>
#include <stdlib.h>
#include "queue.h" // contem funções uteis para filas
#include "proc.h"  // possui as funções dos processos
#include "stats.h" // possui as funções de estatisticas
#include "utils.h" // possui funções uteis

// Utilizando as variáveis globais definidas no 'main'
extern struct queue *ready;    // fila de aptos
extern struct queue *ready2;   // segunda fila de aptos
extern struct queue *blocked;  // fila de bloqueados
extern struct queue *finished; // fila de finalizados
// NOTE: essa fila de finalizados é utilizada apenas para
// as estatisticas finais

// variavel global que indica o tempo maximo que um processo pode executar ao todo
extern int MAX_TIME;

struct proc *scheduler(struct proc *current)
{
    struct proc *selected;
    // O escalonador deverá, a cada rodada,
    // selecionar o processo que possui menor tempo restante (remaining_time).
    // Para tratar o caso do início do sistema, quando nao tem ninguem rodando
    if (current != NULL)
    {
        // Verificando o estado em que o processo executando está
        switch (current->state)
        {
        // Caso esteja 'apto' é porque saiu por preempção
        case READY:

            // Adicionando no final da fila de aptos
            enqueue(ready, current);

            // Realizando as estatisticas para o processo que
            // entra na fila de aptos
            count_ready_in(current);

            break;

        // Caso o processo tenha saído como 'bloqueado', é porque fez uma E/S
        case BLOCKED:

            // Adicionando no final da fila de bloqueados
            enqueue(blocked, current);

            // Realizando as estatisticas para o processo que
            // entra na fila de bloqueados
            count_blocked_in(current);

            break;

        // Caso o processo tenha finalizado, estado 'finished'
        case FINISHED:

            // Adicionando-o na fila de finalizados
            enqueue(finished, current);

            // Realizando as estatisticas para o processo que
            // entra na fila de finalizados
            count_finished_in(current);

            break;

        default:
            printf("@@ ERRO no estado de saída do processo %d\n", current->pid);
        }
    }

    /*
     *   Estratégia de seleção de um novo processo para executar
     */

    // Se a fila de aptos está vazia, nao há o que fazer
    if (isempty(ready))
    {
        // printf("== sched FIFO: fila READY is empty\n");
        return NULL;
    }

    // Estratégia para o SJF:
    selected = NULL;                 // Inicializa o ponteiro para o processo selecionado.
    struct proc *temp = ready->head; // Inicializa o ponteiro temporário temp
                                     // com o primeiro processo da fila de prontos.
    struct proc *prev = NULL;        // Inicializa o ponteiro prev como NULL para rastrear
                                     // o nó anterior durante a iteração.
    struct proc *min_node = NULL;    // Inicializa o ponteiro para o nó com o menor tempo restante.
    struct proc *min_prev = NULL;    // Inicializa o ponteiro para o nó anterior ao nó com o menor tempo restante.

    // Encontrar o processo com o menor tempo restante
    while (temp != NULL)
    { // Percorre todos os nós na fila de prontos.
        if (selected == NULL || temp->remaining_time < selected->remaining_time)
        {
            // Se selected for NULL (primeiro nó) ou o tempo restante de temp for menor que o tempo restante
            // do processo atualmente selecionado, atualiza selected, min_prev e min_node.
            selected = temp;
            min_prev = prev;
            min_node = temp;
        }
        prev = temp;
        temp = temp->next;
    }

    // Remover o processo selecionado da fila
    if (min_node != NULL)
    { // Se encontrou um processo com menor tempo restante:
        if (min_prev == NULL)
        { // Se o processo selecionado é o primeiro da fila:
            // O processo selecionado é o primeiro da fila
            ready->head = min_node->next; // Atualiza o cabeçalho da fila.
        }
        else
        {
            min_prev->next = min_node->next; // Remove min_node da fila ajustando o ponteiro next do nó anterior.
        }
        if (min_node->next == NULL)
        { // e o processo selecionado é o último da fila:
            // O processo selecionado é o último da fila
            ready->tail = min_prev; // Atualiza o final da fila.
        }
        min_node->next = NULL; // desconectar o nó
    }

    // Realizando as estatisticas para o processo que
    // saiu da fila de aptos
    count_ready_out(selected);

    // Alterando o estado do processo selecionado para executando
    selected->state = RUNNING;

    // Retornando o processo selecionado
    return selected;
}
