all: client

dames.o: dames.c dames.h
	gcc -c dames.c

algorithme.o: algorithme.c algorithme.h dames.h
	gcc -c algorithme.c

reseau.o: reseau.c algorithme.h dames.h reseau.h
	gcc -c reseau.c

client.o: client.c dames.h algorithme.h reseau.h
	gcc -c client.c

client: client.o reseau.o algorithme.o dames.o
	gcc -o client client.o dames.o algorithme.o reseau.o

clean:
	rm -f *.o client