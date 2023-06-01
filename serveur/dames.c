#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "dames.h"

void initialiser_damier(damier *my_damier, int rb, int ad) {
	/*Toutes les cases sont vides*/
	int m = 1;
	my_damier->pions_noirs = 20;
	my_damier->pions_blancs = 20;
    my_damier->rb = rb;
    my_damier->ad = ad;

	for(int i = 0; i < 10; i++) {
		for(int j = 0; j < 10 ; j++) {
			my_damier->pion_dm[i][j].contenu_case = CASE_VIDE; // au départ aucun pion n'est sur le damier 
			my_damier->pion_dm[i][j].ligne = i;
            my_damier->pion_dm[i][j].couleur = -1;
			my_damier->pion_dm[i][j].colonne = j;
			if((i %2 == 0 && j %2 != 0) || (i%2 != 0 && j%2 == 0)) { // si i est impair et j pair (ou iversement) => c'est une case noire
				my_damier->pion_dm[i][j].manoury = m; // à revoir 
				m++;
			}
			else 
				my_damier->pion_dm[i][j].manoury = -1; // case blanche, pas de notation manory

		}
	}

	/* mettre les pions noirs sur la partie haute du damier (4 premières lignes)*/
	int j = 0;
	for(int i = 0; i < 4; i++) {
		if(i %2 == 0) //la première case de la ligne i est blanche
			j = 1; 
		else
			j = 0; // la première case de la ligne est noire
		for(j; j < 10; j += 2) {
			my_damier->pion_dm[i][j].contenu_case = PION_NOIR;
            my_damier->pion_dm[i][j].couleur = 0;
		}
	}

	/* mettre les pions blancs sur la partie basse du damier (4 premières lignes) */

	for(int i = 6; i < 10; i++) {

		if(i %2 == 0) //la première case de la ligne i est blanche
			j = 1; 
		else
			j = 0; // la première case de la ligne est noire
		for(j; j < 10; j += 2) {
			my_damier->pion_dm[i][j].contenu_case = PION_BLANC;
            my_damier->pion_dm[i][j].couleur = 1;
		}
	}
}

liste *initialiser_liste_vide() {
    liste *ls = malloc(sizeof(liste));
    ls->premier = NULL;
    ls->dernier = NULL;
    ls->nb = 0;
    return ls;
}

void ajouter_elem(liste *ls, int i, int j) {
    element *e = malloc(sizeof(element));
    e->i = i;
    e->j = j;
    e->suivant = NULL;
    if(ls->nb == 0) {
        ls->premier = e;
        ls->dernier = e;
        (ls->nb)++;
    }
    else {
        (ls->dernier)->suivant = e;
        ls->dernier = e;
        (ls->nb)++;
    }
}
void ajouter_debut(liste *ls, int i, int j) {
    element *e = malloc(sizeof(element));
    e->i = i;
    e->j = j;
    e->suivant = NULL;
    if(ls->nb == 0) {
        ls->premier = e;
        ls->dernier = e;
        (ls->nb)++;
    }
    else {
        e->suivant = ls->premier;
        ls->premier = e;
        (ls->nb)++;
    }
}
/* fonctions de transformation manoury en coordonnées et inversement */

cpl* man_to_coord(int manoury) {
    // transformer le manoury en un entier
    int entier=0;

    if (manoury%10>0 && manoury%10<6)  // man vaut de 1 à 5
        entier=2*manoury-1;
    if ((manoury%10>5 && manoury%10<10) || manoury%10==0) // man vaut de 6 à 9 ou 0
        entier=2*manoury-2;

    // transformer l'entier en coordonnées
    cpl* coord = malloc(sizeof(cpl));
    coord->i=entier/10;
    coord->j=entier%10;

    return coord;
}

int coord_to_man(cpl* coord) {
  // tester si la case est blanche
    if(((coord->i) % 2==0 && (coord->j)%2==0)||((coord->i)%2 !=0 && (coord->j)%2 !=0)) // si la case est blanche
        return -1;
  
  // transformer les coordonnées en un entier
    int entier=(coord->i)*10+(coord->j);
  // transformer l'entier en manoury
    int *manoury = malloc(sizeof(int)); 
    *manoury = (entier/2)+1;
    return *manoury;

}

int valeur_absolue(int x) {
    if(x >= 0)
        return x;
    return (-1 * x);
}

