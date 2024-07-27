#include <stdio.h>
#include <stdlib.h> // Necessário para rand()
#include "queue.h"
#include "proc.h"
#include "stats.h"
#include "utils.h"

// Utilizando as variáveis globais definidas no 'main'
extern struct queue * ready;    // fila de aptos
extern struct queue * ready2;   // fila de aptos
extern struct queue * blocked;  // fila de bloqueados
extern struct queue * finished; // fila de finalizados
extern int MAX_TIME;            // Tempo máximo de execução

struct proc * scheduler(struct proc * current)
{
    struct proc * selected;

    // Tratamento do processo atual
    if (current != NULL)
    {
        if (current->remaining_time < 0) {
            current->remaining_time = 0;
            printf("@@ ERRO: Tempo restante do processo %d ajustado para 0 na interrupção\n", current->pid);
        }

        switch (current->state) 
        {
            case READY:
                enqueue(ready, current);
                count_ready_in(current);
                break;

            case BLOCKED:
                enqueue(blocked, current);
                count_blocked_in(current);
                break;

            case FINISHED:
                enqueue(finished, current);
                count_finished_in(current);
                break;

            default:
                printf("@@ ERRO no estado de saída do processo %d\n", current->pid);
        }
    }

    // Seleção de um novo processo
    if (isempty(ready) && isempty(ready2))
    {
        return NULL;
    }

    // Probabilidade para seleção entre as filas
    int prob = rand() % 100;
    if (prob < 80 && !isempty(ready)) {
        selected = dequeue(ready);
    } else if (!isempty(ready2)) {
        selected = dequeue(ready2);
    } else if (!isempty(ready)) {
        selected = dequeue(ready);
    } else {
        return NULL;
    }

    if (selected->remaining_time < 0) {
        selected->remaining_time = 0;
        printf("@@ ERRO: Tempo restante do processo %d ajustado para 0 ao ser selecionado\n", selected->pid);
    }

    count_ready_out(selected);
    selected->state = RUNNING;

    return selected;
}
