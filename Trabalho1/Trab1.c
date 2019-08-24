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

typedef struct processo{ /*Estrutura que define um processo*/
int id, temp_ingresso, temp_exec, prioridade;
} processo;


/*Potótipos*/
processo* leituraDeDados(); 
void imprimeProcessos(processo *processos, int N_processos);

int main(int argc, char **argv){
    processo *processos;
    processos = leituraDeDados();
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
        fscanf (file, "%d", &processos[i].temp_ingresso);
    }

    for(i = 0; i < N_processos; i++){
        fscanf (file, "%d", &processos[i].temp_exec);
    }

    for(i = 0; i < N_processos; i++){
        fscanf (file, "%d", &processos[i].prioridade);
    }

    imprimeProcessos(processos, N_processos);    
    fclose(file);
    return processos;
}

void imprimeProcessos(processo *processos, int N_processos){ /*Imprime os processos criados*/
    int i;
    for(i = 0; i < N_processos; i++){
        printf("%d ", processos[i].temp_ingresso);
    }
    printf("\n");
    for(i = 0; i < N_processos; i++){
        printf("%d ", processos[i].temp_exec);
    }
    printf("\n");
    for(i = 0; i < N_processos; i++){
        printf("%d ", processos[i].prioridade);
    }
    printf("\n");
}