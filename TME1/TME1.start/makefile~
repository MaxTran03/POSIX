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


#${OBJ}/stack_array.o: ${INC}/stack.h ${SRC}/stack_array.c
#	${CC} -c -o ${OBJ}/stack_array.o ${SRC}/stack_array.c -I${INC} -D STACK_SIZE=16
	
#${OBJ}/stack_list.o: ${INC}/stack.h ${SRC}/stack_list.c
#	${CC} -c -o ${OBJ}/stack_list.o ${SRC}/stack_list.c -I${INC}
	
${OBJ}/list_impl.o: ${INC}/fifo.h ${SRC}/list_impl.c
	${CC} -c -o ${OBJ}/list_impl.o ${SRC}/list_impl.c -I${INC}

${OBJ}/fifo_list.o: ${INC}/fifo.h ${SRC}/fifo_list.c
	${CC} -c -o ${OBJ}/fifo_list.o ${SRC}/fifo_list.c -I${INC}


# Exercice 2
#
#${LIB}/libstack.a: ${OBJ}/stack_array.o
#	ar rcv ${LIB}/libstack.a ${OBJ}/stack_array.o
#	ranlib ${LIB}/libstack.a



# Exercice 5
#${LIB}/libstack.a: ${OBJ}/stack_list.o ${OBJ}/list_impl.o
#	ar rcv ${LIB}/libstack.a ${OBJ}/stack_list.o ${OBJ}/list_impl.o
#	ranlib ${LIB}/libstack.a
	
	
	
# Exercice 6
${LIB}/libfifo.a: ${OBJ}/fifo_list.o ${OBJ}/list_impl.o
	ar rcv ${LIB}/libfifo.a ${OBJ}/fifo_list.o ${OBJ}/list_impl.o
	ranlib ${LIB}/libfifo.a



#${OBJ}/stack_test.o: ${INC}/stack.h ${SRC}/stack_test.c
#	${CC} -c -o ${OBJ}/stack_test.o ${SRC}/stack_test.c -I${INC} -D STACK_SIZE=16

#${BIN}/stack_test: ${OBJ}/stack_test.o ${LIB}/libstack.a
#	${CC} -o ${BIN}/stack_test ${OBJ}/stack_test.o -L${LIB} -lstack
	
	
${OBJ}/fifo_test.o: ${INC}/fifo.h ${SRC}/fifo_test.c
	${CC} -c -o ${OBJ}/fifo_test.o ${SRC}/fifo_test.c -I${INC}	
	
${BIN}/fifo_test: ${OBJ}/fifo_test.o ${LIB}/libfifo.a
	${CC} -o ${BIN}/fifo_test ${OBJ}/fifo_test.o -L${LIB} -lfifo



#runstack: ${BIN}/stack_test
#	echo "Testing Stack"
#	${BIN}/stack_test

runstack: ${BIN}/fifo_test
	echo "Testing Fifo"
	${BIN}/fifo_test

clean:
	rm -f ${OBJ}/* ${BIN}/* ${LIB}/*

cleanall:
	rm -rf ${OBJ} ${BIN} ${LIB}
	rm -f ${INC}/*~ ${SRC}/*~ *~

