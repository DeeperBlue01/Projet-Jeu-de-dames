#ifndef _RES_
#define _RES_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "dames.h"
#include "algorithme.h"

#include <unistd.h> // close, read, write
#include <sys/types.h>
#include <sys/socket.h>

#include <arpa/inet.h> /* inet_pton */
#include <netinet/in.h> 

typedef struct elem_man_t{
    uint32_t m;
    struct elem_man_t *suivant;
} elem;


typedef struct {
    elem *premier;
    elem *dernier;
    int nb;
} ls_manoury;

typedef struct { //pour enregistrer la partie on va envoyer tab[256]
	int i;
	unsigned char tab[256];
}enrg;

void initialiser_enreg(enrg *e, int couleur, unsigned char ch[]);

void ajouter_coup(enrg *e, int mv);

void copier(uint8_t chaine[], uint32_t *entier, int ind);

void extraire(char chaine[], uint32_t *entier, int ind);

ssize_t exact_read(int sock, void *buffer, int nbytes);

ssize_t exact_write(int fd, void *buf, size_t nbytes);

/*Initialise une liste de Manoury*/
ls_manoury *initialiser_man();

/*Fonction qui ajoute une coordonne de manoury dans la liste*/
void ajouter_man(ls_manoury *ls, uint32_t m);

/*Fonction qui envoie un deplacement allié*/
int deplacer(int sock_echange, liste *ls, enrg *er);

/*Fonction qui envoie une prise allié*/
int prendre(int sock_echange, liste *ls, enrg *er);

/*Fonction qui recupere le coup adverse et le realise*/
int cp_adversaire(int sock_echange, damier *d, enrg *er);

#endif