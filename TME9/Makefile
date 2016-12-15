CC=gcc -Wall -ansi
BIN=bin
INC=include
LIB=lib
OBJ=obj
SRC=src


all: directories capitalize test conversion_client multi_converter 

directories: ${OBJ} ${BIN}

${OBJ}:
	mkdir ${OBJ}
${BIN}:
	mkdir ${BIN}

## Exercice 1 ##
${OBJ}/capitalize.o: ${SRC}/capitalize.c
	${CC} -c -o ${OBJ}/capitalize.o ${SRC}/capitalize.c -lrt -lpthread
	
${BIN}/capitalize: ${OBJ}/capitalize.o
	${CC} -o ${BIN}/capitalize ${SRC}/capitalize.c -lrt -lpthread
	
capitalize: ${BIN}/capitalize

${OBJ}/test.o: ${SRC}/test.c
	${CC} -c -o ${OBJ}/test.o ${SRC}/test.c -lrt -lpthread
	
${BIN}/test: ${OBJ}/test.o
	${CC} -o ${BIN}/test ${SRC}/test.c -lrt -lpthread
	
test: ${BIN}/test

## Exercice 2 ##

${OBJ}/converters.o: ${SRC}/converters.c ${INC}/converters.h
	${CC} -o ${OBJ}/converters.o -c ${SRC}/converters.c -I${INC} ${CFLAGS} -lrt -lpthread
	
${OBJ}/conversion_client.o: ${SRC}/conversion_client.c ${INC}/converters.h
	${CC} -c -o ${OBJ}/conversion_client.o ${SRC}/conversion_client.c -I${INC} -lrt -lpthread

${BIN}/conversion_client: ${OBJ}/conversion_client.o
	${CC} -o ${BIN}/conversion_client ${OBJ}/conversion_client.o -lrt -lpthread
	
conversion_client: ${BIN}/conversion_client

${OBJ}/multi_converter.o: ${SRC}/multi_converter.c ${INC}/converters.h
	${CC} -c -o ${OBJ}/multi_converter.o ${SRC}/multi_converter.c -I${INC} -lrt -lpthread

${BIN}/multi_converter: ${OBJ}/multi_converter.o
	${CC} -o ${BIN}/multi_converter ${OBJ}/multi_converter.o -lrt -lpthread

multi_converter : ${BIN}/multi_converter


#runstack: ${BIN}/single_converters
#	echo "Testing single_converters"
#	${BIN}/single_converters

#runExo2: ${OBJ}/converters.o ${BIN}/conversion_client ${BIN}/multi_converter
#	echo "Ouverture d'un nouveau terminal exécutant: ./bin/conversion_client segEchanges JPY 100.0"
#	gnome-terminal --title=356Exo2 -e "./bin/conversion_client segEchanges JPY 100.0"  &
#	${BIN}/multi_converter segEchanges

clean:
	rm -f ${OBJ}/* ${BIN}/*
	rm -f /dev/shm/*

cleanall:
	rm -rf ${OBJ} ${BIN}
	rm -f ${INC}/*~ ${SRC}/*~ *~
	rm -f /dev/shm/*
