/*
Nome: Eduardo da Silva Barbosa
DRE: 116150432
---------Trabalho 1--------------
*/

/*
compilação: gcc -o Trab1 Trab1.c -ansi -Wall
*/

/*Bibliotecas*/
#include<stdlib.h>
#include<stdio.h>
#include <stdbool.h>

/*Estruturas*/
typedef struct processo{ /*Estrutura que define um processo.*/
    int id, prioridade; /*ID e prioridade estática do processo*/
    int prioridade_dinamica; /*Prioridade dinâmica do processo*/
    float temp_ingresso; /*Tempo de ingresso*/
    float temp_exec; /*Tempo necessário para a execução do processo*/
    float temp_inicio_exec; /*Tempo em que o processo começou a ser executado*/
    float temp_fim_exec; /*Tempo em que o processo parou de ser executado*/
    float temp_run; /*Tempo que o código já rodou*/
    bool executado; /*Define se o processo já terminou ou não de ser executado*/
} processo;


typedef struct escalonamento{ /*Estrutura que define informações sobre um tipo de escalonamento.*/
    int trocas_Contexto;
    float Tt, Tw, tempo_Total;
} escalonamento;


/*Potótipos*/
processo* leituraDeDados(); /*Faz a leitura dos dados do arquivo de entrada e cria um array com todos os processos.*/
void imprimeProcessos(processo *processos, int N_processos); /*Imprime a tabela dos processos criados, informando o ingresso, duração e prioridade.*/
void resetProcessos(processo *processos, int N_processos); /*Reseta as variáveis de todos os processos. OBS: as variáveis com informação lida do arquivo de entrada é mantida.*/
bool existeTrabalho(processo *processos, int N_processos); /*Verifica se ainda existe processo que não tenha sido executado*/
void inicializaEscalonamentos(escalonamento *escalonadores); /*Inicializa a estrutura que define cada tipo de escalonamento.*/
void imprimeEscalonamentos(escalonamento *escalonadores); /*Imprime o resultado de todos os tipos de escalonamentos.*/

void inicializarFila(int *fila, int tam); /*Inicializa a fila que guardará a ordem dos processos com -1.*/
void incluirFila(int *fila, int tam, int processo); /*Inclui o processo no final da fila.*/
void removerFila(int *fila, int tam); /*Remove o primeiro processo da fila.*/
void atualizarFila(processo *Processos, int *fila, int tam, int temp); /*Atualiza a fila com todos os processos que tenha surgido e não executado até o tempo temp. OBS: Todos os processos são incluidos no final da fila.*/ 
bool pertencerFila(int *fila, int tam, int processo); /*Verifica se um processo já pertence a fila.*/
void ordenarFila_menorTempExec(int *fila, int tam, processo *processos); /*Ordena os processos da fila pelo tempo de execução (Do menor para o maior).*/
void ordenarFila_menorTempExecRestante(int *fila, int tam, processo *processos); /*Ordena os processos da fila pelo tempo restante de execução (Do menor para o maior).*/
void ordenarFila_maiorPrioridade(int *fila, int N_processos, processo *processos); /*Ordena os processos da fila pela ordem de prioridade (Da maior pra a menor).*/
void ordenarFila_maiorPrioridadeDinamica(int *fila, int N_processos, processo *processos); /*Ordena os processos da fila pela ordem de prioridade dinâmica (Da maior para a menor*/

void escalonamento_FCFS(processo *processos, int N_processos, escalonamento *escalonador);
void escalonamento_RR(processo *processos, int N_processos, escalonamento *escalonador);
void escalonamento_SJF(processo *processos, int N_processos, escalonamento *escalonador);
void escalonamento_SRTF(processo *processos, int N_processos, escalonamento *escalonador);
void escalonamento_PRIOc(processo *processos, int N_processos, escalonamento *escalonador);
void escalonamento_PRIOp(processo *processos, int N_processos, escalonamento *escalonador);
void escalonamento_PRIOd(processo *processos, int N_processos, escalonamento *escalonador);

