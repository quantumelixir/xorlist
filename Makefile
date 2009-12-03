a.out : main.c node.c node.h list.c list.h 
	gcc -Wall main.c node.c list.c
debug : main.c node.c node.h list.c list.h 
	gcc -g main.c node.c list.c -o debug