liste *test_prise(int lg, int cl, damier *d) {
    /* test si la case contient une dame ou un un pion et renvoie une liste chainée avec toutes les prises possibles
    Cette fonction ne renvoie pas la raffle maximale  mais juste les pions qui peuvent être pris sur le moment, (ceux qui sont à proximité) */
    liste *ls = initialiser_liste_vide();
    if((d->pion_dm[lg][cl].contenu_case) == DAME_NOIRE || (d->pion_dm[lg][cl].contenu_case) == DAME_BLANCHE) {
        int i = lg - 1;
        int j = cl + 1;
        while(i >= 1 && j < 9) {
            if(d->pion_dm[i][j].couleur == d->rb)
                break;
            else if(d->pion_dm[i][j].couleur == d->ad) {
        
                if(d->pion_dm[i-1][j+1].contenu_case == CASE_VIDE)
                    ajouter_elem(ls, i, j);
                else 
                    break; 
            }
            i--;
            j++;
        }
        i = lg + 1;
        j = cl - 1;
        while(i < 9 && j >= 1) {
            if(d->pion_dm[i][j].couleur == d->rb)
                break;
            else if(d->pion_dm[i][j].couleur == d->ad) {
                if(d->pion_dm[i+1][j-1].contenu_case == CASE_VIDE)
                    ajouter_elem(ls, i, j); 
                else 
                    break;
            }
            i++;
            j--;
        }
        i = lg - 1;
        j = cl - 1;
        while(i >= 1 && j >= 1) {
            if(d->pion_dm[i][j].couleur == d->rb)
                break;
            else if(d->pion_dm[i][j].couleur == d->ad) {
        
                if(d->pion_dm[i-1][j-1].contenu_case == CASE_VIDE)
                    ajouter_elem(ls, i, j); 
                else 
                    break;
            }
            i--;
            j--;
        }
        i = lg + 1;
        j = cl + 1;
        while(i < 9 && j < 9) {
            if(d->pion_dm[i][j].couleur == d->rb)
                break;
            else if(d->pion_dm[i][j].couleur == d->ad) {
                if(d->pion_dm[i+1][j+1].contenu_case == CASE_VIDE)
                    ajouter_elem(ls, i, j); 
                else
                    break;
            }
            i++;
            j++;
        }
        printf("finito\n");

    }
    else { 
        if((lg+1 >= 0 && lg+1 < 10 && cl+1 >= 0 && cl+1 < 10) && (d->pion_dm[lg+1][cl+1]).couleur == d->ad) { //bas droite
            if(lg+2 >= 0 && lg+2 < 10 && cl+2 >= 0 && cl+2 < 10 ) {
                if((d->pion_dm[lg+2][cl+2]).contenu_case == CASE_VIDE) {
                    //prise obligatoire
                    ajouter_elem(ls, lg+1, cl+1);
                }
            }
        }  
        if(lg-1 >= 0 && lg-1 < 10 && cl-1 >= 0 && cl-1 < 10 && (d->pion_dm[lg-1][cl-1]).couleur == d->ad) { //haut gauche
            if(lg-2 >= 0 && lg-2 < 10 && cl-2 >= 0 && cl-2 < 10 ) {
                if((d->pion_dm[lg-2][cl-2]).contenu_case == CASE_VIDE) {
                    //prise obligatoire
                    ajouter_elem(ls, lg-1, cl-1);
                }
            }
        }

        if(lg+1 >= 0 && lg+1 < 10 && cl-1 >= 0 && cl-1 < 10 && (d->pion_dm[lg+1][cl-1]).couleur == d->ad) { // bas gauche
            if(lg+2 >= 0 && lg+2 < 10 && cl-2 >= 0 && cl-2 < 10 ) {
                if((d->pion_dm[lg+2][cl-2]).contenu_case == CASE_VIDE) {
                    //prise obligatoire
                    ajouter_elem(ls, lg+1, cl-1);
                }
            }
        } 
        if(lg-1 >= 0 && lg-1 < 10 && cl+1 >= 0 && cl+1 < 10 && (d->pion_dm[lg-1][cl+1]).couleur == d->ad) { //haut droite
            if(lg-2 >= 0 && lg-2 < 10 && cl+2 >= 0 && cl+2 < 10 ) {
                if((d->pion_dm[lg-2][cl+2]).contenu_case == CASE_VIDE) {
                    //prise obligatoire
                    ajouter_elem(ls, lg-1, cl+1);
                }
            }
        }
    }
    //printf("ls->nb = %d\n",ls->nb); // pour des tests
    return ls; // return la liste des prises possibles;
}

