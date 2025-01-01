FLAGS = -g3 -Wall -ansi -pedantic

all: complex.o mycomp.out

complex.o: complex.c complex.h
	cc -c $(FLAGS) complex.c -ocomplex.o 

mycomp.out: mycomp.c complex.o
	cc $(FLAGS) mycomp.c complex.o -omycomp.out

clean:
	rm complex.o mycomp.out