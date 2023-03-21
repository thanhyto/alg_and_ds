CC=g++
DEV=-Wall -g -std=c++14
OPT=-O3 -std=c++14

.PHONY: all
all: AVLCommands.exe CreateData

CreateData: CreateData.cxx json.hpp
	$(CC) $(OPT) CreateData.cxx -o CreateData.exe

BST.o: BST.cpp BST.h
	$(CC) $(DEV) -c BST.cpp

AVL.o: AVL.cpp AVL.h
	$(CC) $(FLAGS) -c AVL.cpp

AVLCommands.exe: AVLCommands.cxx AVL.o $(JSON)
	$(CC) $(FLAGS) AVLCommands.cxx AVL.o -o AVLCommands.exe

# Build
.PHONY: clean
clean:
	rm -f *.o
	rm -f *.exe
	rm -rf *dSYM

.PHONY: update
update:
	make clean
	make all
