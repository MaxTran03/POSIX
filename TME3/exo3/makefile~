CC=gcc
BIN=bin
INC=include
LIB=lib
OBJ=obj
SRC=src
CFLAGS=-c -Wall -I.
LDFLAGS=
SOURCES=affiche1.c affiche2.c affiche3.c affiche4.c affiche-main.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=/bin/affiche

all: ${SRC}/$(SOURCES) ${BIN}/$(EXECUTABLE)

directories: ${OBJ} ${BIN} ${LIB}

${OBJ}:
	mkdir ${OBJ}
${BIN}:
	mkdir ${BIN}
${LIB}:
	mkdir ${LIB}
	
	

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) ${OBJ}/$(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o *~