/* fonction de déplacement simple sans prise */
int test_deplacement_pion(int ligne1, int colonne1, int ligne2, int colonne2, damier *d) {
    if(!test_sortie_cadre(ligne1,colonne1) || !test_sortie_cadre(ligne2,colonne2)) {
        return 0; // on sort du cadre
    }
    if((d->pion_dm[ligne2][colonne2].contenu_case) != CASE_VIDE) {
        return 0; // le déplacement ne peut pas être fait car la case n'est pas vide
    }

    if(((d->pion_dm[ligne1][colonne1].contenu_case) == CASE_VIDE) || (d->pion_dm[ligne1][colonne1].couleur) == d->ad) {
        return 0; // le déplacement ne peut pas être fait, la case de départ est vide ou le pion ne m'appartient pas
    }
    return 1;
}
int deplacement_pion(int ligne1, int colonne1, int ligne2, int colonne2, damier *d) { // pas de notation de Manoury
    if(!test_deplacement_pion(ligne1, colonne1, ligne2, colonne2, d))
        return 0;
    printf("(fonction deplacment_pion) :contenu avant deplacement %d\n", d->pion_dm[ligne1][colonne1].couleur);
    d->pion_dm[ligne2][colonne2].contenu_case = d->pion_dm[ligne1][colonne1].contenu_case;
    d->pion_dm[ligne2][colonne2].couleur = d->pion_dm[ligne1][colonne1].couleur;
	d->pion_dm[ligne1][colonne1].contenu_case = CASE_VIDE;
    d->pion_dm[ligne1][colonne1].couleur = -1;

    if(d->rb == 1 && ligne2 == 0) {
        d->pion_dm[ligne2][colonne2].contenu_case = DAME_BLANCHE;
    }
    if(d->rb == 0 && ligne2 == 9) {
        d->pion_dm[ligne2][colonne2].contenu_case = DAME_NOIRE;
    }


    return 1; // le déplacement a bien été fait
}

int deplacement_pion_adverse(int man1, int man2, damier *d) { // fonction de déplacement simple (pas de prise).
	/* On récupère les coordonnées du pion avant et après le coup */
    printf("(fonction deplacement_pion_adverse) deplacement de  %d vers %d\n", man1, man2);
	int i1,i2,j1,j2;
	cpl *cd_from = man_to_coord(man1);
	cpl *cd_to = man_to_coord(man2);
    // On récupere les coord des deux cases.
	i1 = cd_from->i;
	j1 = cd_from->j;
	i2 = cd_to->i;
	j2 = cd_to->j;
 
    if(!test_sortie_cadre(i1,j1) || !test_sortie_cadre(i2,j2)) {
        return 0; // le joeur est sortie du cadre
    }

    /* Un tas de tests */
    if((d->pion_dm[i2][j2].contenu_case) != CASE_VIDE) {
        return 0; // le déplacement ne peut pas être fait car la case n'est pas vide
    }
    if(((d->pion_dm[i1][j1].contenu_case) == CASE_VIDE) || (d->pion_dm[i1][j1].couleur) == d->rb) { 
        return 0; // le déplacement ne peut pas être fait, le joueur essaie de déplacer un pion qui ne lui appartient pas
    }
    
    if((d->pion_dm[i2][j2].couleur) == d->rb) {
        return 0; // le déplacement ne peut pas être fait car le joeur essaie de déplacer une dame qui ne lui appartient pasj1
    }
    
    if(j2 == j1 || i2 == i1) {
        return 0; //impossible de se déplacer sur la même ligne ou la même colonne
    }

    if((d->ad == 1 && d->pion_dm[i1][j1].contenu_case == PION_BLANC) ||(d->ad == 0 && d->pion_dm[i1][j1].contenu_case == PION_NOIR)) {
        if(valeur_absolue(i2 - i1) != 1 || valeur_absolue(j2 - j1) != 1)
            return 0; // l'adversaire a sauté plusieurs cases sans aucune prise
    }
    //c'est une dame on doit parcourir les 4 diagonales pour voir s'il n'a pas sauté un pion ou autre mouvement interdit
    if(d->pion_dm[i1][j1].contenu_case == DAME_BLANCHE || d->pion_dm[i1][j1].contenu_case == DAME_NOIRE) { 
        if(j2 > j1) { //droite
            if(i2 < i1) { // haut
                int i = i1 - 1;
                int j = j1 + 1;
                while(i >= i2 && j <= j2) {
                    if(d->pion_dm[i][j].contenu_case != CASE_VIDE)
                        return 0;
                    i--;
                    j++;
                }
            }
            else { //bas
                int i = i1 + 1;
                int j = j1 + 1;
                while(i <= i2 && j <= j2) {
                    if(d->pion_dm[i][j].contenu_case != CASE_VIDE)
                        return 0;
                    i++;
                    j++;
                }
            }
        }
        else { //gauche
            if(i2 < i1) { // haut
                int i = i1 - 1;
                int j = j1 - 1;
                while(i >= i2 && j >= j2) {
                    if(d->pion_dm[i][j].contenu_case != CASE_VIDE)
                        return 0;
                    i--;
                    j--;
                }
            }
            else { //bas
                int i = i1 + 1;
                int j = j1 - 1;
                while(i <= i2 && j >= j2) {
                    if(d->pion_dm[i][j].contenu_case != CASE_VIDE)
                        return 0;
                    i++;
                    j--;
                }
            }
        }
    }


    /* le déplacement peut enfin être fait */
    d->pion_dm[i2][j2].contenu_case = d->pion_dm[i1][j1].contenu_case;
    d->pion_dm[i2][j2].couleur = d->pion_dm[i1][j1].couleur;
	d->pion_dm[i1][j1].contenu_case = CASE_VIDE;
    d->pion_dm[i1][j1].couleur = -1;

    if(d->ad == 1 && i2 == 0) {
        d->pion_dm[i2][j2].contenu_case = DAME_BLANCHE;
    }
    if(d->ad == 0 && i2 == 9) {
        d->pion_dm[i2][j2].contenu_case = DAME_NOIRE;
    }
    return 1;
}

