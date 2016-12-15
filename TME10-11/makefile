CC= gcc
CFLAGS= -Wall
LDFLAGS= -L $(LIB) -lrt -lmyqueue -lpthread
INCFLAGS= -I$(INC)
INC=include
LIB=lib
SRC=src
OBJ=obj
BIN=bin

all: directories test1_myqueue test2_myqueue

directories: $(LIB) $(OBJ) $(BIN)

$(LIB):
	mkdir -p $(LIB)
$(OBJ):
	mkdir -p $(OBJ)
$(BIN):
	mkdir -p $(BIN)


$(OBJ)/myqueue.o: $(SRC)/myqueue.c
	$(CC) -c $< -o $@ $(CFLAGS) $(INCFLAGS)
$(OBJ)/test1_myqueue.o: $(SRC)/test1_myqueue.c
	$(CC) -c $< -o $@ $(CFLAGS) $(INCFLAGS)
$(OBJ)/test2_myqueue.o: $(SRC)/test2_myqueue.c
	$(CC) -c $< -o $@ $(CFLAGS) $(INCFLAGS)


$(LIB)/libmyqueue.a: $(OBJ)/myqueue.o
	ar -rs $@ $^
	

test1_myqueue: $(OBJ)/test1_myqueue.o $(LIB)/libmyqueue.a
	$(CC) -o $(BIN)/$@ $^ $(LDFLAGS)
test2_myqueue: $(OBJ)/test2_myqueue.o $(LIB)/libmyqueue.a
	$(CC) -o $(BIN)/$@ $^ $(LDFLAGS)

clean:
	rm -f ${OBJ}/* ${BIN}/* ${LIB}/*

cleanall:
	rm -rf ${OBJ} ${BIN} ${LIB}
	rm -f ${INC}/*~ ${SRC}/*~ ${LIB}/*~ *~

