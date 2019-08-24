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
float temp_ingresso, temp_exec, temp_inicio_exec;
bool executado;
} processo;


typedef struct escalonamento{ /*Estrutura que define informações sobre um tipo de escalonamento*/
float Tt, Tw, trocas_Contexto, tempo_Total;
} escalonamento;


/*Potótipos*/
processo* leituraDeDados(); 
void imprimeProcessos(processo *processos, int N_processos);
void escalonamento_FCFS(processo *processos, int N_processos, escalonamento *escalonador);

/*Variáveis Globais*/
int total_Processos;


int main(int argc, char **argv){
    processo *processos;
    escalonamento escalonadores[7];

    processos = leituraDeDados();

    escalonamento_FCFS(processos, total_Processos, &escalonadores[0]);
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

void escalonamento_FCFS(processo *processos, int N_processos, escalonamento *escalonador){
    float linha_do_tempo = 0;

}