/*
Compilar: g++ -o simula-vm simula-vm.cpp -Wall -ansi --std=c++17
Executar: ./simula-vm "Número de quadros" < "refs.txt"
*/

#include<iostream>
#include<string.h>
#include<stdio.h>
#include<vector>

using namespace std;

void readRefs(vector<int> &refs); /*Faz a leitura das referências e coloca no vetor passado.*/
void printVector(vector<int> &vetor); /*Imprime o vetor passado.*/
int find(vector<int> &vetor, int num); /*Verifica a existência de num no vetor passado. Caso encontre retorna a posição caso contrário retorna -1.*/

/*Algoritmos*/
int FIFO(vector<int> &refs, vector<int> &frame, int size_frame); /*Simula o algortimo FIFO e retorna a quantidade de falhas de páginas.*/
int LRU(vector<int> &refs, vector<int> &frame, int size_frame); /*Simula o algortimo LRU e retorna a quantidade de falhas de páginas.*/
int OPT(vector<int> &refs, vector<int> &frame, int size_frame); /*Simula o algortimo OPT e retorna a quantidade de falhas de páginas.*/

int main(int argc, char *argv[]){
vector<int> refs, frame;
int size_frame = 0, pageFaults_FIFO = 0, pageFaults_LRU = 0, pageFaults_OPT = 0; 

    if(argc < 2){ /*Verifica se foi passada a quantidade de argumentos correto.*/
        cerr << "Número de argumentos insuficiente, insira a quantidade de quadros!" << endl;
        exit(1);
    }else{
        size_frame = *argv[1] - '0'; //Convertendo de char para inteiro (Através da tabela ASCII).
    }

    readRefs(refs);

    pageFaults_FIFO = FIFO(refs, frame, size_frame);
    pageFaults_LRU = LRU(refs, frame, size_frame);


    printf ("%5d quadros, %7d refs: FIFO: %5d PFs, LRU: %5d PFs, OPT: %5d PFs\n", size_frame, (int)refs.size(), pageFaults_FIFO, pageFaults_LRU, pageFaults_OPT);

    return 0;

}

void readRefs(vector<int> &refs){
    int i;
    while (!cin.eof()){
        cin >> i;
        refs.push_back(i);
    }
}

void printVector(vector<int> &vetor){
    for(auto const i: vetor){
        cout << i << " | ";
    }
    cout << endl;
}

int find(vector<int> &vetor, int num){
    for(unsigned int i = 0; i<vetor.size(); i++){
        if(vetor[i] == num)
            return i;
    }
    return -1;
}

int FIFO(vector<int> &refs, vector<int> &frame, int size_frame){
    int pageFaults=0;
    
    frame.assign(size_frame,-1);

    for(unsigned int i = 0; i < refs.size(); i++){
        if(find(frame, refs[i]) == -1){
            frame[pageFaults%size_frame] = refs[i];
            pageFaults++;
        }
        //printVector(frame);
    }
    return pageFaults;
}

int LRU(vector<int> &refs, vector<int> &frame, int size_frame){

}