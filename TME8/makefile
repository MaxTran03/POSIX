CC=gcc -Wall -ansi
BIN=bin
INC=include
LIB=lib
OBJ=obj
SRC=src


all: directories ${BIN}/single_converters  ${BIN}/conversion_server ${BIN}/conversion_client ${BIN}/multi_converter

directories: ${OBJ} ${BIN}

${OBJ}:
	mkdir ${OBJ}
${BIN}:
	mkdir ${BIN}

## Exercice 1 ##
${OBJ}/converters.o: ${INC}/converters.h ${SRC}/converters.c
	${CC} -c -o ${OBJ}/converters.o ${SRC}/converters.c -I${INC}
	
${BIN}/single_converters: ${OBJ}/converters.o
	${CC} -o ${BIN}/single_converters ${SRC}/converters.c
	
single_converters: ${BIN}/single_converters

## Exercice 2 ##
${OBJ}/conversion_server.o: ${SRC}/conversion_server.c
	${CC} -c -o ${OBJ}/conversion_server.o ${SRC}/conversion_server.c

${BIN}/conversion_server: ${OBJ}/conversion_server.o
	${CC} -o ${BIN}/conversion_server ${SRC}/conversion_server.c
	
${OBJ}/conversion_client.o: ${SRC}/conversion_client.c
	${CC} -c -o ${OBJ}/conversion_client.o ${SRC}/conversion_client.c

${BIN}/conversion_client: ${OBJ}/conversion_client.o
	${CC} -o ${BIN}/conversion_client ${SRC}/conversion_client.c

conversion_server: ${BIN}/conversion_server

conversion_client: ${BIN}/conversion_client

## Exercice 3 ##

${OBJ}/multi_converter.o: ${SRC}/multi_converter.c
	${CC} -c -o ${OBJ}/multi_converter.o ${SRC}/multi_converter.c

${BIN}/multi_converter: ${OBJ}/multi_converter.o
	${CC} -o ${BIN}/multi_converter ${SRC}/multi_converter.c

multi_converter : ${BIN}/multi_converter


#runstack: ${BIN}/single_converters
#	echo "Testing single_converters"
#	${BIN}/single_converters

clean:
	rm -f ${OBJ}/* ${BIN}/*

cleanall:
	rm -rf ${OBJ} ${BIN}
	rm -f ${INC}/*~ ${SRC}/*~ *~

