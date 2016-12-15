CC=gcc -Wall -ansi
BIN=bin
INC=include
LIB=lib
OBJ=obj
SRC=src


all: directories runstack


directories: ${OBJ} ${BIN} ${LIB}

${OBJ}:
	mkdir ${OBJ}
${BIN}:
	mkdir ${BIN}
${LIB}:
	mkdir ${LIB}


${LIB}/libficindex.a: ${OBJ}/autotestfic.o
	ar rcv ${LIB}/libficindex.a ${OBJ}/autotestfic.o
	ranlib ${LIB}/libficindex.a
	
${OBJ}/ficindex.o: ${INC}/ficindex.h ${SRC}/ficindex.c
	${CC} -c -o ${OBJ}/ficindex.o ${SRC}/ficindex.c -I${INC}	
	
${BIN}/ficindex: ${OBJ}/ficindex.o ${LIB}/libficindex.a
	${CC} -o ${BIN}/ficindex ${OBJ}/ficindex.o -L${LIB} -lficindex



#runstack: ${BIN}/stack_test
#	echo "Testing Stack"
#	${BIN}/stack_test

runstack: ${BIN}/ficindex
	echo "Testing Fifo"
	${BIN}/ficindex

clean:
	rm -f ${OBJ}/* ${BIN}/* ${LIB}/*

cleanall:
	rm -rf ${OBJ} ${BIN} ${LIB}
	rm -f ${INC}/*~ ${SRC}/*~ *~

