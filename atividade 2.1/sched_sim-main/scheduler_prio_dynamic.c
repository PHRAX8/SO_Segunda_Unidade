#include <stdio.h>
#include <stdlib.h> // Necessário para rand()
#include "queue.h" // Contém funções úteis para filas
#include "proc.h"  // Possui as funções dos processos
#include "stats.h" // Possui as funções de estatísticas
#include "utils.h" // Possui funções úteis

// Utilizando as variáveis globais definidas no 'main'
extern struct queue * ready;    // fila de aptos (prioridade alta)
extern struct queue * ready2;   // segunda fila de aptos (prioridade baixa)
extern struct queue * blocked;  // fila de bloqueados
extern struct queue * finished; // fila de finalizados
extern int MAX_TIME;            // Tempo máximo total de execução

// Função para selecionar um processo com base nas probabilidades
struct proc * scheduler(struct proc * current)
{
    struct proc * selected;
    int random;

    // Verifica o estado do processo atual (se não for nulo)
    if (current != NULL)
    {
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

    // Se a fila de aptos está vazia, não há o que fazer
    if (isempty(ready) && isempty(ready2))
    {
        return NULL;
    }

    // Decide qual fila usar baseado em probabilidades
    random = rand() % 100;
    if (random < 80 && !isempty(ready))  // 80% de chance de escolher a fila 'ready'
    {
        selected = dequeue(ready);
    }
    else if (!isempty(ready2))  // Se a fila 'ready2' não está vazia
    {
        selected = dequeue(ready2);
    }
    else if (!isempty(ready))  // Se a fila 'ready2' está vazia, mas a 'ready' não está
    {
        selected = dequeue(ready);
    }
    else
    {
        return NULL;
    }

    count_ready_out(selected);
    selected->state = RUNNING;

    return selected;
}
