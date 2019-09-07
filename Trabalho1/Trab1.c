/*
compilação: gcc -o Trab1 Trab1.c -ansi -Wall
*/

/*Bibliotecas*/
#include<stdlib.h>
#include<stdio.h>
#include <stdbool.h>

/*Estruturas*/
typedef struct tarefa{ /*Estrutura que define um tarefa.*/
    int id, prioridade; /*ID e prioridade estática de uma tarefa*/
    int prioridade_dinamica; /*Prioridade dinâmica de uma tarefa*/
    float temp_ingresso; /*Tempo de ingresso*/
    float temp_exec; /*Tempo necessário para a execução uma tarefa*/
    float temp_inicio_exec; /*Tempo em que a tarefa começou a ser executada*/
    float temp_fim_exec; /*Tempo em que a tarefa parou de ser executada*/
    float temp_run; /*Tempo que o código já rodou*/
    bool executado; /*Define se a tarefa já terminou ou não de ser executada*/
} tarefa;


typedef struct escalonamento{ /*Estrutura que define informações sobre um tipo de escalonamento.*/
    int trocas_Contexto;
    float Tt, Tw, tempo_Total;
} escalonamento;


/*Potótipos*/
tarefa* leituraDeDados(); /*Faz a leitura dos dados do arquivo de entrada e cria um array com todas as tarefas.*/
void imprimeTarefas(tarefa *tarefas, int N_tarefas); /*Imprime a tabela de tarefas criadas, informando o ingresso, duração e prioridade.*/
void resetTarefas(tarefa *tarefas, int N_tarefas); /*Reseta as variáveis de todas as tarefas. OBS: as variáveis com informação lida do arquivo de entrada é mantida.*/
bool existeTrabalho(tarefa *tarefas, int N_tarefas); /*Verifica se ainda existe tarefas que não tenha sido executada*/
void inicializaEscalonamentos(escalonamento *escalonadores); /*Inicializa a estrutura que define cada tipo de escalonamento.*/
void imprimeEscalonamentos(escalonamento escalonadores); /*Imprime o resultado do escalonamento.*/

void inicializarFila(int *fila, int tam); /*Inicializa a fila que guardará a ordem das tarefas com -1.*/
void incluirFila(int *fila, int tam, int tarefa); /*Inclui a tarefa no final da fila.*/
void removerFila(int *fila, int tam); /*Remove a primeira tarefa da fila.*/
bool atualizarFila(tarefa *tarefas, int *fila, int tam, int temp); /*Atualiza a fila com todas as tarefas que tenha surgido e não executada até o tempo temp, retornando false caso nenhuma tarefa nova seja incluida ou true caso contrário. OBS: Todas as tarefas são incluidas no final da fila.*/ 
bool pertencerFila(int *fila, int tam, int tarefa); /*Verifica se uma tarefa já pertence a fila.*/
void ordenarFila_menorTempExec(int *fila, int tam, tarefa *tarefas); /*Ordena as tarefas da fila pelo tempo de execução (Da menor para a maior).*/
void ordenarFila_menorTempExecRestante(int *fila, int tam, tarefa *tarefas); /*Ordena as tarefas da fila pelo tempo restante de execução (Da menor para a maior).*/
void ordenarFila_maiorPrioridade(int *fila, int N_tarefas, tarefa *tarefas); /*Ordena as tarefas da fila pela ordem de prioridade (Da maior pra a menor).*/
void ordenarFila_maiorPrioridadeDinamica(int *fila, int N_tarefas, tarefa *tarefas); /*Ordena as tarefas da fila pela ordem de prioridade dinâmica (Da maior para a menor*/

void escalonamento_FCFS(tarefa *tarefas, int N_tarefas, escalonamento *escalonador);
void escalonamento_RR(tarefa *tarefas, int N_tarefas, escalonamento *escalonador);
void escalonamento_SJF(tarefa *tarefas, int N_tarefas, escalonamento *escalonador);
void escalonamento_SRTF(tarefa *tarefas, int N_tarefas, escalonamento *escalonador);
void escalonamento_PRIOc(tarefa *tarefas, int N_tarefas, escalonamento *escalonador);
void escalonamento_PRIOp(tarefa *tarefas, int N_tarefas, escalonamento *escalonador);
void escalonamento_PRIOd(tarefa *tarefas, int N_tarefas, escalonamento *escalonador);

