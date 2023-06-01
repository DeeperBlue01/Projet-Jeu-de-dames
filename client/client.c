#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // close, read, write
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdint.h>
#include "reseau.h"
#include "dames.h"
#include "algorithme.h"

#define PORT_DEFI 7777

char invalide[] = "COUP INVALIDE";
char perdu[] = "PERDU";
char egalite[] = "EGALITE";
char inter[] = "INTERUPTION";

void usage(char *nom_prog)
{
	fprintf(stderr, "Usage: %s addr_ipv6\n"
			"Client pour CRP (Checker Robot Protocol)\n"
			"Exemple: %s 2001:910:1410:523:0:fada:80af:2bc2\n", nom_prog, nom_prog);
}


int main(int argc, char *argv[])
{
	enrg *e = malloc(sizeof(enrg));

    if (argc != 2)
    {
		usage(argv[0]);
	    return 1;
	}

    /*1*/
    int tcp6_sock;
    if ((tcp6_sock = socket(AF_INET6, SOCK_STREAM, 0)) < 0) {
		perror("Error Socket() Client");
		exit(2);
	}
   /*2*/
    struct sockaddr_in6 addr = { .sin6_family = AF_INET6, .sin6_port = htons(PORT_DEFI) };

    if(inet_pton(AF_INET6, argv[1], &addr.sin6_addr) != 1)
    {
        fprintf(stderr, "Adresse IPv6 invalide\n");
		exit(1);
    }
    /*3*/
    if ( connect(tcp6_sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
       	perror("Error Connect()");
		exit(3);
    }
    int clt = 0;
    int srv = 0;
    initialiser_enreg(e, 0, addr.sin6_addr.s6_addr);
	damier *d = malloc(sizeof(damier));
	initialiser_damier(d, 0, 1);
	liste *ls;
	int v;
	int termine = 0;
	while(!termine && ((srv < 50) || (clt < 50))) {
		ls = initialiser_liste_vide();
		if(!cp_adversaire(tcp6_sock, d, e)) {
			termine = 1;
			break;
		}

		else {
			if(d->pions_noirs == 0)
				break;
		}
		srv++;
		v = algorithme(d, ls);
		if(!v) { // deplacement
			deplacer(tcp6_sock, ls, e);
		}
		else { //prise
			prendre(tcp6_sock, ls, e);
			if(d->pions_blancs == 0) {
				termine = 1;
				break;
			}
		}
		clt++;
		if(d->pions_blancs == 0 || d->pions_noirs == 0) {
				termine = 1;
		}
		free(ls);
	}

    close(tcp6_sock);
    printf("partie termine ! \nnombre de coups client : %d\nnombre de coups serveur : %d\n\n",clt,srv);
	printf("nombre de pions client : %d\nnombre de pions serveur : %d\n\n",d->pions_noirs,d->pions_blancs);
	printf("\n\n\n\n");
	for(int i = 0; i < e->i; i++) {
		printf("%u ",e->tab[i]);
	}
	while(e->i < 256) {
		ajouter_coup(e, 0);
	}


	int sock_env;
    if ((sock_env = socket(AF_INET6, SOCK_STREAM, 0)) < 0) {
		perror("Error Socket() Client");
		exit(2);
	}
   
    struct sockaddr_in6 addr_r = { .sin6_family = AF_INET6, .sin6_port = htons(5777) };
    char adresse_ip[] = "2001:910:1410:523:0:fada:80af:2bc2";
    if(inet_pton(AF_INET6, adresse_ip, &addr_r.sin6_addr) != 1)
    {
        fprintf(stderr, "Adresse IPv6 invalide\n");
		exit(1);
    }
    int opta = 1;
	setsockopt(sock_env, SOL_SOCKET, SO_REUSEADDR, &opta, sizeof(int));
    
    
    if ( connect(sock_env, (struct sockaddr *)&addr_r, sizeof(addr_r)) < 0)
    {
       	perror("Error Connect()");
		exit(3);
    }
    printf("je vais env\n");
    exact_write(sock_env, e->tab, 1);

    char rcv[10];
    printf("envoye ! \n");
    exact_read(sock_env, rcv, 9);
    rcv[9] = '\0';
    printf("\n%s\n",rcv);
    return 0;
}
