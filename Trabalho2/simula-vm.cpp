/*
Compilar: g++ -o simula-vm simula-vm.cpp -Wall -ansi --std=c++17
Executar: ./simula-vm "Número de quadros" < "refs.txt"
*/

#include<iostream>
#include<string.h>
#include<stdio.h>
#include<cstdlib>
#include<vector>

using namespace std;

void readRefs(vector<int> &refs); /*Faz a leitura das referências e coloca no vetor passado.*/
void printVector(vector<int> &vetor); /*Imprime o vetor passado.*/
int find(vector<int> &vetor, int num); /*Verifica a existência de num no vetor passado. Caso encontre retorna a posição caso contrário retorna -1.*/
int findSmaller(vector<int> &vetor); /*Retorna o indíce da posição que possui o menor conteúdo*/ 
int findSlower(vector<int> &refs, int position, vector<int> &frame); /*Retorna o índice que contém a página que levará mais tempo para aparecer novamente*/

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
        size_frame = atoi(argv[1]); //Convertendo de char para inteiro (Através da tabela ASCII).
    }

    readRefs(refs);

    pageFaults_FIFO = FIFO(refs, frame, size_frame);
    pageFaults_LRU = LRU(refs, frame, size_frame);
    pageFaults_OPT = OPT(refs, frame, size_frame);

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

int findSmaller(vector<int> &vetor){
    int smallerIndex = 0;
    for(unsigned int i=0; i<vetor.size(); i++){
        if(vetor[i]<vetor[smallerIndex]){
            smallerIndex = i;
        }
    }
    return smallerIndex;
}

int findSlower(vector<int> &refs, int position, vector<int> &frame){
    int index = -1, index_frame = 0;

    index_frame = find(frame, -1);
    if(index_frame != -1){
        return index_frame;
    }

    for(unsigned int i = 0; i<frame.size(); i++){
        for(unsigned int j = position+1; j<refs.size(); j++){
            if(frame[i] == refs[j]){
                if(j > index || index == -1){
                    index = j;
                    index_frame = i;
                }
                j = refs.size();
            }
        }
    }
    return index_frame;

/*
    index = find(frame, -1);
    if(index != -1){
        return index;
    }

    for(unsigned int i = position+1; i < refs.size() && j < frame.size()-1; i++){
        index = find(frame, refs[i]);
        if(index != -1){
            frame[index] = -1;
            j++;
        }
    }
    j=0;
    while(frame[j] == -1)
        j++;
    return j;*/
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
    int pageFaults=0, index;
    vector<int> time;
    
    frame.assign(size_frame,-1);
    time.assign(size_frame,-1);

    for(unsigned int i = 0; i < refs.size(); i++){
        index = find(frame, refs[i]);
        if(index == -1){
            index = findSmaller(time); /*Encontrar o indice de time que tem o menor valor.*/
            frame[index] = refs[i];
            time[index] = i;
            pageFaults++;
        }
        time[index] = i;
        //printVector(frame);
    }
    return pageFaults;
}

int OPT(vector<int> &refs, vector<int> &frame, int size_frame){
 int pageFaults=0, index = 0;
     frame.assign(size_frame,-1);


    for(unsigned int i = 0; i < refs.size(); i++){
        index = find(frame, refs[i]);
        if(index == -1){
            index = findSlower(refs, i, frame);
            frame[index] = refs[i];
            pageFaults++;
        }
        //printVector(frame);
    }

    return pageFaults;
}