int prise_adverse(int man1, int man2, damier *d) { // prend les deux manoury recu par l'adversaire
	/* On récupère les coordonnées du pion avant et après le coup */
    if(man1 == man2)
        return 0;
    if(man1 > 50 || man1 < 1 || man2 < 1 || man2 > 50)
        return 0;
	int i1,i2,j1,j2;
	cpl *cd_from = man_to_coord(man1);
	cpl *cd_to = man_to_coord(man2);
    // On récupere les coord des deux cases.
	i1 = cd_from->i;
	j1 = cd_from->j;
	i2 = cd_to->i;
	j2 = cd_to->j;
    if((d->ad == 0 && d->pion_dm[i1][j1].contenu_case == PION_NOIR) ||(d->ad == 1 && d->pion_dm[i1][j1].contenu_case == PION_BLANC) ) {
        if(d->pion_dm[i2][j2].contenu_case != CASE_VIDE)
            return 0;
        
        int pi, pj; // coordonnees du pion pris par l'adversaire
        if(i2 > i1 && j2 > j1) {
            pi = i2 - 1;
            pj = j2 - 1;
        }
        else if(i2 < i1 && j2 > j1) {
            pi = i2 + 1;
            pj = j2 - 1;
        }
        else if(i2 < i1 && j2 < j1) {
            pi = i2 + 1;
            pj = j2 + 1;
        }
        else if(i2 > i1 && j2 < j1) {
            pi = i2 - 1;
            pj = j2 + 1;
        }
        if(pi > 9 || pi < 0 || pj > 9 || pj < 0)
            return 0;
        d->pion_dm[pi][pj].contenu_case = CASE_VIDE;
        d->pion_dm[pi][pj].couleur = -1;

        d->pion_dm[i2][j2].couleur = d->pion_dm[i1][j1].couleur;
        d->pion_dm[i2][j2].contenu_case = d->pion_dm[i1][j1].contenu_case;
        d->pion_dm[i1][j1].contenu_case = CASE_VIDE;
        d->pion_dm[i1][j1].couleur = -1;
        if(d->ad == 0)
                        d->pions_blancs--;
                    else    
                        d->pions_noirs--;
        return 1;

    }
    //sinon c'est une dame plusieurs tests sur les 4 diagonales
    else {
        if(i2 > i1 && j2 > j1) {
            int i = i1 + 1;
            int j = j1 + 1;
            while (i < i2 && j < j2) {
                if(d->pion_dm[i][j].couleur == d->rb && d->pion_dm[i+1][j+1].couleur == -1) {

                    d->pion_dm[i2][j2].couleur = d->pion_dm[i1][j1].couleur;
                    d->pion_dm[i2][j2].contenu_case = d->pion_dm[i1][j1].contenu_case;
                    d->pion_dm[i1][j1].contenu_case = CASE_VIDE;
                    d->pion_dm[i1][j1].couleur = -1;
                    d->pion_dm[i][j].couleur = -1;
                    d->pion_dm[i][j].contenu_case = CASE_VIDE;
                    if(d->ad == 0)
                        d->pions_blancs--;
                    else    
                        d->pions_noirs--;
                    return 1;

                }
                i++;
                j++;
            }
        }
    
        if(i2 < i1 && j2 < j1) {
            int i = i1 - 1;
            int j = j1 - 1;
            while (i > i2 && j > j2) {
                if(d->pion_dm[i][j].couleur == d->rb && d->pion_dm[i-1][j-1].couleur == -1) {

                    d->pion_dm[i2][j2].couleur = d->pion_dm[i1][j1].couleur;
                    d->pion_dm[i2][j2].contenu_case = d->pion_dm[i1][j1].contenu_case;
                    d->pion_dm[i1][j1].contenu_case = CASE_VIDE;
                    d->pion_dm[i1][j1].couleur = -1;
                    d->pion_dm[i][j].couleur = -1;
                    d->pion_dm[i][j].contenu_case = CASE_VIDE;
                    if(d->ad == 0)
                        d->pions_blancs--;
                    else    
                        d->pions_noirs--;
                    return 1;

                }
                i--;
                j--;
            }
        }
        if(i2 > i1 && j2 < j1) {
            int i = i1 + 1;
            int j = j1 - 1;
            while (i < i2 && j > j2) {
                if(d->pion_dm[i][j].couleur == d->rb && d->pion_dm[i+1][j-1].couleur == -1) {

                    d->pion_dm[i2][j2].couleur = d->pion_dm[i1][j1].couleur;
                    d->pion_dm[i2][j2].contenu_case = d->pion_dm[i1][j1].contenu_case;
                    d->pion_dm[i1][j1].contenu_case = CASE_VIDE;
                    d->pion_dm[i1][j1].couleur = -1;
                    d->pion_dm[i][j].couleur = -1;
                    d->pion_dm[i][j].contenu_case = CASE_VIDE;
                    if(d->ad == 0)
                        d->pions_blancs--;
                    else    
                        d->pions_noirs--;
                    return 1;

                }
                i++;
                j--;
            }
        }
        if(i2 < i1 && j2 > j1) {
            int i = i1 - 1;
            int j = j1 + 1;
            while (i > i2 && j < j2) {
                if(d->pion_dm[i][j].couleur == d->rb && d->pion_dm[i-1][j+1].couleur == -1) {

                    d->pion_dm[i2][j2].couleur = d->pion_dm[i1][j1].couleur;
                    d->pion_dm[i2][j2].contenu_case = d->pion_dm[i1][j1].contenu_case;
                    d->pion_dm[i1][j1].contenu_case = CASE_VIDE;
                    d->pion_dm[i1][j1].couleur = -1;
                    d->pion_dm[i][j].couleur = -1;
                    d->pion_dm[i][j].contenu_case = CASE_VIDE;
                    if(d->ad == 0)
                        d->pions_blancs--;
                    else    
                        d->pions_noirs--;
                    return 1;

                }
                i--;
                j++;
            }
        }
    }
   

    return 0;
   

}


