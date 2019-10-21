/*
Compilar: g++ -o simula-vm simula-vm.cpp -Wall -ansi --std=c++17 -Os
Executar: ./simula-vm "Número de quadros" < "refs.txt"
*/

#include<iostream>
#include<string.h>
#include<stdio.h>
#include<cstdlib>
#include<vector>
#include <algorithm>
#include <iterator>

using namespace std;

void readRefs(vector<int> &refs); /*Faz a leitura das referências e coloca no vetor refs.*/
int find(vector<int> &vetor, int num); /*Verifica a existência de num no vetor passado(vetor). Caso encontre retorna a posição, caso contrário retorna -1.*/
int findSmaller(vector<int> &vetor); /*Retorna o indíce da posição que possui o menor conteúdo. Utilizado para encontrar a página com menor tempo de entrada (mais velha).*/ 
int findSlower(vector<int> &refs, int position, vector<int> frame); /*Retorna o índice que contém a página que levará mais tempo para aparecer novamente*/

/*Algoritmos*/
int FIFO(vector<int> &refs, vector<int> &frame, int size_frame); /*Simula o algortimo FIFO e retorna a quantidade de falhas de páginas.*/
int LRU(vector<int> &refs, vector<int> &frame, int size_frame); /*Simula o algortimo LRU e retorna a quantidade de falhas de páginas.*/
int OPT(vector<int> &refs, vector<int> &frame, int size_frame); /*Simula o algortimo OPT e retorna a quantidade de falhas de páginas.*/

int main(int argc, char *argv[]){
vector<int> refs, frame;
int size_frame = 0, pageFaults_FIFO = 0, pageFaults_LRU = 0, pageFaults_OPT = 0; 

    if(argc < 2){ /*Verifica se foi passada a quantidade de argumentos correta.*/
        cerr << "Número de argumentos insuficiente, insira a quantidade de quadros!" << endl;
        exit(1);
    }else{
        size_frame = atoi(argv[1]); //Convertendo de char para inteiro.
    }

    readRefs(refs); //Lê as referências de entrada. 

    pageFaults_FIFO = FIFO(refs, frame, size_frame); //Executa o algoritmo FIFO
    pageFaults_LRU = LRU(refs, frame, size_frame); //Executa o algoritmo LRU
    pageFaults_OPT = OPT(refs, frame, size_frame); //Executa o algoritmo OPT

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

int find(vector<int> &vetor, int num){
    vector<int>::iterator it;
    it = find(vetor.begin(), vetor.end(), num);
    if(it == vetor.end()){
        return -1;
    }
    return it - vetor.begin();
    
}

int findSmaller(vector<int> &vetor){
    int smallerIndex = 0;
    for(unsigned int i=0; i<vetor.size(); i++){ //Compara dois a dois pegando sempre o menor índice. 
        if(vetor[i]<vetor[smallerIndex]){
            smallerIndex = i;
        }
    }
    return smallerIndex;
}

int findSlower(vector<int> &refs, int position, vector<int> frame){
    int index = 0;
    unsigned aux = 0;
    vector<int>::iterator it;

    index = find(frame, -1); //Verifica se existe posição vaga, para a página.
    if(index != -1){
        return index;
    }

    for(unsigned int i = 0; i<frame.size(); i++){ //Para cada página pertencente ao quadro(frame), é buscada sua próxima requisição nas referências.
        it = find(refs.begin()+position+1, refs.end(), frame[i]);
        if(it == refs.end()){ //Caso não seja encontrada, o índice da mesma é retornado, pois ela não será mais requisitada.
            index = i;
            i = frame.size();
        }else{ //Caso não seja encontrada, é guardado o tempo em que a mesma irá ser requisitada e tal tempo é comparado com os das demais páginas do frame. Assim ficando com o índice da página com maior tempo.
            if(it - refs.begin() > aux){
                index = i;
                aux = it - refs.begin();    
            }
        } 
    }
    return index;
}

int FIFO(vector<int> &refs, vector<int> &frame, int size_frame){
    int pageFaults=0;
    
    frame.assign(size_frame,-1);

    for(unsigned int i = 0; i < refs.size(); i++){ //Para cada página, pertecente a referência, é analisada a situação do frame (quadro). 
        if(find(frame, refs[i]) == -1){ //Verifica se a página atual já está no quadro (frame). Caso não esteja é inserida.
            frame[pageFaults%size_frame] = refs[i]; //É utilizada a ideia de módulo para inserir as páginas. OBS: A variável pageFaults é utilizada para duas ideias distintas: 1 - Falhas de página e 2 - Como indice dá próxima possição que deve ser inserida a nova página. 
            pageFaults++;
        }
    }
    return pageFaults;
}

int LRU(vector<int> &refs, vector<int> &frame, int size_frame){
    int pageFaults=0, index;
    vector<int> time; //time é utlizada para guardar o tempo de entrada de cada página. Obs: As páginas e seus tempos são relacionados pelo índice. 
    
    frame.assign(size_frame,-1);
    time.assign(size_frame,-1);

    for(unsigned int i = 0; i < refs.size(); i++){ //Para cada página, pertecente a referência, é analisada a situação do frame (quadro). 
        index = find(frame, refs[i]);
        if(index == -1){ //Caso a página não exista no quadro (frame), é necessário inserir.
            index = findSmaller(time); //Encontra o índice da página que tem o menor tempo de entrada, ou seja, a mais velha.
            frame[index] = refs[i];
            time[index] = i;
            pageFaults++;
        }
        time[index] = i;
    }
    return pageFaults;
}

int OPT(vector<int> &refs, vector<int> &frame, int size_frame){
    int pageFaults=0, index=0;
    frame.assign(size_frame,-1);

    for(unsigned int i = 0; i < refs.size(); i++){ //Para cada página, pertecente a referência, é analisada a situação do frame (quadro). 
        index = find(frame, refs[i]);
        if(index == -1){ //Caso a página não exista no quadro (frame), é necessário inserir.
            index = findSlower(refs, i, frame); //Encontra o índice da página que levará mais tempo para ser referênciada novamente.
            frame[index] = refs[i];
            pageFaults++;
        }    
    }

    return pageFaults;
}