/*
Nome: Eduardo da Silva Barbosa
DRE: 116150432
---------Trabalho 1--------------
*/

/*
compilação: gcc -o Trab1 Trab1.c -ansi -Wall
*/

#include<stdlib.h>
#include<stdio.h>
#include <stdbool.h>

/*Estruturas*/
typedef struct processo{ /*Estrutura que define um processo*/
int id, prioridade;
float temp_ingresso; /*Tempo de ingresso*/
float temp_exec; /*Tempo necessário para rodar o processo*/
float temp_inicio_exec; /*Tempo em que a execução começa*/
float temp_fim_exec; /*Tempo final da execução*/
float temp_run; /*Tempo que o código já rodou*/
bool executado;
} processo;


typedef struct escalonamento{ /*Estrutura que define informações sobre um tipo de escalonamento*/
int trocas_Contexto;
float Tt, Tw, tempo_Total;
} escalonamento;


/*Potótipos*/
processo* leituraDeDados(); 
void inicializaEscalonamentos(escalonamento *escalonadores);
void imprimeProcessos(processo *processos, int N_processos);
void resetProcessos(processo *processos, int N_processos);

int menorTemp_ingresso(processo *processos, int N_processos); /*DELETAR*/


void inicializarFila(int *fila, int tam);
void incluirFila(int *fila, int tam, int processo);
void removerFila(int *fila, int tam);
void atualizarFila(processo *Processos, int *fila, int tam, int temp);
bool pertencerFila(int *fila, int tam, int processo);
void ordenarFila_menorTempExec(int *fila, int tam, processo *processos);
void ordenarFila_menorTempExecRestante(int *fila, int tam, processo *processos);

void escalonamento_FCFS(processo *processos, int N_processos, escalonamento *escalonador);
void escalonamento_RR(processo *processos, int N_processos, escalonamento *escalonador);
void escalonamento_SJF(processo *processos, int N_processos, escalonamento *escalonador);
void escalonamento_SRTF(processo *processos, int N_processos, escalonamento *escalonador);

/*Variáveis Globais*/
int total_Processos;


int main(int argc, char **argv){
    processo *processos;
    escalonamento escalonadores[7];

    processos = leituraDeDados();
    inicializaEscalonamentos(escalonadores);

    escalonamento_FCFS(processos, total_Processos, &escalonadores[0]);
    resetProcessos(processos, total_Processos);
    printf("\n");
    escalonamento_RR(processos, total_Processos, &escalonadores[1]);
    resetProcessos(processos, total_Processos);
    printf("\n");
    escalonamento_SJF(processos, total_Processos, &escalonadores[2]);
    resetProcessos(processos, total_Processos);
    printf("\n");
    escalonamento_SRTF(processos, total_Processos, &escalonadores[3]);
    return 0;
}

processo* leituraDeDados(){ /*Leitura da entrada e inicialização da estrutura de processos*/
    processo *processos;
    int counter, N_processos, i;

    FILE *file = fopen("Entrada","r");
    if(!file){
        printf("Arquivo não encontrado\n");
        exit(0);
    }

    counter = fscanf (file, "%d", &N_processos);
    if(counter == 0){
        printf("Arquivo de entrada vazio\n\n");
        exit(0);
    }

    processos = (processo*) malloc(sizeof(processo)*N_processos);

    for(i = 0; i < N_processos; i++){
        processos[i].id = i;
        processos[i].executado = false;
        fscanf (file, "%f", &processos[i].temp_ingresso);
    }

    for(i = 0; i < N_processos; i++){
        fscanf (file, "%f", &processos[i].temp_exec);
    }

    for(i = 0; i < N_processos; i++){
        fscanf (file, "%d", &processos[i].prioridade);
    }

    imprimeProcessos(processos, N_processos);    
    fclose(file);

    total_Processos = N_processos;
    return processos;
}

void inicializaEscalonamentos(escalonamento *escalonadores){
    int i;
    for(i = 0; i<4; i++){
        escalonadores[i].tempo_Total = 0;
        escalonadores[i].Tt = 0;
        escalonadores[i].Tw = 0;
        escalonadores[i].trocas_Contexto = 0;
    }
}