/*Variáveis Globais*/
int total_Tarefas;
int total_Escalonamentos = 7;

/*Main*/
int main(int argc, char **argv){
    tarefa *tarefas;
    escalonamento escalonadores[total_Escalonamentos];

    tarefas = leituraDeDados();
    inicializaEscalonamentos(escalonadores);
    
    resetTarefas(tarefas, total_Tarefas);
    escalonamento_FCFS(tarefas, total_Tarefas, &escalonadores[0]);
    
    resetTarefas(tarefas, total_Tarefas);
    escalonamento_RR(tarefas, total_Tarefas, &escalonadores[1]);
    
    resetTarefas(tarefas, total_Tarefas);
    escalonamento_SJF(tarefas, total_Tarefas, &escalonadores[2]);
    
    resetTarefas(tarefas, total_Tarefas);
    escalonamento_SRTF(tarefas, total_Tarefas, &escalonadores[3]);
    
    resetTarefas(tarefas, total_Tarefas);
    escalonamento_PRIOc(tarefas, total_Tarefas, &escalonadores[4]);
    
    resetTarefas(tarefas, total_Tarefas);
    escalonamento_PRIOp(tarefas, total_Tarefas, &escalonadores[5]);
    
    resetTarefas(tarefas, total_Tarefas);
    escalonamento_PRIOd(tarefas, total_Tarefas, &escalonadores[6]);

    return 0;
}

/*Funções*/
tarefa* leituraDeDados(){
    tarefa *tarefas;
    int counter, N_tarefa, i;

    FILE *file = fopen("Entrada","r");
    if(!file){
        printf("Arquivo não encontrado\n");
        exit(0);
    }

    counter = fscanf (file, "%d", &N_tarefa);
    if(counter == 0){
        printf("Arquivo de entrada vazio\n\n");
        exit(0);
    }

    tarefas = (tarefa*) malloc(sizeof(tarefa)*N_tarefa);

    for(i = 0; i < N_tarefa; i++){
        tarefas[i].id = i;
        fscanf (file, "%f", &tarefas[i].temp_ingresso);
    }

    for(i = 0; i < N_tarefa; i++){
        fscanf (file, "%f", &tarefas[i].temp_exec);
    }

    for(i = 0; i < N_tarefa; i++){
        fscanf (file, "%d", &tarefas[i].prioridade);
    }

    imprimeTarefas(tarefas, N_tarefa);    
    fclose(file);

    total_Tarefas = N_tarefa;
    return tarefas;
}

void imprimeTarefas(tarefa *tarefas, int N_tarefas){
    int i;
    printf("            ");
    for(i = 0; i<N_tarefas; i++){
        printf("P%d ", i+1);
    }
    printf("\n");
    for(i = 0; i < N_tarefas; i++){
        if(i == 0){
          printf("Ingresso    ");  
        }
        printf("%d  ", (int) tarefas[i].temp_ingresso);
    }
    printf("\n");
    for(i = 0; i < N_tarefas; i++){
        if(i == 0){
          printf("Duração     ");  
        }
        printf("%d  ", (int) tarefas[i].temp_exec);
    }
    printf("\n");
    for(i = 0; i < N_tarefas; i++){
        if(i == 0){
          printf("Prioridade  ");  
        }
        printf("%d  ", tarefas[i].prioridade);
    }
    printf("\n\n");
}

void resetTarefas(tarefa *tarefas, int N_tarefas){
    int i;
    for(i = 0; i < N_tarefas; i++){
        tarefas[i].executado = false;
        tarefas[i].temp_inicio_exec = -1;
        tarefas[i].temp_fim_exec = -1;
        tarefas[i].temp_run = 0;
        tarefas[i].prioridade_dinamica = tarefas[i].prioridade;
    }
}

bool existeTrabalho(tarefa *tarefas, int N_tarefas){
    int i;
    for(i = 0; i < N_tarefas; i++){
        if(tarefas[i].executado == false)
            return true;
    }
    return false;
}

void inicializaEscalonamentos(escalonamento *escalonadores){
    int i;
    for(i = 0; i<total_Escalonamentos; i++){
        escalonadores[i].tempo_Total = 0;
        escalonadores[i].Tt = 0;
        escalonadores[i].Tw = 0;
        escalonadores[i].trocas_Contexto = 0;
    }
}

