#include <stdio.h>
#include "queue.h"
#include "proc.h"
#include "stats.h"

// Utilizando as variáveis globais definidas no 'main'
extern struct queue * ready;    // fila de aptos
extern struct queue * ready2;   // segunda fila de aptos
extern int MAX_TIME; 
// Para tratar o que fazer com o processo ao ocorrer a interrupção que define sua saída da fila de bloqueados
void proc_interrupt(struct proc * p)
{
    if (p->remaining_time < 0) {
        p->remaining_time = 0;
        printf("@@ ERRO: Tempo restante do processo %d ajustado para 0 ao sair do bloqueio\n", p->pid);
    }

    // Adicionando o processo na fila de aptos
    if (p->remaining_time <= (MAX_TIME * 0.2)) {
        enqueue(ready, p);
        printf("Processo %d retornado para a fila READY com tempo restante %d\n", p->pid, p->remaining_time);
    } else {
        enqueue(ready2, p);
        printf("Processo %d retornado para a fila READY2 com tempo restante %d\n", p->pid, p->remaining_time);
    }

    // Alterando o status para apto
    p->state = READY;

    // Realizando as estatísticas para o processo que entra na fila de aptos
    count_ready_in(p);
}
