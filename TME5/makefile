CC=gcc -Wall -ansi
BIN=bin
INC=include
OBJ=obj
SRC=src



all: directories ${BIN}/mywait ${BIN}/mysleep ${BIN}/synchro3 ${BIN}/synchro3_v2 ${BIN}/tempo


directories: ${OBJ} ${BIN}

${OBJ}:
	mkdir ${OBJ}
${BIN}:
	mkdir ${BIN}


## Exercice 1 ##
${BIN}/mywait: ${OBJ}/mywait.o
	${CC} -o ${BIN}/mywait ${OBJ}/mywait.o
	
${OBJ}/mywait.o: ${SRC}/mywait.c
	${CC} -c -o ${OBJ}/mywait.o ${SRC}/mywait.c -I${INC}
	


## Exercice 2 ##
${BIN}/mysleep: ${OBJ}/mysleep.o
	${CC} -o ${BIN}/mysleep ${OBJ}/mysleep.o
	
${OBJ}/mysleep.o: ${SRC}/mysleep.c
	${CC} -c -o ${OBJ}/mysleep.o ${SRC}/mysleep.c -I${INC}


## Exercice 3 ##
${BIN}/synchro3: ${OBJ}/synchro3.o
	${CC} -o ${BIN}/synchro3 ${OBJ}/synchro3.o
	
${OBJ}/synchro3.o: ${SRC}/synchro3.c
	${CC} -c -o ${OBJ}/synchro3.o ${SRC}/synchro3.c -I${INC}


${BIN}/synchro3_v2: ${OBJ}/synchro3_v2.o
	${CC} -o ${BIN}/synchro3_v2 ${OBJ}/synchro3_v2.o
	
${OBJ}/synchro3_v2.o: ${SRC}/synchro3.c
	${CC} -c -o ${OBJ}/synchro3_v2.o ${SRC}/synchro3_v2.c -I${INC}


## Exercice 4 ##
${BIN}/tempo: ${OBJ}/tempo.o
	${CC} -o ${BIN}/tempo ${OBJ}/tempo.o
	
${OBJ}/tempo.o: ${SRC}/tempo.c
	${CC} -c -o ${OBJ}/tempo.o ${SRC}/tempo.c -I${INC}

	


#run${NCODE}: ${BIN}/${NCODE}
#	echo "Exécution"
#	${BIN}/${NCODE}

clean:
	rm -f ${OBJ}/* ${BIN}/*

cleanall:
	rm -rf ${OBJ} ${BIN}
	rm -f ${INC}/*~ ${SRC}/*~ *~

