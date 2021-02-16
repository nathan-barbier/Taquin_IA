CC = gcc
CCOPTS = -Wall

all: taquin

list.o: list.c list.h
	${CC} ${CCOPTS} -c list.c

board.o: board.c board.h
	${CC} ${CCOPTS} -c board.c

taquin.o: taquin.c
	${CC} ${CCOPTS} -c taquin.c -lm

taquin: taquin.o board.o list.o board.h list.h item.h
	${CC} ${CCOPTS} -o taquin taquin.o board.o list.o -lm
	./taquin


clean:
	rm *.o taquin
