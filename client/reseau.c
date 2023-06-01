#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "dames.h"
#include "algorithme.h"
#include "reseau.h"

#include <unistd.h> // close, read, write
#include <sys/types.h>
#include <sys/socket.h>

#include <arpa/inet.h> /* inet_pton */
#include <netinet/in.h> 

void copier(uint8_t chaine[], uint32_t *entier, int ind) {
	for(int i = 0; i < 4; i++) {
		*(chaine + ind + i) =  *((uint8_t *) entier + i);
	}
}
void extraire(char chaine[], uint32_t *entier, int ind) {
	for(int i = 0; i < 4; i++) {
		 *((uint8_t *)entier + i) = *(chaine + ind + i); 
	}
}


ssize_t exact_read(int sock, void *buffer, int nbytes) {
	ssize_t n;
	size_t total = 0;
	while (total < nbytes) {
	n = read(sock, (char *) buffer + total, nbytes - total);
	if (n == -1) /* error */
	return n;
	if (n == 0) /* end of file */
	break;
	total += n;
	}
	return total;
}

ssize_t exact_write(int fd, void *buf, size_t nbytes)
{
	ssize_t n;
	size_t total = 0;
	while (total < nbytes) {
		n = write(fd, (char *) buf + total, nbytes - total);
		if (n == -1) /* error */
		return n;
		total += n;
	}
	return total;
}

ls_manoury *initialiser_man() { //initialise la liste chainée
    ls_manoury *ls = malloc(sizeof(ls_manoury));
    ls->premier = NULL;
    ls->dernier = NULL;
    ls->nb = 0;
    return ls;
}

void ajouter_man(ls_manoury *ls, uint32_t m) { //ajoute l'elem dans la liste
    elem *e = malloc(sizeof(elem));
    e->m = m;
    e->suivant = NULL;
    if(ls->nb == 0) {
        ls->premier = e;
        ls->dernier = e;  
    }
    else {
        ls->dernier->suivant = e;
        ls->dernier = e;
    }
     ls->nb++;
}
int deplacer(int sock_echange, liste *ls, enrg *er) { //liste contient tous nos deplacement
    element *e = ls->premier;
    cpl *c = malloc(sizeof(cpl));
    c->i = e->i;
    c->j = e->j;
    uint32_t man1 = coord_to_man(c);
    printf("\n\n\n(fct deplacer) man1 = %u\n\n\n", man1);
    e = e->suivant;
    c->i = e->i;
    c->j = e->j;
    uint32_t man2 = coord_to_man(c);
    printf("\n\n\n(fct deplacer) man2 = %u\n\n\n", man2);
    int taille = 4; // au moins 2 chiffres  + '-' + '\0' ex "8-9\0"
    if(man1 >= 10) //man1 est sur deux chiffres
        taille++;
    if(man2 >= 10) //man2 est sur deux chiffres
        taille ++;
    char chaine[taille];
    int i = 0;
    char ch[2];
    if(man1 >= 10) {
        sprintf(ch,"%d",(man1/10));
        chaine[i] = ch[0];
        i++;
        sprintf(ch,"%d",(man1%10));
        chaine[i] = ch[0];
        i++;
    }
    else {
        sprintf(ch,"%d",(man1));
        chaine[i] = ch[0];
        i++;
    }
    chaine[i] = '-';
    i++;
    if(man2 >= 10) {
        sprintf(ch,"%d",(man2/10));
        chaine[i] = ch[0];
        i++;
        sprintf(ch,"%d",(man2%10));
        chaine[i] = ch[0];
        i++;
    }
    else {
        sprintf(ch,"%d",(man2));
        chaine[i] = ch[0];
        i++;
    }
    chaine[i] = '\0';

    exact_write(sock_echange, chaine, taille);
    ajouter_coup(er, man1 + 128);
    ajouter_coup(er, man2 + 128);
    return 1;
}

int prendre(int sock_echange, liste *ls, enrg *er) {
    element *e = ls->premier;
    cpl *c = malloc(sizeof(cpl));
    int taille = 2*(ls->nb);
    for(int i = 0; i < ls->nb; i++ ) {
        c->i = e->i;
        c->j = e->j;
        printf("\n\n\n(fct deplacer) j'envoie rafle = %d , %d\n\n\n", c->i, c->j);      
        int m = coord_to_man(c);
        if(i == ls->nb - 1)
            ajouter_coup(er, m + 128);
        else
            ajouter_coup(er, m);
        if(m >= 10)
            taille++;
         printf("\n\n\n(fct deplacer) j'envoie rafle = %u\n\n\n", coord_to_man(c));
        e = e->suivant;
    }
    char ch[2];
    char chaine[taille];
    e = ls->premier;
    int i = 0;
    cpl *x = malloc(sizeof(cpl));
    while(e != NULL) {
        x->i = e->i;
        x->j = e->j;    
        int entier = coord_to_man(x);
        if(entier >= 10) {
            sprintf(ch,"%d",(entier/10));
            chaine[i] = ch[0];
            i++;
            sprintf(ch,"%d",(entier%10));
            chaine[i] = ch[0];
            i++;
        }
        else {
            sprintf(ch,"%d",(entier));
            chaine[i] = ch[0];
            i++;
        }
        chaine[i] = 'x';
        i++;
        e = e->suivant;
    }

    chaine[i-1] = '\0';
    exact_write(sock_echange, chaine, taille);
    return 1;
}