void imprimeEscalonamentos(escalonamento escalonadores){
    printf("Tempo médio Tt = %f\n",  escalonadores.Tt );
    printf("Tempo médio Tw = %f\n",  escalonadores.Tw);
    printf("Trocas de contexto = %d\n", escalonadores.trocas_Contexto);
    printf("Tempo total = %f\n", escalonadores.tempo_Total);
    printf("\n");
}


void inicializarFila(int *fila, int tam){
    int i;
    for(i = 0; i<tam; i++){
        fila[i] = -1;
    }
}

void incluirFila(int *fila, int tam, int tarefa){
    int i = 0;
    while(fila[i] != -1){
        i++;
    }
    fila[i] = tarefa;
}

void removerFila(int *fila, int tam){
    int i;
    for(i = 0; i<tam-1; i++){
        fila[i] = fila[i+1];
    }
    fila[tam-1] = -1;
}

bool atualizarFila(tarefa *tarefas, int *fila, int tam, int temp){  
    int i, cont = 0;
    for(i = 0; i<tam; i++){
        if(tarefas[i].temp_ingresso <= temp && !pertencerFila(fila, tam, i) && !tarefas[i].executado){
            incluirFila(fila, tam, i);
            cont++;
        }
    }
    return cont > 0 ? true : false;
}

bool pertencerFila(int *fila, int tam, int tarefa){
    int i; 
    for(i=0; i<tam; i++){
        if(fila[i] == tarefa){
            return true;
        }
    }
    return false;
}

void ordenarFila_menorTempExec(int *fila, int tam, tarefa *tarefas){ 
    int i, j, temp, numTarefas = 0;
    while(fila[numTarefas] != -1){
        numTarefas++;
    }

    for(i=0; i<numTarefas-1; i++){
        for(j=0; j<numTarefas-i-1; j++){
            if(tarefas[fila[j]].temp_exec > tarefas[fila[j+1]].temp_exec){
                temp = fila[j];
                fila[j] = fila[j+1];
                fila[j+1] = temp;
            }
        }
    }

}

void ordenarFila_menorTempExecRestante(int *fila, int tam, tarefa *tarefas){ 
    int i, j, temp, numTarefas = 0;
    while(fila[numTarefas] != -1){
        numTarefas++;
    }

    for(i=0; i<numTarefas-1; i++){
        for(j=0; j<numTarefas-i-1; j++){
            if(tarefas[fila[j]].temp_exec - tarefas[fila[j]].temp_run > tarefas[fila[j+1]].temp_exec - tarefas[fila[j+1]].temp_run){
                temp = fila[j];
                fila[j] = fila[j+1];
                fila[j+1] = temp;
            }
        }
    }
}

void ordenarFila_maiorPrioridade(int *fila, int N_tarefas, tarefa *tarefas){ 
    int i, j, temp, numTarefas = 0;
    while(fila[numTarefas] != -1){
        numTarefas++;
    }

    for(i=0; i<numTarefas-1; i++){
        for(j=0; j<numTarefas-i-1; j++){
            if(tarefas[fila[j]].prioridade < tarefas[fila[j+1]].prioridade){
                temp = fila[j];
                fila[j] = fila[j+1];
                fila[j+1] = temp;
            }
        }
    }
}

void ordenarFila_maiorPrioridadeDinamica(int *fila, int N_tarefas, tarefa *tarefas){
        int i, j, temp, numTarefas = 0;
    while(fila[numTarefas] != -1){
        numTarefas++;
    }

    for(i=0; i<numTarefas-1; i++){
        for(j=0; j<numTarefas-i-1; j++){
            if(tarefas[fila[j]].prioridade_dinamica < tarefas[fila[j+1]].prioridade_dinamica){
                temp = fila[j];
                fila[j] = fila[j+1];
                fila[j+1] = temp;
            }
        }
    }
}