/*Variáveis Globais*/
int total_Processos;
int total_Escalonamentos = 7;

/*Main*/
int main(int argc, char **argv){
    processo *processos;
    escalonamento escalonadores[total_Escalonamentos];

    processos = leituraDeDados();
    inicializaEscalonamentos(escalonadores);
    
    resetProcessos(processos, total_Processos);
    escalonamento_FCFS(processos, total_Processos, &escalonadores[0]);
    
    resetProcessos(processos, total_Processos);
    escalonamento_RR(processos, total_Processos, &escalonadores[1]);
    
    resetProcessos(processos, total_Processos);
    escalonamento_SJF(processos, total_Processos, &escalonadores[2]);
    
    resetProcessos(processos, total_Processos);
    escalonamento_SRTF(processos, total_Processos, &escalonadores[3]);
    
    resetProcessos(processos, total_Processos);
    escalonamento_PRIOc(processos, total_Processos, &escalonadores[4]);
    
    resetProcessos(processos, total_Processos);
    escalonamento_PRIOp(processos, total_Processos, &escalonadores[5]);
    
    resetProcessos(processos, total_Processos);
    escalonamento_PRIOd(processos, total_Processos, &escalonadores[6]);

    imprimeEscalonamentos(escalonadores);

    return 0;
}

