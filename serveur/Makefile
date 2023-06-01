all: serveur

dames.o: dames.c dames.h
	gcc -c dames.c

algorithme.o: algorithme.c algorithme.h dames.h
	gcc -c algorithme.c

reseau.o: reseau.c algorithme.h dames.h reseau.h
	gcc -c reseau.c

serveur.o: serveur.c dames.h algorithme.h reseau.h
	gcc -c serveur.c

serveur: serveur.o reseau.o algorithme.o dames.o
	gcc -o serveur serveur.o dames.o algorithme.o reseau.o

clean:
	rm -f *.o test