void escalonamento_FCFS(tarefa *tarefas, int N_tarefas, escalonamento *escalonador){
    int prox_tarefa = -1, fila[N_tarefas], intervalo_temp = 1; /*Intervalo_temp representa o intervalo em que o sistema vai verificar o possível surgimento de uma nova tarefa e adicionar na fila*/

    inicializarFila(fila, N_tarefas);
    atualizarFila(tarefas, fila, N_tarefas, (*escalonador).tempo_Total);

    while(fila[0] != -1 && existeTrabalho(tarefas,N_tarefas)){ /*Enquanto a fila não estiver vázia e ainda existir tarefas para ser realizado.*/
        atualizarFila(tarefas, fila, N_tarefas, (*escalonador).tempo_Total); /*tempo_total é usado também como uma linha do tempo.*/
        if(fila[0] != prox_tarefa && prox_tarefa != -1){ /*Verifica se houve troca de contexto.*/
            (*escalonador).trocas_Contexto++;
        }
        prox_tarefa = fila[0];

        if(tarefas[prox_tarefa].temp_inicio_exec == -1){
            tarefas[prox_tarefa].temp_inicio_exec = (*escalonador).tempo_Total;
        }

        if((tarefas[prox_tarefa].temp_exec - tarefas[prox_tarefa].temp_run) <= intervalo_temp){
            (*escalonador).tempo_Total += intervalo_temp;
            tarefas[prox_tarefa].temp_fim_exec = (*escalonador).tempo_Total;
            tarefas[prox_tarefa].temp_run += tarefas[prox_tarefa].temp_exec;
            tarefas[prox_tarefa].executado = true;

            (*escalonador).Tt += tarefas[prox_tarefa].temp_fim_exec - tarefas[prox_tarefa].temp_ingresso;
            (*escalonador).Tw += tarefas[prox_tarefa].temp_fim_exec - tarefas[prox_tarefa].temp_ingresso - tarefas[prox_tarefa].temp_exec;
            removerFila(fila, N_tarefas);
        }else{
            tarefas[prox_tarefa].temp_run += intervalo_temp;
            (*escalonador).tempo_Total += intervalo_temp;
        }
    }

    (*escalonador).Tt = (*escalonador).Tt/N_tarefas;
    (*escalonador).Tw = (*escalonador).Tw/N_tarefas;
    puts("FCFS");
    imprimeEscalonamentos(*escalonador);
}

void escalonamento_RR(tarefa *tarefas, int N_tarefas, escalonamento *escalonador){
    int prox_tarefa = -1;
    int tq = 2;
    int fila[N_tarefas];

    inicializarFila(fila, N_tarefas);
    atualizarFila(tarefas, fila, N_tarefas, (*escalonador).tempo_Total);

    while(fila[0] != -1  && existeTrabalho(tarefas,N_tarefas)){ /*Enquanto a fila não estiver vázia e ainda existir tarefas para ser realizado.*/
        atualizarFila(tarefas, fila, N_tarefas, (*escalonador).tempo_Total); /*tempo_total é usado também como uma linha do tempo.*/
        if(fila[0] != prox_tarefa && prox_tarefa != -1){ /*Verifica se houve troca de contexto.*/
            (*escalonador).trocas_Contexto++;
        }
        prox_tarefa = fila[0];
        
        if(tarefas[prox_tarefa].temp_inicio_exec == -1){
            tarefas[prox_tarefa].temp_inicio_exec = (*escalonador).tempo_Total;
        }
        if((tarefas[prox_tarefa].temp_exec - tarefas[prox_tarefa].temp_run) <= tq){
            (*escalonador).tempo_Total += tarefas[prox_tarefa].temp_exec - tarefas[prox_tarefa].temp_run;
            tarefas[prox_tarefa].temp_fim_exec = (*escalonador).tempo_Total;
            tarefas[prox_tarefa].temp_run += tarefas[prox_tarefa].temp_exec;
            tarefas[prox_tarefa].executado = true;

            (*escalonador).Tt += tarefas[prox_tarefa].temp_fim_exec - tarefas[prox_tarefa].temp_ingresso;
            (*escalonador).Tw += tarefas[prox_tarefa].temp_fim_exec - tarefas[prox_tarefa].temp_ingresso - tarefas[prox_tarefa].temp_exec;
            removerFila(fila, N_tarefas);
        }else{
            tarefas[prox_tarefa].temp_run += tq;
            (*escalonador).tempo_Total += tq;
            atualizarFila(tarefas, fila, N_tarefas, (*escalonador).tempo_Total);
            removerFila(fila, N_tarefas);
            incluirFila(fila, N_tarefas, prox_tarefa);
        }
        
    }

    (*escalonador).Tt = (*escalonador).Tt/N_tarefas;
    (*escalonador).Tw = (*escalonador).Tw/N_tarefas;
    puts("RR");
    imprimeEscalonamentos(*escalonador);
}