int test_sortie_cadre(int lg, int cl) {
    if(lg >= 10 || lg < 0 || cl >= 10 || cl < 0)
        return 0;
    return 1;
}

void nouveau_damier(damier *d1, damier *d2) { // copie le damier d1 dans le damier d2
    d2->pions_blancs = d1->pions_blancs;
    d2->pions_noirs = d1->pions_noirs;
    d2->rb = d1->rb;
    d2->ad = d1->ad;
    for(int i = 0; i < 10; i++) {
        for(int j = 0; j < 10; j++) {
            d2->pion_dm[i][j].ligne = d1->pion_dm[i][j].ligne; 
            d2->pion_dm[i][j].colonne = d1->pion_dm[i][j].colonne;
            d2->pion_dm[i][j].manoury = d1->pion_dm[i][j].manoury;
            d2->pion_dm[i][j].contenu_case = d1->pion_dm[i][j].contenu_case;
            d2->pion_dm[i][j].couleur = d1->pion_dm[i][j].couleur; 
        }
    }
}
/* fonction qui stocke la rafle maximale dans une liste chainée. Fonction réccursive qui pour chaque prise possible sur le moment
fera un appel réccursif afin de calculer la rafle dans ce cas-là. Ensuite on compare la taille de toutes les chaines afin de prendre la plus
grande et ce sera la rafle maximale*/
liste *rafle_max(damier *d, int lg, int cl) {

    liste *ls = initialiser_liste_vide(); // liste qui va stocker tous les pions qu'on peut prendre sur le moment 
    liste *ls_prise = test_prise(lg, cl, d); // test_prise() renvoie cette liste
    liste *tab_p[ls_prise->nb]; // tableau qui stocke la rafle max pour chaque possibilité de prise
    damier *tab_d[ls_prise->nb]; // tableau de damier pour chaque prise possible
    for(int i = 0; i < ls_prise->nb; i++) {
        tab_p[i] = malloc(sizeof(liste));   
        tab_d[i] = malloc(sizeof(damier));
    }
    element *el = ls_prise->premier;
    element *el_max; 
    int max = 0;
    liste *ls_max;

  
    for(int i = 0; i < ls_prise->nb; i++) {
        nouveau_damier(d, tab_d[i]);
        cpl *c = prise(lg, cl, el->i, el->j, tab_d[i]); // renvoie la nouvelle position du pion
        tab_p[i] = rafle_max(tab_d[i], c->i, c->j);
        if(tab_p[i]->nb >= max) {
            max = tab_p[i]->nb; 
            ls_max = tab_p[i];
            el_max = el;
        }
        el = el->suivant;
    }
    if(ls_prise->nb == 0)
        return initialiser_liste_vide();
    ajouter_debut(ls_max, el_max->i, el_max->j);
    return ls_max; 
}

