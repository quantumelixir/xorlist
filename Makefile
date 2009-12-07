demo : main.o list.o node.o
	gcc -Wall main.o list.o node.o -o demo

debug : main.c list.c list.h node.c node.h
	gcc -g -Wall main.c list.c node.c -o debug

clean :
	rm -rf *.o debug demo

main.o : main.c
	gcc -Wall -c main.c

list.o : list.c list.h
	gcc -Wall -c list.c

node.o : node.c node.h
	gcc -Wall -c node.c