void escalonamento_SJF(tarefa *tarefas, int N_tarefas, escalonamento *escalonador){
    int prox_tarefa = -1, fila[N_tarefas], intervalo_temp = 1; /*Intervalo_temp representa o intervalo em que o sistema vai verificar o possível surgimento de uma nova tarefa e adicionar na fila*/


    inicializarFila(fila, N_tarefas);
    atualizarFila(tarefas, fila, N_tarefas, (*escalonador).tempo_Total);
    ordenarFila_menorTempExec(fila, N_tarefas, tarefas);

    while(fila[0] != -1  && existeTrabalho(tarefas,N_tarefas)){ /*Enquanto a fila não estiver vázia e ainda existir tarefas para ser realizado.*/
        prox_tarefa = fila[0];
       
        if(tarefas[prox_tarefa].temp_inicio_exec == -1){
            tarefas[prox_tarefa].temp_inicio_exec = (*escalonador).tempo_Total;
        }

        if((tarefas[prox_tarefa].temp_exec - tarefas[prox_tarefa].temp_run) <= intervalo_temp){
            (*escalonador).tempo_Total += intervalo_temp;
            tarefas[prox_tarefa].temp_fim_exec = (*escalonador).tempo_Total;
            tarefas[prox_tarefa].temp_run += tarefas[prox_tarefa].temp_exec;
            tarefas[prox_tarefa].executado = true;

            (*escalonador).Tt += tarefas[prox_tarefa].temp_fim_exec - tarefas[prox_tarefa].temp_ingresso;
            (*escalonador).Tw += tarefas[prox_tarefa].temp_fim_exec - tarefas[prox_tarefa].temp_ingresso - tarefas[prox_tarefa].temp_exec;
            removerFila(fila, N_tarefas);

            atualizarFila(tarefas, fila, N_tarefas, (*escalonador).tempo_Total);
            ordenarFila_menorTempExec(fila, N_tarefas, tarefas);
            if(fila[0] != prox_tarefa && prox_tarefa != -1 && fila[0] != -1){
                (*escalonador).trocas_Contexto++;
            }

        }else{
            tarefas[prox_tarefa].temp_run += intervalo_temp;
            (*escalonador).tempo_Total += intervalo_temp;
            atualizarFila(tarefas, fila, N_tarefas, (*escalonador).tempo_Total);
        }

    }

    (*escalonador).Tt = (*escalonador).Tt/N_tarefas;
    (*escalonador).Tw = (*escalonador).Tw/N_tarefas;
    puts("SJF");
    imprimeEscalonamentos(*escalonador);
}

void escalonamento_SRTF(tarefa *tarefas, int N_tarefas, escalonamento *escalonador){
    int prox_tarefa = -1 , fila[N_tarefas], intervalo_temp = 1; /*Intervalo_temp representa o intervalo em que o sistema vai verificar o possível surgimento de uma nova tarefa e adicionar na fila*/


    inicializarFila(fila, N_tarefas);
    atualizarFila(tarefas, fila, N_tarefas, (*escalonador).tempo_Total);
    ordenarFila_menorTempExecRestante(fila, N_tarefas, tarefas);

    while(fila[0] != -1  && existeTrabalho(tarefas,N_tarefas)){
        atualizarFila(tarefas, fila, N_tarefas, (*escalonador).tempo_Total);
        ordenarFila_menorTempExecRestante(fila, N_tarefas, tarefas);
        if(fila[0] != prox_tarefa && prox_tarefa != -1){
            (*escalonador).trocas_Contexto++;
        }
        prox_tarefa = fila[0];

        if(tarefas[prox_tarefa].temp_inicio_exec == -1){
            tarefas[prox_tarefa].temp_inicio_exec = (*escalonador).tempo_Total;
        }

        if((tarefas[prox_tarefa].temp_exec - tarefas[prox_tarefa].temp_run) <= intervalo_temp){
            (*escalonador).tempo_Total += intervalo_temp;
            tarefas[prox_tarefa].temp_fim_exec = (*escalonador).tempo_Total;
            tarefas[prox_tarefa].temp_run += tarefas[prox_tarefa].temp_exec;
            tarefas[prox_tarefa].executado = true;

            (*escalonador).Tt += tarefas[prox_tarefa].temp_fim_exec - tarefas[prox_tarefa].temp_ingresso;
            (*escalonador).Tw += tarefas[prox_tarefa].temp_fim_exec - tarefas[prox_tarefa].temp_ingresso - tarefas[prox_tarefa].temp_exec;
            removerFila(fila, N_tarefas);
        }else{
            tarefas[prox_tarefa].temp_run += intervalo_temp;
            (*escalonador).tempo_Total += intervalo_temp;
        }

    }

    (*escalonador).Tt = (*escalonador).Tt/N_tarefas;
    (*escalonador).Tw = (*escalonador).Tw/N_tarefas;
    puts("SRTF");
    imprimeEscalonamentos(*escalonador);

}

