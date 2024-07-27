1. Esta atividade consiste em duas etapas:
(a) Implementação de 4 algoritmos de Escalonamento de Processos; e
(b) Análise de performance comparativa entre os algoritmos.
2. A implementação deverá ser baseada no Simulador de Escalonador disponı́vel no repositório do
endereço:
https://github.com/labepi/sched_sim
Este simulador já possui a implementação de um algoritmo de Escalonamento FIFO Preemptivo, que
deverá ser utilizado como base para a implementação dos demais algoritmos, e que está disponı́vel
no arquivo:
scheduler_fifo.c
3. A tarefa consistirá em, com base na implementação citada, implementar os seguintes algoritmos de
escalonamento:
(a) SJF (Shortest Job First)
• O escalonador deverá, a cada rodada, selecionar o processo que possui menor tempo res-
tante (remaining_time).
(b) LJF (Longest Job First)
• Este é um escalonador apenas de testes, pois pode produzir resultados não ideais.
• Ele é o inverso do SJF.
• O escalonador deverá, a cada rodada, selecionar o processo que possui maior tempo res-
tante (remaining_time).
(c) PRIO STATIC: Com prioridades estáticas
• Possui duas filas de prioridades
• Não há realimentação entre as filas.
• Os processos devem ser inseridos em sua fila no inı́cio da simulação
– Para isto, deve-se modificar o arquivo proc_init_prio_static.c.
– Processos com menor tempo restante deverão ir para a primeira fila (ready).
– Processos com maior tempo restante deverão ir para a segunda fila (ready2).
1– O limite para definir a fila do processo é de acordo com o tempo máximo de execução
total de um processo, disponı́vel na variável global MAX_TIME, definida no arquivo
main.c.
– Processos com remaining_time até 20% de MAX_TIME irão para a primeira fila, os
demais irão para a segunda fila.
• Sempre que um processo sair do processador ou da fila de bloqueados, devem voltar para
a mesma fila de inı́cio.
• O tratamento dos processos ao voltarem da fila de bloqueados deve ser feito como modi-
ficação no arquivo proc_interrupt_prio_static.c.
• O escalonador utiliza a seguinte regra para selecionar um processo, de acordo com as
probabilidades entre as filas:
– Fila 1 (ready) - FIFO = 80% de probabilidade
– Fila 2 (ready2) - FIFO = 20% de probabildiade
(d) PRIO DYNAMIC: Com prioridades dinâmicas
• Possui duas filas de prioridades
• Há realimentação entre as filas.
• Os processos, ao serem iniciados, são inseridos sempre na mesma fila (ready), mas poderão
mudar de fila de acordo com a sua execução.
• As regras para mudança de fila são as seguintes:
– Fila 1 (ready) - FIFO = Processos que saı́ram por E/S, após voltar de bloqueado,
deverão voltar para a primeira fila.
– Fila 2 (ready2) - FIFO = Processos que saı́ram por preempção, devem retornar para
a segunda fila.
• O escalonador utiliza a seguinte regra para selecionar um processo, de acordo com as
probabilidades entre as filas:
– Fila 1 (ready) - FIFO = 80% de probabilidade
– Fila 2 (ready2) - FIFO = 20% de probabildiade
4. Para a implementação dos algoritmos, os seguintes arquivos deverão ser modificados:
• scheduler_sjf.c - implementação do SJF
• scheduler_ljf.c - implementação do LJF
• scheduler_prio_static.c - implementação do PRIO STATIC
• scheduler_prio_dynamic.c - implementação do PRIO DYNAMIC
• proc_init.c - deverá ser alterado para a inserção dos processos em diferentes filas durante a
sua inicialização
São permitidas modificações no código original, a fim de realizar as implementações exigidas. Mas,
o professor deverá ser informado das modificações nos demais arquivos além dos listados acima.
5. Considerando que haverão implementações de 5 algoritmos disponı́veis ao final da etapa de imple-
mentação, sendo 4 algoritmos produzidos e 1 algoritmo já implementado com o exemplo, a etapa
de análise de performance consistirá em:
(a) Executar o código de cada algoritmo variando-se linearmente a quantidade de processos:
• Para cada algoritmo realizar no mı́nimo 10 rodadas para cada quantidade de processos;
• Sendo a quantidade de processos a seguinte:
10 20 30 40 50 60 70 80 90 100
• Isto é, para cada algoritmo serão realizadas, no mı́nimo, 100 rodadas. Sendo um total de
500 simulações ao todo, para os 5 algoritmos.
6. Após a execução de cada rodada de simulação, deverá ser armazenado o valor do “Tempo Médio
de Espera” (TME) para cada uma das rodadas, a fim de posterior análise estatı́stica.
27. De posse das amostras do TME para cada simulação, deverão ser obtidas as seguintes medidas
estatı́sticas:
(a) Valor médio (µ) das amostras do TME, para cada par “algoritmo x quantidade de processos”,
que pode ser obtida pela seguinte equação:
n
µ=
1X
xi
n i=1
(1)
Onde n é a quantidade de amostras e xi é a iésima amostra obtida;
(b) Variância (σ 2 ) entre as amostras do TME para cada um destes pares, obtida pela seguinte
equação:
n
1X
σ2 =
(µ − xi )2
(2)
n i=1
8. De posse de toda a análise estatı́stica, organizar os valores sob a forma de um gráfico:
(a) Grafico - Tempo médio de espera (total)
• Eixo X: a quantidade de processos
• Eixo Y : o valor da média do tempo de espera (total)
Os valores de variância serão utilizados para ilustrar os erros de cada uma das métricas simuladas,
e deverão ser plotadas nos gráficos acima como barras de erro (errorbars).
9. Por fim, todas estas informações deverão ser organizadas sob a forma de relatório, onde deverão
ser discutidas as implementações dos algoritmos, bem como uma análise dos gráficos apresentados
(por exemplo, qual algoritmo possui menor TME, por qual motivo isto acontece, o que acontece à
medida que o número de processos aumenta, ...).
O modelo de relatório seguirá o Modelo para publicação de artigos da Sociedade Brasileira de
Computação (SBC):
http://bit.ly/SBCArtigos
10. Os códigos-fonte dos algoritmos deverão ser enviados juntamente com o relatório da atividade. No
entanto, para melhor explicar a sua implementação, no relatório poderão ser inseridos trechos do
código, ou algoritmo, conforme sua necessidade.
11. O envio da atividade deverá ser feita pelo SIGAA, até a data estabelecida na tarefa cadastrada no
sistema.
