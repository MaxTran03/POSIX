CC=gcc -Wall -ansi
BIN=bin
INC=include
OBJ=obj
SRC=src



all: directories ${BIN}/exo1 ${BIN}/exo2 ${BIN}/exo4 ${BIN}/spy-incomplete


directories: ${OBJ} ${BIN}

${OBJ}:
	mkdir ${OBJ}
${BIN}:
	mkdir ${BIN}


${BIN}/exo1: ${OBJ}/exo1.o
	${CC} -o ${BIN}/exo1 ${OBJ}/exo1.o
	
${OBJ}/exo1.o: ${SRC}/exo1.c
	${CC} -c -o ${OBJ}/exo1.o ${SRC}/exo1.c -I${INC}
	
${BIN}/exo2: ${OBJ}/exo2.o
	${CC} -o ${BIN}/exo2 ${OBJ}/exo2.o
	
${OBJ}/exo2.o: ${SRC}/exo2.c
	${CC} -c -o ${OBJ}/exo2.o ${SRC}/exo2.c -I${INC}
	
${BIN}/exo4: ${OBJ}/exo4.o
	${CC} -o ${BIN}/exo4 ${OBJ}/exo4.o
	
${OBJ}/exo4.o: ${SRC}/exo4.c
	${CC} -c -o ${OBJ}/exo4.o ${SRC}/exo4.c -I${INC}
	
${BIN}/spy-incomplete: ${OBJ}/spy-incomplete.o
	${CC} -o ${BIN}/spy-incomplete ${OBJ}/spy-incomplete.o
	
${OBJ}/spy-incomplete.o: ${SRC}/spy-incomplete.c
	${CC} -c -o ${OBJ}/spy-incomplete.o ${SRC}/spy-incomplete.c -I${INC}
	


#run${NCODE}: ${BIN}/${NCODE}
#	echo "Exécution"
#	${BIN}/${NCODE}

clean:
	rm -f ${OBJ}/* ${BIN}/*

cleanall:
	rm -rf ${OBJ} ${BIN}
	rm -f ${INC}/*~ ${SRC}/*~ *~