/*Funções*/
processo* leituraDeDados(){
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

void imprimeProcessos(processo *processos, int N_processos){
    int i;
    printf("            ");
    for(i = 0; i<N_processos; i++){
        printf("P%d ", i+1);
    }
    printf("\n");
    for(i = 0; i < N_processos; i++){
        if(i == 0){
          printf("Ingresso    ");  
        }
        printf("%d  ", (int) processos[i].temp_ingresso);
    }
    printf("\n");
    for(i = 0; i < N_processos; i++){
        if(i == 0){
          printf("Duração     ");  
        }
        printf("%d  ", (int) processos[i].temp_exec);
    }
    printf("\n");
    for(i = 0; i < N_processos; i++){
        if(i == 0){
          printf("Prioridade  ");  
        }
        printf("%d  ", processos[i].prioridade);
    }
    printf("\n\n");
}

void resetProcessos(processo *processos, int N_processos){
    int i;
    for(i = 0; i < N_processos; i++){
        processos[i].executado = false;
        processos[i].temp_inicio_exec = -1;
        processos[i].temp_fim_exec = -1;
        processos[i].temp_run = 0;
        processos[i].prioridade_dinamica = processos[i].prioridade;
    }
}

bool existeTrabalho(processo *processos, int N_processos){
    int i;
    for(i = 0; i < N_processos; i++){
        if(processos[i].executado == false)
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

void imprimeEscalonamentos(escalonamento *escalonadores){
    int i;
    for(i=0; i < total_Escalonamentos; i++){
        printf("Tempo médio Tt = %f\n",  escalonadores[i].Tt );
        printf("Tempo médio Tw = %f\n",  escalonadores[i].Tw);
        printf("Trocas de contexto = %d\n", escalonadores[i].trocas_Contexto);
        printf("Tempo total = %f\n", escalonadores[i].tempo_Total);
        printf("\n");
    }
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

void removerFila(int *fila, int tam){
    int i;
    for(i = 0; i<tam-1; i++){
        fila[i] = fila[i+1];
    }
    fila[tam-1] = -1;
}

void atualizarFila(processo *processos, int *fila, int tam, int temp){  
    int i;
    for(i = 0; i<tam; i++){
        if(processos[i].temp_ingresso <= temp && !pertencerFila(fila, tam, i) && !processos[i].executado){
            incluirFila(fila, tam, i);
        }
    }
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

void ordenarFila_menorTempExec(int *fila, int tam, processo *processos){ 
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

void ordenarFila_menorTempExecRestante(int *fila, int tam, processo *processos){ 
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

void ordenarFila_maiorPrioridade(int *fila, int N_processos, processo *processos){ 
    int i, j, temp, numProcessos = 0;
    while(fila[numProcessos] != -1){
        numProcessos++;
    }

    for(i=0; i<numProcessos-1; i++){
        for(j=0; j<numProcessos-i-1; j++){
            if(processos[fila[j]].prioridade < processos[fila[j+1]].prioridade){
                temp = fila[j];
                fila[j] = fila[j+1];
                fila[j+1] = temp;
            }
        }
    }
}

void ordenarFila_maiorPrioridadeDinamica(int *fila, int N_processos, processo *processos){
        int i, j, temp, numProcessos = 0;
    while(fila[numProcessos] != -1){
        numProcessos++;
    }

    for(i=0; i<numProcessos-1; i++){
        for(j=0; j<numProcessos-i-1; j++){
            if(processos[fila[j]].prioridade_dinamica < processos[fila[j+1]].prioridade_dinamica){
                temp = fila[j];
                fila[j] = fila[j+1];
                fila[j+1] = temp;
            }
        }
    }
}


void escalonamento_FCFS(processo *processos, int N_processos, escalonamento *escalonador){
    int prox_processo = -1, fila[N_processos], intervalo_temp = 1; /*Intervalo_temp representa o intervalo em que o sistema vai verificar o possível surgimento de um novo processo e adicionar na fila*/

    inicializarFila(fila, N_processos);
    atualizarFila(processos, fila, N_processos, (*escalonador).tempo_Total);

    while(fila[0] != -1 && existeTrabalho(processos,N_processos)){ /*Enquanto a fila não estiver vázia e ainda existir processos para ser realizado.*/
        atualizarFila(processos, fila, N_processos, (*escalonador).tempo_Total); /*tempo_total é usado também como uma linha do tempo.*/
        if(fila[0] != prox_processo && prox_processo != -1){ /*Verifica se houve troca de contexto.*/
            (*escalonador).trocas_Contexto++;
        }
        prox_processo = fila[0];

        if(processos[prox_processo].temp_inicio_exec == -1){
            processos[prox_processo].temp_inicio_exec = (*escalonador).tempo_Total;
        }

        if((processos[prox_processo].temp_exec - processos[prox_processo].temp_run) <= intervalo_temp){
            (*escalonador).tempo_Total += intervalo_temp;
            processos[prox_processo].temp_fim_exec = (*escalonador).tempo_Total;
            processos[prox_processo].temp_run += processos[prox_processo].temp_exec;
            processos[prox_processo].executado = true;

            (*escalonador).Tt += processos[prox_processo].temp_fim_exec - processos[prox_processo].temp_ingresso;
            (*escalonador).Tw += processos[prox_processo].temp_fim_exec - processos[prox_processo].temp_ingresso - processos[prox_processo].temp_exec;
            removerFila(fila, N_processos);
        }else{
            processos[prox_processo].temp_run += intervalo_temp;
            (*escalonador).tempo_Total += intervalo_temp;
        }
    }

    (*escalonador).Tt = (*escalonador).Tt/N_processos;
    (*escalonador).Tw = (*escalonador).Tw/N_processos;

}

void escalonamento_RR(processo *processos, int N_processos, escalonamento *escalonador){
    int prox_processo = -1;
    int tq = 2;
    int fila[N_processos];

    inicializarFila(fila, N_processos);
    atualizarFila(processos, fila, N_processos, (*escalonador).tempo_Total);

    while(fila[0] != -1  && existeTrabalho(processos,N_processos)){ /*Enquanto a fila não estiver vázia e ainda existir processos para ser realizado.*/
        atualizarFila(processos, fila, N_processos, (*escalonador).tempo_Total); /*tempo_total é usado também como uma linha do tempo.*/
        if(fila[0] != prox_processo && prox_processo != -1){ /*Verifica se houve troca de contexto.*/
            (*escalonador).trocas_Contexto++;
        }
        prox_processo = fila[0];
        
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
            removerFila(fila, N_processos);
        }else{
            processos[prox_processo].temp_run += tq;
            (*escalonador).tempo_Total += tq;
            atualizarFila(processos, fila, N_processos, (*escalonador).tempo_Total);
            removerFila(fila, N_processos);
            incluirFila(fila, N_processos, prox_processo);
        }
        
    }

    (*escalonador).Tt = (*escalonador).Tt/N_processos;
    (*escalonador).Tw = (*escalonador).Tw/N_processos;
}

void escalonamento_SJF(processo *processos, int N_processos, escalonamento *escalonador){
    int prox_processo = -1, fila[N_processos], intervalo_temp = 1; /*Intervalo_temp representa o intervalo em que o sistema vai verificar o possível surgimento de um novo processo e adicionar na fila*/


    inicializarFila(fila, N_processos);
    atualizarFila(processos, fila, N_processos, (*escalonador).tempo_Total);
    ordenarFila_menorTempExec(fila, N_processos, processos);

    while(fila[0] != -1  && existeTrabalho(processos,N_processos)){ /*Enquanto a fila não estiver vázia e ainda existir processos para ser realizado.*/
        prox_processo = fila[0];
       
        if(processos[prox_processo].temp_inicio_exec == -1){
            processos[prox_processo].temp_inicio_exec = (*escalonador).tempo_Total;
        }

        if((processos[prox_processo].temp_exec - processos[prox_processo].temp_run) <= intervalo_temp){
            (*escalonador).tempo_Total += intervalo_temp;
            processos[prox_processo].temp_fim_exec = (*escalonador).tempo_Total;
            processos[prox_processo].temp_run += processos[prox_processo].temp_exec;
            processos[prox_processo].executado = true;

            (*escalonador).Tt += processos[prox_processo].temp_fim_exec - processos[prox_processo].temp_ingresso;
            (*escalonador).Tw += processos[prox_processo].temp_fim_exec - processos[prox_processo].temp_ingresso - processos[prox_processo].temp_exec;
            removerFila(fila, N_processos);

            atualizarFila(processos, fila, N_processos, (*escalonador).tempo_Total);
            ordenarFila_menorTempExec(fila, N_processos, processos);
            if(fila[0] != prox_processo && prox_processo != -1 && fila[0] != -1){
                (*escalonador).trocas_Contexto++;
            }

        }else{
            processos[prox_processo].temp_run += intervalo_temp;
            (*escalonador).tempo_Total += intervalo_temp;
            atualizarFila(processos, fila, N_processos, (*escalonador).tempo_Total);
        }

    }

    (*escalonador).Tt = (*escalonador).Tt/N_processos;
    (*escalonador).Tw = (*escalonador).Tw/N_processos;
}

void escalonamento_SRTF(processo *processos, int N_processos, escalonamento *escalonador){
    int prox_processo = -1 , fila[N_processos], intervalo_temp = 1; /*Intervalo_temp representa o intervalo em que o sistema vai verificar o possível surgimento de um novo processo e adicionar na fila*/


    inicializarFila(fila, N_processos);
    atualizarFila(processos, fila, N_processos, (*escalonador).tempo_Total);
    ordenarFila_menorTempExecRestante(fila, N_processos, processos);

    while(fila[0] != -1  && existeTrabalho(processos,N_processos)){
        atualizarFila(processos, fila, N_processos, (*escalonador).tempo_Total);
        ordenarFila_menorTempExecRestante(fila, N_processos, processos);
        if(fila[0] != prox_processo && prox_processo != -1){
            (*escalonador).trocas_Contexto++;
        }
        prox_processo = fila[0];

        if(processos[prox_processo].temp_inicio_exec == -1){
            processos[prox_processo].temp_inicio_exec = (*escalonador).tempo_Total;
        }

        if((processos[prox_processo].temp_exec - processos[prox_processo].temp_run) <= intervalo_temp){
            (*escalonador).tempo_Total += intervalo_temp;
            processos[prox_processo].temp_fim_exec = (*escalonador).tempo_Total;
            processos[prox_processo].temp_run += processos[prox_processo].temp_exec;
            processos[prox_processo].executado = true;

            (*escalonador).Tt += processos[prox_processo].temp_fim_exec - processos[prox_processo].temp_ingresso;
            (*escalonador).Tw += processos[prox_processo].temp_fim_exec - processos[prox_processo].temp_ingresso - processos[prox_processo].temp_exec;
            removerFila(fila, N_processos);
        }else{
            processos[prox_processo].temp_run += intervalo_temp;
            (*escalonador).tempo_Total += intervalo_temp;
        }

    }

    (*escalonador).Tt = (*escalonador).Tt/N_processos;
    (*escalonador).Tw = (*escalonador).Tw/N_processos;

}

void escalonamento_PRIOc(processo *processos, int N_processos, escalonamento *escalonador){
    int prox_processo = -1, fila[N_processos], intervalo_temp = 1;

    inicializarFila(fila, N_processos);
    atualizarFila(processos, fila, N_processos, (*escalonador).tempo_Total);
    ordenarFila_maiorPrioridade(fila, N_processos, processos);

    while(fila[0] != -1  && existeTrabalho(processos,N_processos)){
        prox_processo = fila[0];
        
        if(processos[prox_processo].temp_inicio_exec == -1){
            processos[prox_processo].temp_inicio_exec = (*escalonador).tempo_Total;
        }

        if((processos[prox_processo].temp_exec - processos[prox_processo].temp_run) <= intervalo_temp){
            (*escalonador).tempo_Total += intervalo_temp;
            processos[prox_processo].temp_fim_exec = (*escalonador).tempo_Total;
            processos[prox_processo].temp_run += processos[prox_processo].temp_exec;
            processos[prox_processo].executado = true;

            (*escalonador).Tt += processos[prox_processo].temp_fim_exec - processos[prox_processo].temp_ingresso;
            (*escalonador).Tw += processos[prox_processo].temp_fim_exec - processos[prox_processo].temp_ingresso - processos[prox_processo].temp_exec;
            removerFila(fila, N_processos);

            atualizarFila(processos, fila, N_processos, (*escalonador).tempo_Total);
            ordenarFila_maiorPrioridade(fila, N_processos, processos);
            if(fila[0] != prox_processo && prox_processo != -1 && fila[0] != -1){
                (*escalonador).trocas_Contexto++;
            }

        }else{
            processos[prox_processo].temp_run += intervalo_temp;
            (*escalonador).tempo_Total += intervalo_temp;
            atualizarFila(processos, fila, N_processos, (*escalonador).tempo_Total);
        }

    }

    (*escalonador).Tt = (*escalonador).Tt/N_processos;
    (*escalonador).Tw = (*escalonador).Tw/N_processos;    

}

void escalonamento_PRIOp(processo *processos, int N_processos, escalonamento *escalonador){
    int prox_processo = -1, fila[N_processos], intervalo_temp = 1;

    inicializarFila(fila, N_processos);
    atualizarFila(processos, fila, N_processos, (*escalonador).tempo_Total);
    ordenarFila_maiorPrioridade(fila, N_processos, processos);
    

    while(fila[0] != -1  && existeTrabalho(processos,N_processos)){
        atualizarFila(processos, fila, N_processos, (*escalonador).tempo_Total);
        ordenarFila_maiorPrioridade(fila, N_processos, processos);
        if(fila[0] != prox_processo && prox_processo != -1){
            (*escalonador).trocas_Contexto++;
        }
        prox_processo = fila[0];

        if(processos[prox_processo].temp_inicio_exec == -1){
            processos[prox_processo].temp_inicio_exec = (*escalonador).tempo_Total;
        }

        if((processos[prox_processo].temp_exec - processos[prox_processo].temp_run) <= intervalo_temp){
            (*escalonador).tempo_Total += intervalo_temp;
            processos[prox_processo].temp_fim_exec = (*escalonador).tempo_Total;
            processos[prox_processo].temp_run += processos[prox_processo].temp_exec;
            processos[prox_processo].executado = true;

            (*escalonador).Tt += processos[prox_processo].temp_fim_exec - processos[prox_processo].temp_ingresso;
            (*escalonador).Tw += processos[prox_processo].temp_fim_exec - processos[prox_processo].temp_ingresso - processos[prox_processo].temp_exec;
            removerFila(fila, N_processos);
        }else{
            processos[prox_processo].temp_run += intervalo_temp;
            (*escalonador).tempo_Total += intervalo_temp;
        }

    }

    (*escalonador).Tt = (*escalonador).Tt/N_processos;
    (*escalonador).Tw = (*escalonador).Tw/N_processos;
}

void escalonamento_PRIOd(processo *processos, int N_processos, escalonamento *escalonador){
    int prox_processo = 0, fila[N_processos], intervalo_temp = 1, fator_evelhecimento = 1;
    int i;

    inicializarFila(fila, N_processos);
    atualizarFila(processos, fila, N_processos, (*escalonador).tempo_Total);

    while(fila[0] != -1  && existeTrabalho(processos,N_processos)){
        atualizarFila(processos, fila, N_processos, (*escalonador).tempo_Total);
        ordenarFila_maiorPrioridadeDinamica(fila, N_processos, processos);
        if(fila[0] != prox_processo && prox_processo != -1){
           /* printf("%d - %d\n",  prox_processo, fila[0]);*/
            (*escalonador).trocas_Contexto++;
        }

        printf("pfila (%d) = %d e pprox ((%d)) = %d\n", fila[0], processos[fila[0]].prioridade_dinamica, prox_processo, processos[prox_processo].prioridade_dinamica );
        if(processos[fila[0]].prioridade_dinamica != processos[prox_processo].prioridade_dinamica){
            prox_processo = fila[0];
        }

        processos[prox_processo].prioridade_dinamica = processos[prox_processo].prioridade;
        printf("cara da fila = %d\n", prox_processo);

        if(processos[prox_processo].temp_inicio_exec == -1){
            processos[prox_processo].temp_inicio_exec = (*escalonador).tempo_Total;
        }

        if((processos[prox_processo].temp_exec - processos[prox_processo].temp_run) <= intervalo_temp){
            (*escalonador).tempo_Total += intervalo_temp;
            processos[prox_processo].temp_fim_exec = (*escalonador).tempo_Total;
            processos[prox_processo].temp_run += processos[prox_processo].temp_exec;
            processos[prox_processo].executado = true;

            (*escalonador).Tt += processos[prox_processo].temp_fim_exec - processos[prox_processo].temp_ingresso;
            (*escalonador).Tw += processos[prox_processo].temp_fim_exec - processos[prox_processo].temp_ingresso - processos[prox_processo].temp_exec;
            removerFila(fila, N_processos);
        }else{
            processos[prox_processo].temp_run += intervalo_temp;
            (*escalonador).tempo_Total += intervalo_temp;
        }
        
        for(i = 0; i<N_processos; i++){ /*Aumentar a prioridade dinâmica*/
            if(processos[i].executado == false && i != prox_processo){
                processos[i].prioridade_dinamica += fator_evelhecimento;
            }
        }

    }

    (*escalonador).Tt = (*escalonador).Tt/N_processos;
    (*escalonador).Tw = (*escalonador).Tw/N_processos;
}