void imprimeProcessos(processo *processos, int N_processos){ /*Imprime os processos criados*/
    int i;
    for(i = 0; i < N_processos; i++){
        printf("%d ", (int) processos[i].temp_ingresso);
    }
    printf("\n");
    for(i = 0; i < N_processos; i++){
        printf("%d ", (int) processos[i].temp_exec);
    }
    printf("\n");
    for(i = 0; i < N_processos; i++){
        printf("%d ", processos[i].prioridade);
    }
    printf("\n");
}

void resetProcessos(processo *processos, int N_processos){
    int i;
    for(i = 0; i < N_processos; i++){
        processos[i].executado = false;
        processos[i].temp_inicio_exec = -1;
        processos[i].temp_fim_exec = -1;
        processos[i].temp_run = 0;
    }
}

/*Morrer 0 referência*/
int menorIngresso_Id(processo *processos, int N_processos){ /*Retorna o ID do processo com menor tempo de ingresso e que não tenha  sido executado. No caso de empate o que tiver o menor id*/
    int i, menorTemp_ingresso = 0;

    while(processos[menorTemp_ingresso].executado){
        menorTemp_ingresso++;
    }

    for(i = menorTemp_ingresso + 1; i < N_processos; i++){
        if((processos[i].temp_ingresso < processos[menorTemp_ingresso].temp_ingresso) && (!processos[i].executado)){
            menorTemp_ingresso = i;
        }
    }
    return menorTemp_ingresso;
}

void inicializarFila(int *fila, int tam){
    int i;
    for(i = 0; i<tam; i++){
        fila[i] = -1;
    }
}

void incluirFila(int *fila, int tam, int processo){
    int i = 0;
    while(fila[i] != -1){
        i++;
    }
    fila[i] = processo;
}

void removerFila(int *fila, int tam){ /*Remove o primeiro da fila e shifta a fila*/
    int i;
    for(i = 0; i<tam-1; i++){
        fila[i] = fila[i+1];
    }
    fila[tam-1] = -1;
}

bool pertencerFila(int *fila, int tam, int processo){
    int i; 
    for(i=0; i<tam; i++){
        if(fila[i] == processo){
            return true;
        }
    }
    return false;
}

void atualizarFila(processo *processos, int *fila, int tam, int temp){ /*Atualiza a fila com todos os processos que tenha surgido e não executado até o tempo temp*/ 
    int i;
    for(i = 0; i<tam; i++){
        if(processos[i].temp_ingresso <= temp && !pertencerFila(fila, tam, i) && !processos[i].executado){
            incluirFila(fila, tam, i);
        }
    }
}

void ordenarFila_menorTempExec(int *fila, int tam, processo *processos){ /*Ordena os processos da fila pelo tempo de execução*/
    int i, j, temp, numProcessos = 0;
    while(fila[numProcessos] != -1){
        numProcessos++;
    }

    for(i=0; i<numProcessos-1; i++){
        for(j=0; j<numProcessos-i-1; j++){
            if(processos[fila[j]].temp_exec > processos[fila[j+1]].temp_exec){
                temp = fila[j];
                fila[j] = fila[j+1];
                fila[j+1] = temp;
            }
        }
    }

}

void ordenarFila_menorTempExecRestante(int *fila, int tam, processo *processos){/*Ordena os processos da fila pelo tempo restante de execução*/
    int i, j, temp, numProcessos = 0;
    while(fila[numProcessos] != -1){
        numProcessos++;
    }

    for(i=0; i<numProcessos-1; i++){
        for(j=0; j<numProcessos-i-1; j++){
            if(processos[fila[j]].temp_exec - processos[fila[j]].temp_run > processos[fila[j+1]].temp_exec - processos[fila[j+1]].temp_run){
                temp = fila[j];
                fila[j] = fila[j+1];
                fila[j+1] = temp;
            }
        }
    }
}