int cp_adversaire(int sock_echange, damier *d, enrg *er) {
    char recevoir[256];
    char poubelle[256];
    int i = 0;
    int j = 0;
    do {
        exact_read(sock_echange, poubelle+j, 1);
	printf("b %c ",poubelle[j]);
        j++;
    }while((int)poubelle[j-1] == 0);
	putchar('\n');
	recevoir[i]=poubelle[j-1];
	i++;
    do {
        exact_read(sock_echange, recevoir + i, 1);
	printf("b %c ",recevoir[i]);
        i++;
    }while((int)recevoir[i-1] != 0);
	putchar('\n');
    int taille = i;
    for(int j = 0; j < taille; j++) {
        printf("%c ",recevoir[j]);
    }
    putchar('\n');
    if(strchr(recevoir, '-') != NULL) {
        int m = 0;
        int n = 1;
        int tab[2];
        if(recevoir[n] == '-') {
            tab[0] = recevoir[m] - 48;
            m = n + 1;
            n = n + 2;
            if(n >= taille - 1) {
                tab[1] = recevoir[m] - 48;
            }
            else {
                tab[1] = (recevoir[m] - 48) * 10 + (recevoir[n] - 48);
            }
        }
        else {
            tab[0] = (recevoir[m] - 48) * 10 + (recevoir[n] - 48);
            m = n + 2;
            n = n + 3;
            if(n >= taille - 1) {
                tab[1] = (recevoir[m] - 48);
            }
            else {
                tab[1] = (recevoir[m] - 48) * 10 + (recevoir[n] - 48);
            }
        }
        printf("\n\nj'ai recu deplacement adverse %d-%d\n\n",tab[0],tab[1]);
        int rep = algorithme_defense(d, tab, 2, 0);
        if(rep == 0) {
                printf("coup adverse invalide \n");
                return 0;
            }
        ajouter_coup(er, tab[0] + 128);
        ajouter_coup(er, tab[1] + 128);
    }
    else 
        printf("- non present\n");
    if(strchr(recevoir, 'x') != NULL) {
        int acc = 0;
        int par = 0;
        while((int)recevoir[par] != 0) {
            if(recevoir[par] == 'x')
                acc++;
            par++;
        }
        int nb = acc + 1;
        ls_manoury *lsr = initialiser_man();
        int i = 0;
        int j = 1;
        int trouve = 0;
        while(trouve < nb) {
            if(recevoir[j] == 'x') {
                ajouter_man(lsr, (recevoir[i] - 48));
                i = j+1;
                j = j + 2;
                trouve++;
            }
            else {
                ajouter_man(lsr, (recevoir[i] - 48) * 10 + (recevoir[j] - 48));
                i = j + 2;
                j = j + 3;
                trouve++;
            }
        }
        int tab_deplacement[lsr->nb];
        elem *e = lsr->premier;
        for(int h = 0; h < lsr->nb; h++) {
            tab_deplacement[h] = e->m;
            e = e->suivant;
        }
        int rep = algorithme_defense(d, tab_deplacement, lsr->nb, 1);
        if(rep == 0) {
            printf("prise adverse invalide \n");
            return 0;
        }
        for(int r = 0; r < lsr->nb; r++) {
            if(r == lsr->nb -1)
                ajouter_coup(er, tab_deplacement[r] + 128);
            else
                ajouter_coup(er, tab_deplacement[r]);
        }
    }
    else
        printf("x non present\n");
    return 1;
}

void initialiser_enreg(enrg *e, int couleur, unsigned char ch[]) {
    if(couleur == 1) {
        e->tab[0] = 255;
    }
    else {
        e->tab[0] = 1;
    }
    for(int j = 0; j < 16; j++) {
        e->tab[j + 1] = ch[j];
        printf("ch[j] = %u ",ch[j]);
    }
    e->i = 17;
}
void ajouter_coup(enrg *e, int mv) {
    //printf("mv avant = %d, ",mv);
    e->tab[e->i] = (uint8_t)mv;
    //printf("mv apres = %u\n",e->tab[e->i]);
    e->i++;
}