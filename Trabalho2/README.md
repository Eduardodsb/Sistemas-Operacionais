# Trabalho 2

Proposta: http://wiki.inf.ufpr.br/maziero/doku.php?id=so:algoritmos_de_substituicao_de_paginas

## Build
Para compilar e executar todos os exemplos digite o comando:
```
make
```
OBS: Utilize os arquivos de entrada aqui disponibilizado, pois caso o arquivo possua linhas a mais no final a saída será incorreta.

Compilar:

```
g++ -o simula-vm simula-vm.cpp -Wall -ansi --std=c++17 -Os
```

Executar:
```
./simula-vm "Número de quadros" < "refs.txt"
```
OBS: substitua "Número de quadros" pelo número de quadros desejado e "refs.txt" pelo arquivo de referência desejado.