void escalonamento_FCFS(processo *processos, int N_processos, escalonamento *escalonador){
    int prox_processo;
    int fila[N_processos];

    inicializarFila(fila, N_processos);
    atualizarFila(processos, fila, N_processos, (*escalonador).tempo_Total);

    while(fila[0] != -1){ /*tempo_total é usado também como uma linha do tempo*/
        /*prox_processo = menorIngresso_Id(processos,N_processos);*/
        atualizarFila(processos, fila, N_processos, (*escalonador).tempo_Total);
        prox_processo = fila[0];

        processos[prox_processo].temp_inicio_exec = (*escalonador).tempo_Total;
        processos[prox_processo].temp_fim_exec = processos[prox_processo].temp_inicio_exec + processos[prox_processo].temp_exec;

        (*escalonador).Tt += processos[prox_processo].temp_fim_exec  - processos[prox_processo].temp_ingresso;
        (*escalonador).Tw += processos[prox_processo].temp_fim_exec - processos[prox_processo].temp_ingresso - processos[prox_processo].temp_exec;
        (*escalonador).tempo_Total += processos[prox_processo].temp_exec;
        processos[prox_processo].executado = true;
        removerFila(fila, N_processos);
    }

    (*escalonador).Tt = (*escalonador).Tt/N_processos;
    (*escalonador).Tw = (*escalonador).Tw/N_processos;
    (*escalonador).trocas_Contexto = N_processos - 1;

    printf("Tempo médio Tt = %f\n", (*escalonador).Tt);
    printf("Tempo médio Tw = %f\n", (*escalonador).Tw);
    printf("Trocas de contexto = %d\n", (*escalonador).trocas_Contexto);
    printf("Tempo total = %f\n", (*escalonador).tempo_Total);
}

void escalonamento_RR(processo *processos, int N_processos, escalonamento *escalonador){
    int prox_processo;
    int tq = 2;
    int fila[N_processos];

    inicializarFila(fila, N_processos);
    atualizarFila(processos, fila, N_processos, (*escalonador).tempo_Total);

    while(fila[0] != -1){ /*Enquanto a fila não estiver vázia*/
        atualizarFila(processos, fila, N_processos, (*escalonador).tempo_Total);
        prox_processo = fila[0];
       /* printf("cara da fila = %d\n", fila[0]);*/
        if(processos[prox_processo].temp_inicio_exec == -1){
            processos[prox_processo].temp_inicio_exec = (*escalonador).tempo_Total;
        }
        if((processos[prox_processo].temp_exec - processos[prox_processo].temp_run) <= tq){
            (*escalonador).tempo_Total += processos[prox_processo].temp_exec - processos[prox_processo].temp_run;
            processos[prox_processo].temp_fim_exec = (*escalonador).tempo_Total;
            processos[prox_processo].temp_run += processos[prox_processo].temp_exec;
            processos[prox_processo].executado = true;

            (*escalonador).Tt += processos[prox_processo].temp_fim_exec - processos[prox_processo].temp_ingresso;
            (*escalonador).Tw += processos[prox_processo].temp_fim_exec - processos[prox_processo].temp_ingresso - processos[prox_processo].temp_exec;
            (*escalonador).trocas_Contexto++;
            removerFila(fila, N_processos);
            /*SAI DA FILA - TERMINOU*/
        }else{
            processos[prox_processo].temp_run += tq;
            (*escalonador).tempo_Total += tq;
            (*escalonador).trocas_Contexto++;
            atualizarFila(processos, fila, N_processos, (*escalonador).tempo_Total);
            removerFila(fila, N_processos);
            incluirFila(fila, N_processos, prox_processo);
            /*VAI PARA O FIM DA FILA*/
        }
        
    }

    (*escalonador).Tt = (*escalonador).Tt/N_processos;
    (*escalonador).Tw = (*escalonador).Tw/N_processos;
    (*escalonador).trocas_Contexto--;    

    printf("Tempo médio Tt = %f\n", (*escalonador).Tt);
    printf("Tempo médio Tw = %f\n", (*escalonador).Tw);
    printf("Trocas de contexto = %d\n", (*escalonador).trocas_Contexto);
    printf("Tempo total = %f\n", (*escalonador).tempo_Total);

}

