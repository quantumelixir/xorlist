srcdir = ../src
builddir = .
CFLAGS = -Wall -O2 -c

demo : main.o list.o node.o iterator.o
	gcc main.o list.o node.o iterator.o -o demo

debug : ${srcdir}/main.c ${srcdir}/list.c ${srcdir}/list.h ${srcdir}/iterator.c ${srcdir}/iterator.h ${srcdir}/node.c ${srcdir}/node.h
	gcc -g -Wall ${srcdir}/main.c ${srcdir}/list.c ${srcdir}/iterator.c ${srcdir}/node.c -o debug

clean :
	rm -rf *.o debug

main.o : ${srcdir}/main.c
	gcc ${CFLAGS} ${srcdir}/main.c

list.o : ${srcdir}/list.c ${srcdir}/list.h
	gcc ${CFLAGS} ${srcdir}/list.c

iterator.o : ${srcdir}/iterator.c ${srcdir}/iterator.h
	gcc ${CFLAGS} ${srcdir}/iterator.c

node.o : ${srcdir}/node.c ${srcdir}/node.h
	gcc ${CFLAGS} ${srcdir}/node.c
