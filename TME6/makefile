CC=gcc -Wall -ansi
BIN=bin
INC=include
OBJ=obj
SRC=src



all: directories ${BIN}/liste-rep ${BIN}/setfilestats ${BIN}/filefusion ${BIN}/findword ${BIN}/redirection


directories: ${OBJ} ${BIN}

${OBJ}:
	mkdir ${OBJ}
${BIN}:
	mkdir ${BIN}


## Exercice 1 ##
${BIN}/liste-rep: ${OBJ}/liste-rep.o
	${CC} -o ${BIN}/liste-rep ${OBJ}/liste-rep.o
	
${OBJ}/liste-rep.o: ${SRC}/liste-rep.c
	${CC} -c -o ${OBJ}/liste-rep.o ${SRC}/liste-rep.c -I${INC}
	


## Exercice 2 ##
${BIN}/setfilestats: ${OBJ}/setfilestats.o
	${CC} -o ${BIN}/setfilestats ${OBJ}/setfilestats.o
	
${OBJ}/setfilestats.o: ${SRC}/setfilestats.c
	${CC} -c -o ${OBJ}/setfilestats.o ${SRC}/setfilestats.c -I${INC}


## Exercice 3 ##
${BIN}/filefusion: ${OBJ}/filefusion.o
	${CC} -o ${BIN}/filefusion ${OBJ}/filefusion.o
	
${OBJ}/filefusion.o: ${SRC}/filefusion.c
	${CC} -c -o ${OBJ}/filefusion.o ${SRC}/filefusion.c -I${INC}


## Exercice 4 ##
${BIN}/findword: ${OBJ}/findword.o
	${CC} -o ${BIN}/findword ${OBJ}/findword.o
	
${OBJ}/findword.o: ${SRC}/findword.c
	${CC} -c -o ${OBJ}/findword.o ${SRC}/findword.c -I${INC}


## Exercice 5 ##
${BIN}/redirection: ${OBJ}/redirection.o
	${CC} -o ${BIN}/redirection ${OBJ}/redirection.o
	
${OBJ}/redirection.o: ${SRC}/redirection.c
	${CC} -c -o ${OBJ}/redirection.o ${SRC}/redirection.c -I${INC}

	


#run${NCODE}: ${BIN}/${NCODE}
#	echo "Exécution"
#	${BIN}/${NCODE}

clean:
	rm -f ${OBJ}/* ${BIN}/*

cleanall:
	rm -rf ${OBJ} ${BIN}
	rm -f ${INC}/*~ ${SRC}/*~ *~