void escalonamento_PRIOc(tarefa *tarefas, int N_tarefas, escalonamento *escalonador){
    int prox_tarefa = -1, fila[N_tarefas], intervalo_temp = 1;

    inicializarFila(fila, N_tarefas);
    atualizarFila(tarefas, fila, N_tarefas, (*escalonador).tempo_Total);
    ordenarFila_maiorPrioridade(fila, N_tarefas, tarefas);

    while(fila[0] != -1  && existeTrabalho(tarefas,N_tarefas)){
        prox_tarefa = fila[0];
        
        if(tarefas[prox_tarefa].temp_inicio_exec == -1){
            tarefas[prox_tarefa].temp_inicio_exec = (*escalonador).tempo_Total;
        }

        if((tarefas[prox_tarefa].temp_exec - tarefas[prox_tarefa].temp_run) <= intervalo_temp){
            (*escalonador).tempo_Total += intervalo_temp;
            tarefas[prox_tarefa].temp_fim_exec = (*escalonador).tempo_Total;
            tarefas[prox_tarefa].temp_run += tarefas[prox_tarefa].temp_exec;
            tarefas[prox_tarefa].executado = true;

            (*escalonador).Tt += tarefas[prox_tarefa].temp_fim_exec - tarefas[prox_tarefa].temp_ingresso;
            (*escalonador).Tw += tarefas[prox_tarefa].temp_fim_exec - tarefas[prox_tarefa].temp_ingresso - tarefas[prox_tarefa].temp_exec;
            removerFila(fila, N_tarefas);

            atualizarFila(tarefas, fila, N_tarefas, (*escalonador).tempo_Total);
            ordenarFila_maiorPrioridade(fila, N_tarefas, tarefas);
            if(fila[0] != prox_tarefa && prox_tarefa != -1 && fila[0] != -1){
                (*escalonador).trocas_Contexto++;
            }

        }else{
            tarefas[prox_tarefa].temp_run += intervalo_temp;
            (*escalonador).tempo_Total += intervalo_temp;
            atualizarFila(tarefas, fila, N_tarefas, (*escalonador).tempo_Total);
        }

    }

    (*escalonador).Tt = (*escalonador).Tt/N_tarefas;
    (*escalonador).Tw = (*escalonador).Tw/N_tarefas;
    puts("PRIOc");
    imprimeEscalonamentos(*escalonador);    

}

