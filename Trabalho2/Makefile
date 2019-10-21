all:
	g++ -o simula-vm simula-vm.cpp -Wall -ansi --std=c++17 -Os
	./simula-vm 4 < vsim-exemplo.txt
	./simula-vm 4 < vsim-belady.txt 
	./simula-vm 64 < vsim-gcc.txt 
	./simula-vm 256 < vsim-gcc.txt 
	./simula-vm 1024 < vsim-gcc.txt
	./simula-vm 4096 < vsim-gcc.txt  
