#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "reseau.h"
#include "dames.h"
#include "algorithme.h"

#include <unistd.h> // close, read, write
#include <sys/types.h>
#include <sys/socket.h>

#include <arpa/inet.h> /* inet_pton */
#include <netinet/in.h> 

#define PORT_DEFI 7777

char invalide[] = "COUP INVALIDE";
char perdu[] = "PERDU";
char egalite[] = "EGALITE";
char inter[] = "INTERUPTION";

int main()
{
	enrg *e = malloc(sizeof(enrg));

    /* 1. Création d'une socket tcp ipv4 */
	int tcp6_sock = socket(AF_INET6, SOCK_STREAM, 0);
	if (tcp6_sock < 0) {
		perror("Error Socket()");
		exit(2);
	}

    /* 2. Création de la sockaddr */
	struct sockaddr_in6 sa = { .sin6_family = AF_INET6, .sin6_port = htons(PORT_DEFI), .sin6_addr= in6addr_any};

    /* Faire en sorte de pouvoir réutiliser l'adresse sans attendre après la fin du serveur. */
	int opt = 1;
	setsockopt(tcp6_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	/* 3. Attacher la socket d'écoute à l'adresse */
	if (bind(tcp6_sock, (struct sockaddr *) &sa, sizeof(sa)) < 0) 
    {
		perror("Error Bind()");
		exit(3);
	}

	/* 4. Écouter sur la socket d'écoute */
	if (listen(tcp6_sock, 0) < 0) {
		perror("Error Listen()");
		exit(4);
	}

	struct sockaddr_in6 sockaddr_client_addr;
	int sock_echange;
	socklen_t addrlen=sizeof(struct sockaddr_in6);

	while(1)
	{
		/* 5. Attente passive d'une connexion. */
		if ((sock_echange = accept(tcp6_sock, (struct sockaddr *) &sockaddr_client_addr,&addrlen)) == -1 )
		{
			perror("Error Accept()");
			exit(5);
		}
		char char_client_address[INET6_ADDRSTRLEN];// const char * dst, voir man inet_ntop
		printf("Connexion etablie avec %s\n", inet_ntop(AF_INET6,&(sockaddr_client_addr.sin6_addr), char_client_address,INET6_ADDRSTRLEN));
		
		int clt = 0;
    	int srv = 0;
    	initialiser_enreg(e, 1, sockaddr_client_addr.sin6_addr.s6_addr);

		damier *d = malloc(sizeof(damier));
		initialiser_damier(d, 1, 0);
		int termine = 0;
		while(!termine && ((srv < 50) || (clt < 50))) {
			liste *ls = initialiser_liste_vide();
			int v = algorithme(d, ls);
			printf("\nje suis V : %d\n",v);
			printf("pions noies : %d, pions blancs : %d\n",d->pions_noirs, d->pions_blancs);
			
			if(!v) { // deplacement
				deplacer(sock_echange, ls, e);
			}
			else { //prise
				prendre(sock_echange, ls, e);
				if(d->pions_noirs == 0)
					break;
			}
			srv++;
			if(!cp_adversaire(sock_echange, d, e)) {
				termine = 1;
				break;
			}
			else {
				if(d->pions_blancs == 0) {
					termine = 1;
					break;
				}
			}
			clt++;
			free(ls);
			if(d->pions_blancs == 0 || d->pions_noirs == 0) {
				termine = 1;
			}
			
		}
		close(sock_echange);
		printf("partie termine ! \nnombre de coups client : %d\nnombre de coups serveur : %d\n\n",clt,srv);
		printf("nombre de pions client : %d\nnombre de pions serveur : %d\n\n",d->pions_blancs,d->pions_noirs);

		while(e->i < 256) {
			ajouter_coup(e, 0);
		}
		printf("\n");

		int sock_env;
	    if ((sock_env = socket(AF_INET6, SOCK_STREAM, 0)) < 0) {
			perror("Error Socket() Client");
			exit(2);
		}
	   
	    struct sockaddr_in6 addr_r = { .sin6_family = AF_INET6, .sin6_port = htons(5777) };

	    if(inet_pton(AF_INET6, "2001:910:1410:523:0:fada:80af:2bc2", &addr_r.sin6_addr) != 1)
	    {
	        fprintf(stderr, "Adresse IPv6 invalide\n");
			exit(1);
	    }
	    
	    
	    if ( connect(sock_env, (struct sockaddr *)&addr_r, sizeof(addr_r)) < 0)
	    {
	       	perror("Error Connect()");
			exit(3);
	    }
	    printf("je vais env\n");

	    exact_write(sock_env, e->tab, e->i - 1);
	    char rcv[10];
	    exact_read(sock_env, rcv, 9);
	    rcv[9] = '\0';
	    printf("\n%s\n",rcv);
	}


	close(tcp6_sock);

 return EXIT_SUCCESS;
}
