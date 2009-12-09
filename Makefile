srcdir = ./src
builddir = ./build
CFLAGS = -Wall -O2 -c

demo : ${builddir}/main.o ${builddir}/list.o ${builddir}/node.o ${builddir}/iterator.o
	gcc ${builddir}/main.o ${builddir}/list.o ${builddir}/node.o ${builddir}/iterator.o -o demo

debug : ${srcdir}/main.c ${srcdir}/list.c ${srcdir}/list.h ${srcdir}/iterator.c ${srcdir}/iterator.h ${srcdir}/node.c ${srcdir}/node.h
	gcc -g -Wall ${srcdir}/main.c ${srcdir}/list.c ${srcdir}/iterator.c ${srcdir}/node.c -o debug

clean :
	rm -rf ${builddir}/*.o debug

${builddir}/main.o : ${srcdir}/main.c
	gcc ${CFLAGS} ${srcdir}/main.c -o ${builddir}/main.o

${builddir}/list.o : ${srcdir}/list.c ${srcdir}/list.h
	gcc ${CFLAGS} ${srcdir}/list.c -o ${builddir}/list.o

${builddir}/iterator.o : ${srcdir}/iterator.c ${srcdir}/iterator.h
	gcc ${CFLAGS} ${srcdir}/iterator.c -o ${builddir}/iterator.o

${builddir}/node.o : ${srcdir}/node.c ${srcdir}/node.h
	gcc ${CFLAGS} ${srcdir}/node.c -o ${builddir}/node.o