void escalonamento_PRIOp(tarefa *tarefas, int N_tarefas, escalonamento *escalonador){
    int prox_tarefa = -1, fila[N_tarefas], intervalo_temp = 1;

    inicializarFila(fila, N_tarefas);
    atualizarFila(tarefas, fila, N_tarefas, (*escalonador).tempo_Total);
    ordenarFila_maiorPrioridade(fila, N_tarefas, tarefas);
    

    while(fila[0] != -1  && existeTrabalho(tarefas,N_tarefas)){
        atualizarFila(tarefas, fila, N_tarefas, (*escalonador).tempo_Total);
        ordenarFila_maiorPrioridade(fila, N_tarefas, tarefas);
        if(fila[0] != prox_tarefa && prox_tarefa != -1){
            (*escalonador).trocas_Contexto++;
        }
        prox_tarefa = fila[0];

        if(tarefas[prox_tarefa].temp_inicio_exec == -1){
            tarefas[prox_tarefa].temp_inicio_exec = (*escalonador).tempo_Total;
        }

        if((tarefas[prox_tarefa].temp_exec - tarefas[prox_tarefa].temp_run) <= intervalo_temp){
            (*escalonador).tempo_Total += intervalo_temp;
            tarefas[prox_tarefa].temp_fim_exec = (*escalonador).tempo_Total;
            tarefas[prox_tarefa].temp_run += tarefas[prox_tarefa].temp_exec;
            tarefas[prox_tarefa].executado = true;

            (*escalonador).Tt += tarefas[prox_tarefa].temp_fim_exec - tarefas[prox_tarefa].temp_ingresso;
            (*escalonador).Tw += tarefas[prox_tarefa].temp_fim_exec - tarefas[prox_tarefa].temp_ingresso - tarefas[prox_tarefa].temp_exec;
            removerFila(fila, N_tarefas);
        }else{
            tarefas[prox_tarefa].temp_run += intervalo_temp;
            (*escalonador).tempo_Total += intervalo_temp;
        }

    }

    (*escalonador).Tt = (*escalonador).Tt/N_tarefas;
    (*escalonador).Tw = (*escalonador).Tw/N_tarefas;
    puts("PRIOp");
    imprimeEscalonamentos(*escalonador);
}

void escalonamento_PRIOd(tarefa *tarefas, int N_tarefas, escalonamento *escalonador){
    int prox_tarefa = -1, i, fila[N_tarefas], intervalo_temp = 1, fator_evelhecimento = 1;
    bool status; /*Será utilizada para saber se houve a adição de uma nova tarefa na fila*/

    inicializarFila(fila, N_tarefas);

    while(existeTrabalho(tarefas,N_tarefas)){ /*Enquanto existir tarefas para ser realizado, ficará em loop.*/
        status = atualizarFila(tarefas, fila, N_tarefas, (*escalonador).tempo_Total); /*Caso alguma tarefa nova seja incluída na fila status será true*/

        if(status){ /*Caso alguma tarefa nova seja incluída na fila a fila será reordenada*/
            ordenarFila_maiorPrioridadeDinamica(fila, N_tarefas, tarefas);
        }

        if(fila[0] != prox_tarefa && prox_tarefa != -1){ /*Verifica se ocorreu troca de contexto*/
            /*printf("%d - %d\n",  prox_tarefa, fila[0]);*/
            (*escalonador).trocas_Contexto++;
        } 

        prox_tarefa = fila[0]; 

        tarefas[prox_tarefa].prioridade_dinamica = tarefas[prox_tarefa].prioridade;
        /*printf("cara da fila = %d\n", prox_tarefa);*/

        if(tarefas[prox_tarefa].temp_inicio_exec == -1){
            tarefas[prox_tarefa].temp_inicio_exec = (*escalonador).tempo_Total;
        }

        if((tarefas[prox_tarefa].temp_exec - tarefas[prox_tarefa].temp_run) <= intervalo_temp){
            (*escalonador).tempo_Total += intervalo_temp;
            tarefas[prox_tarefa].temp_fim_exec = (*escalonador).tempo_Total;
            tarefas[prox_tarefa].temp_run += tarefas[prox_tarefa].temp_exec;
            tarefas[prox_tarefa].executado = true;

            (*escalonador).Tt += tarefas[prox_tarefa].temp_fim_exec - tarefas[prox_tarefa].temp_ingresso;
            (*escalonador).Tw += tarefas[prox_tarefa].temp_fim_exec - tarefas[prox_tarefa].temp_ingresso - tarefas[prox_tarefa].temp_exec;
            removerFila(fila, N_tarefas);
        }else{
            tarefas[prox_tarefa].temp_run += intervalo_temp;
            (*escalonador).tempo_Total += intervalo_temp;
        }
        
        for(i = 0; i<N_tarefas; i++){ /*Aumentar a prioridade dinâmica*/
            if(tarefas[i].executado == false && i != prox_tarefa && pertencerFila(fila, N_tarefas, i)){
                tarefas[i].prioridade_dinamica += fator_evelhecimento;
            }
        }

    }

    (*escalonador).Tt = (*escalonador).Tt/N_tarefas;
    (*escalonador).Tw = (*escalonador).Tw/N_tarefas;
    puts("PRIOd");
    imprimeEscalonamentos(*escalonador);
}