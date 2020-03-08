a.out:  main.o functions.o
    gcc main.o functions.o
functions.o:    functions.c headers.h
    gcc -c functions.c headers.h
main.o: main.c headers.h
    gcc -c main.c headers.h
