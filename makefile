CC=gcc

matrix.o : matrix.c
	$(CC) -c matrix.c -I ./include -o matrix.o

libmatrix.a : matrix.o
	ar -rcs libmatrix.a matrix.o