#include <stdio.h>
#include <stdlib.h>

#include "queue.h" // contém funções úteis para filas
#include "proc.h"  // possui as funções dos processos
#include "stats.h" // possui as funções de estatísticas 
#include "utils.h" // possui funções úteis 

// Utilizando as variáveis globais definidas no 'main'
extern struct queue * ready;    // fila de aptos
extern struct queue * ready2;   // segunda fila de aptos
extern struct queue * blocked;  // fila de bloqueados
extern struct queue * finished; // fila de finalizados
// NOTE: essa fila de finalizados é utilizada apenas para as estatísticas finais

// variável global que indica o tempo máximo que um processo pode executar ao todo
extern int MAX_TIME;

struct proc * scheduler(struct proc * current)
{
    struct proc * selected; 
    // O escalonador deverá, a cada rodada, 
    // selecionar o processo que possui maior tempo restante (remaining_time).

    /*
     *   Tratando o processo que está atualmente executando
     */

    // Para tratar o caso do início do sistema, quando não tem ninguém rodando
    if (current != NULL)
    {
        // Verificando o estado em que o processo executando está
        switch (current->state) 
        {
            // Caso esteja 'apto' é porque saiu por preempção
            case READY:
                // Adicionando no final da fila de aptos
                enqueue(ready, current);
                // Realizando as estatísticas para o processo que entra na fila de aptos
                count_ready_in(current);
                break;

            // Caso o processo tenha saído como 'bloqueado', é porque fez uma E/S
            case BLOCKED:
                // Adicionando no final da fila de bloqueados
                enqueue(blocked, current);
                // Realizando as estatísticas para o processo que entra na fila de bloqueados
                count_blocked_in(current);
                break;

            // Caso o processo tenha finalizado, estado 'finished'
            case FINISHED:
                // Adicionando-o na fila de finalizados
                enqueue(finished, current);
                // Realizando as estatísticas para o processo que entra na fila de finalizados
                count_finished_in(current);
                break;

            default:
                printf("@@ ERRO no estado de saída do processo %d\n", current->pid);
        }
    }

    /*
     *   Estratégia de seleção de um novo processo para executar
     */

    // Se a fila de aptos está vazia, não há o que fazer
    if (isempty(ready))
    {
        return NULL;
    }

    // Estratégia para o LJF:
    // Inicializa os ponteiros selected, temp, prev, max_node e max_prev.
    selected = NULL;
    struct proc * temp = ready->head;
    struct proc * prev = NULL;
    struct proc * max_node = NULL;
    struct proc * max_prev = NULL;

    // Encontrar o processo com o maior tempo restante
    while (temp != NULL) { // Percorre todos os nós na fila de prontos.
        if (selected == NULL || temp->remaining_time > selected->remaining_time) { 
            // Se selected for NULL ou o tempo restante de temp for maior que o tempo 
            // restante do processo atualmente selecionado, atualiza selected, max_prev e max_node.
            selected = temp;
            max_prev = prev;
            max_node = temp;
        }
        prev = temp;
        temp = temp->next;
    }

    // Remover o processo selecionado da fila
    if (max_node != NULL) {
        // Se max_node não for NULL, remove o processo da fila 
        // atualizando os ponteiros head e tail conforme necessário.
        if (max_prev == NULL) {
            // O processo selecionado é o primeiro da fila
            ready->head = max_node->next;
        } else {
            max_prev->next = max_node->next;
        }
        if (max_node->next == NULL) {
            // O processo selecionado é o último da fila
            ready->tail = max_prev;
        }
        max_node->next = NULL; // desconectar o nó
    }

    // Realizando as estatísticas para o processo que saiu da fila de aptos
    count_ready_out(selected);

    // Alterando o estado do processo selecionado para executando
    selected->state = RUNNING;
        
    // Retornando o processo selecionado
    return selected;
}
