/*
Compilar: g++ -o simula-vm simula-vm.cpp -Wall -ansi --std=c++17
Executar: ./simula-vm "Número de quadros" < "refs.txt"
*/

#include<iostream>
#include<string.h>
#include<stdio.h>
#include<vector>

using namespace std;

void readRefs(vector<int> &refs);
void printRefs(vector<int> &refs);

int main(int argc, char *argv[]){
vector<int> refs, frame;
int size_frame = 0; 

    if(argc < 2){
        cerr << "Número de argumentos insuficiente, insira a quantidade de quadros!" << endl;
        exit(1);
    }else{
        size_frame = (int) argv[1];
        frame.assign(size_frame,-1);
    }

    readRefs(refs);


//    printRefs(refs);


    return 0;

}

void readRefs(vector<int> &refs){
int i;
    while (!cin.eof()){
        cin >> i;
        refs.push_back(i);
    }
}

void printRefs(vector<int> &refs){
    for(auto const i: refs){
        cout << i << endl;
    }
}