cpl *prise(int l, int c, int l2, int c2, damier *d) {
    cpl *couple = malloc(sizeof(cpl));
    if(l2 < l && c2 > c) {
        deplacement_pion(l, c, l2-1, c2+1, d);
        d->pion_dm[l2][c2].contenu_case = CASE_VIDE;
        d->pion_dm[l2][c2].couleur = -1;
        couple->i = l2-1;
        couple->j = c2+1;
    }
    if(l2 < l && c2 < c) {
        deplacement_pion(l, c, l2-1, c2-1, d);
        d->pion_dm[l2][c2].contenu_case = CASE_VIDE;
        d->pion_dm[l2][c2].couleur = -1;
        couple->i = l2-1;
        couple->j = c2-1;
    }
    if(l2 > l && c2 > c) {
        deplacement_pion(l, c, l2+1, c2+1, d);
        d->pion_dm[l2][c2].contenu_case = CASE_VIDE;
        d->pion_dm[l2][c2].couleur = -1;
        couple->i = l2+1;
        couple->j = c2+1;
    }
    if(l2 > l && c2 < c) {
        deplacement_pion(l, c, l2+1, c2-1, d);
        d->pion_dm[l2][c2].contenu_case = CASE_VIDE;
        d->pion_dm[l2][c2].couleur = -1;
        couple->i = l2+1;
        couple->j = c2-1;
    }
    if(d->ad == 0)
        d->pions_noirs--;
    else    
        d->pions_blancs--;

    return couple;
}
/* fonction qui renvoie une liste de pions ayant une possibilité de prise sur le moment*/
liste *possibilite_prise(damier *d) {
    int clr = d->rb;
    liste *ls = initialiser_liste_vide();
    for(int i = 0; i < 10; i++) {
        for(int j = 0; j < 10; j++) {
            if(d->pion_dm[i][j].couleur == clr) {
                liste *l1 = test_prise(i, j, d);
                if(l1->nb > 0 )
                    ajouter_elem(ls, i, j);
            }
        }
    }
    return ls;
}

/* renvoie un couple d'indice (coordonnées du pion) ayant une rafle maximale par rapport à tous les autres pions 
l'algorithme devra recalculer cette rafle max*/
cpl *rafle_damier(damier *d) {
    liste *ls = possibilite_prise(d);
    printf("e\n");
    liste *liste_max;
    int max = 0;
    int i;
    int j;
    cpl *c = malloc(sizeof(cpl));
    if(ls->nb == 0) {
        i = -1;
        j = -1;
        c->i = i;
        c->j = j;
        return c;
    }
    element *e = ls->premier;
    while(e != NULL) {
        liste *l = rafle_max(d, e->i, e->j);
        if(l->nb >= max) {
            max = l->nb;
            i = e->i;
            j = e->j; 
        }
        e = e->suivant;
    }
    c->i = i;
    c->j = j;
    return c;
}
