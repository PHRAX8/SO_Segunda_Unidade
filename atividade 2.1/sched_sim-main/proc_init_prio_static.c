#include <stdio.h>
#include <stdlib.h> // Necessário para rand()
#include "queue.h"
#include "proc.h"
#include "stats.h"

// Quantidade de processos a serem criados
extern int NPROC;

// Utilizando as variáveis globais das filas definidas no 'main'
extern struct queue * ready;    // fila de aptos
extern struct queue * ready2;   // segunda fila de aptos
extern struct queue * blocked;  // fila de bloqueados
extern struct queue * finished; // fila de finalizados
extern int MAX_TIME;            // Tempo máximo de execução

// Inicializa os 'NPROC' processos informados
void proc_init()
{
    struct proc *p;

    if (MAX_TIME <= 0) {
        printf("@@ ERRO: MAX_TIME deve ser maior que zero. Valor atual: %d\n", MAX_TIME);
        return;
    }

    for (int i = 0; i < NPROC; i++) 
    {
        p = createproc();
        p->tid = createproc_thread(p);

        // Definir o tempo restante aleatório dentro de um intervalo válido
        p->remaining_time = rand() % (MAX_TIME + 1); // Inicializa o tempo restante com um valor aleatório entre 0 e MAX_TIME
        
        if (p->remaining_time < 0) {
            p->remaining_time = 0;
            printf("@@ ERRO: Tempo restante inicial do processo %d ajustado para 0\n", p->pid);
        }

        // Classificar o processo nas filas com base no tempo restante
        if (p->remaining_time <= (MAX_TIME * 0.2)) {
            enqueue(ready, p);
            printf("Processo %d colocado na fila READY com tempo restante %d\n", p->pid, p->remaining_time);
        } else {
            enqueue(ready2, p);
            printf("Processo %d colocado na fila READY2 com tempo restante %d\n", p->pid, p->remaining_time);
        }

        count_ready_in(p);
    }
}