void escalonamento_SJF(processo *processos, int N_processos, escalonamento *escalonador){
    int prox_processo;
    int fila[N_processos];

    inicializarFila(fila, N_processos);
    atualizarFila(processos, fila, N_processos, (*escalonador).tempo_Total);
    ordenarFila_menorTempExec(fila, N_processos, processos);

    while(fila[0] != -1){ /*Enquanto a fila não estiver vázia*/
        atualizarFila(processos, fila, N_processos, (*escalonador).tempo_Total);
        ordenarFila_menorTempExec(fila, N_processos, processos);
        prox_processo = fila[0];
       
        processos[prox_processo].temp_inicio_exec = (*escalonador).tempo_Total;  
        processos[prox_processo].temp_fim_exec = processos[prox_processo].temp_inicio_exec + processos[prox_processo].temp_exec;


        (*escalonador).Tt += processos[prox_processo].temp_fim_exec - processos[prox_processo].temp_ingresso;
        (*escalonador).Tw += processos[prox_processo].temp_inicio_exec - processos[prox_processo].temp_ingresso;
        (*escalonador).tempo_Total += processos[prox_processo].temp_exec;
        (*escalonador).trocas_Contexto++; 
        processos[prox_processo].executado = true;
        removerFila(fila, N_processos);


    }

    (*escalonador).Tt = (*escalonador).Tt/N_processos;
    (*escalonador).Tw = (*escalonador).Tw/N_processos;
    (*escalonador).trocas_Contexto--;    

    printf("Tempo médio Tt = %f\n", (*escalonador).Tt);
    printf("Tempo médio Tw = %f\n", (*escalonador).Tw);
    printf("Trocas de contexto = %d\n", (*escalonador).trocas_Contexto);
    printf("Tempo total = %f\n", (*escalonador).tempo_Total);
}

void escalonamento_SRTF(processo *processos, int N_processos, escalonamento *escalonador){
    int prox_processo, fila[N_processos];

    inicializarFila(fila, N_processos);
    atualizarFila(processos, fila, N_processos, (*escalonador).tempo_Total);
    ordenarFila_menorTempExecRestante(fila, N_processos, processos);

    while(fila[0] != -1){
        atualizarFila(processos, fila, N_processos, (*escalonador).tempo_Total);
        ordenarFila_menorTempExecRestante(fila, N_processos, processos);
        prox_processo = fila[0];
        
        if(processos[prox_processo].temp_inicio_exec == -1){
            processos[prox_processo].temp_inicio_exec = (*escalonador).tempo_Total;
        }

        if((processos[prox_processo].temp_exec - processos[prox_processo].temp_run) <= 1){
            (*escalonador).tempo_Total++;
            processos[prox_processo].temp_fim_exec = (*escalonador).tempo_Total;
            processos[prox_processo].temp_run += processos[prox_processo].temp_exec;
            processos[prox_processo].executado = true;

            (*escalonador).Tt += processos[prox_processo].temp_fim_exec - processos[prox_processo].temp_ingresso;
            (*escalonador).Tw += processos[prox_processo].temp_fim_exec - processos[prox_processo].temp_ingresso - processos[prox_processo].temp_exec;
            removerFila(fila, N_processos);
        }else{
            processos[prox_processo].temp_run += 1;
            (*escalonador).tempo_Total ++;
        }

        if(fila[0] != prox_processo){
            (*escalonador).trocas_Contexto++;
        }

    }

    (*escalonador).Tt = (*escalonador).Tt/N_processos;
    (*escalonador).Tw = (*escalonador).Tw/N_processos;  

    printf("Tempo médio Tt = %f\n", (*escalonador).Tt);
    printf("Tempo médio Tw = %f\n", (*escalonador).Tw);
    printf("Trocas de contexto = %d\n", (*escalonador).trocas_Contexto);
    printf("Tempo total = %f\n", (*escalonador).tempo_Total);

}