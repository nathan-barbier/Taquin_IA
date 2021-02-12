all: taquin

list.o: list.c list.h
	gcc -c list.c

board.o: board.c board.h
	gcc -c board.c

taquin.o: taquin.c
	gcc -c taquin.c -lm

taquin: taquin.o board.o list.o board.h list.h item.h
	gcc -o taquin taquin.o board.o list.o -lm
	./taquin


clean:
	rm *.o taquin
