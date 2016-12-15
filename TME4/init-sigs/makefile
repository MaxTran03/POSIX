CC=gcc -Wall -ansi


all: init-sig.1 init-sig.2 init-sig.3 init-sig.4 init-sig.5


init-sig.1: init-sig.1.o
	${CC} -o init-sig.1 init-sig.1.o
	
init-sig.1.o: init-sig.1.c
	${CC} -c -o init-sig.1.o init-sig.1.c	

init-sig.2: init-sig.2.o
	${CC} -o init-sig.2 init-sig.2.o
	
init-sig.2.o: init-sig.2.c
	${CC} -c -o init-sig.2.o init-sig.2.c
	
init-sig.3: init-sig.3.o
	${CC} -o init-sig.3 init-sig.3.o
	
init-sig.3.o: init-sig.3.c
	${CC} -c -o init-sig.3.o init-sig.3.c
	
init-sig.4: init-sig.4.o
	${CC} -o init-sig.4 init-sig.4.o
	
init-sig.4.o: init-sig.4.c
	${CC} -c -o init-sig.4.o init-sig.4.c
	
init-sig.5: init-sig.5.o
	${CC} -o init-sig.5 init-sig.5.o
	
init-sig.5.o: init-sig.5.c
	${CC} -c -o init-sig.5.o init-sig.5.c


clean:
	rm -f *.o *~ 

