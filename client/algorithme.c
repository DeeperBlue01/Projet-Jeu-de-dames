#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "dames.h"

/*Fonction qui nous permet de verifier si un deplacement peut etre realisé ou non. Prends en parametre les coordonnées de depart, 
d'arrive et le damier.Renvoie 1 si c'est possible, renvoie 0 sinon*/
int deplacement_possible(int lg1, int cl1, int lg2, int cl2, damier *d) {
    if(lg1 >= 10 || lg1 < 0 || lg2 >= 10 || lg2 < 0)
        return 0;

    if(d->pion_dm[lg2][cl2].contenu_case != CASE_VIDE)
        return 0;
    if(d->pion_dm[lg1][cl1].contenu_case == PION_BLANC || d->pion_dm[lg1][cl1].contenu_case == PION_NOIR) {
        if(valeur_absolue(lg2 - lg1) != 1 || valeur_absolue(cl2 - cl1) != 1)
            return 0;
    }
    if(d->rb == 1 && lg2 >= lg1 && d->pion_dm[lg1][cl1].contenu_case == PION_BLANC) // pas de retour en arrière
        return 0;
    if(d->rb == 0 && lg2 <= lg1 && d->pion_dm[lg1][cl1].contenu_case == PION_NOIR)
        return 0;
    else if(d->pion_dm[lg1][cl1].contenu_case == DAME_BLANCHE || d->pion_dm[lg1][cl1].contenu_case == DAME_NOIRE){
        int i, j;
        if(lg2 > lg1 && cl2 > cl1) {
            i = lg1 + 1;
            j = cl1 + 1;
            while(i < lg2 && j < cl2) {
                if(d->pion_dm[i][j].contenu_case != CASE_VIDE)
                    return 0;
                i++;
                j++;
            }
            
        }
        else if(lg2 < lg1 && cl2 > cl1) {
            i = lg1 - 1;
            j = cl1 + 1;
            while(i > lg2 && j < cl2) {
                if(d->pion_dm[i][j].contenu_case != CASE_VIDE)
                    return 0;
                i--;
                j++;
            }
        }
        else if(lg2 < lg1 && cl2 < cl1) {
            i = lg1 - 1;
            j = cl1 - 1;
            while(i > lg2 && j > cl2) {
                if(d->pion_dm[i][j].contenu_case != CASE_VIDE)
                    return 0;
                i--;
                j--;
            }
        }
        else if(lg2 > lg1 && cl2 < cl1) {
            i = lg1 + 1;
            j = cl1 - 1;
            while(i < lg2 && j > cl2) {
                if(d->pion_dm[i][j].contenu_case != CASE_VIDE)
                    return 0;
                i++;
                j--;
            }
        }   
        return 1;
    }
    return 1;
}


/* stratégie protection */
/*Liste tous les pions à proteger des pions adverses. Prend en paramètre le damier et renvoie une liste*/
liste *protection_damier(damier *d_initial) {
    damier *d = malloc(sizeof(damier));
    nouveau_damier(d_initial, d);
    d->rb = d_initial->ad;
    d->ad = d_initial->rb;
    liste *renv = initialiser_liste_vide();

    for(int i = 0; i < 10; i++) {
        for(int j = 0; j < 10; j++) {

            if(d->pion_dm[i][j].couleur == d->rb) {
                liste *l = test_prise(i, j, d);
                if(l->nb != 0) {

                    element *e = l->premier;
                    while(e != NULL) { //on se met à la place de l'adversaire et on test les prises qu'il peut prendre
                        damier *d2 = malloc(sizeof(damier));
                        nouveau_damier(d, d2);
                        cpl *c = prise(i, j, e->i, e->j ,d2);
                        ajouter_elem(renv, c->i, c->j);
                        e = e->suivant;
                    }
                    
                }
            }

        }
    } 
    return renv;   
}

/*Verifie parmi la liste des pions à proteger si il est possible de les proteger. Si c'est le cas, il le fait et renvoie 1, 
Renvoie 0 sinon. Prend en parametre le damier et 2 couples pour pouvoir stocker les coordonnes de la case de depart et d'arrive*/
int protection_possible(damier *d, cpl *c1, cpl *c2) {
   
    liste *ls = protection_damier(d);
    if(ls->nb == 0)
        return 0;
    element *e = ls->premier;
    
    while(e != NULL) {
        for(int i = 0; i < 10; i++) {
            for(int j = 0; j < 10; j++) {
                if(d->pion_dm[i][j].couleur == d->rb) {
                    if(deplacement_possible(i, j, e->i, e->j, d)) {
                         printf("(fct protection possible) protection possible \n");
                        int depl = deplacement_pion(i, j, e->i, e->j, d);
                  
                        /*if(depl == 0)
                            printf("erreur\n");*/

                        c1->i = i;
                        c1->j = j;
                        c2->i = e->i;
                        c2->j = e->j;
                        return 1;
                    }
                }
            }
        }
        e = e->suivant;
    }
    return 0; 
}

/* prise et protection impossible */
/*Fonction qui stock dans 2 listes distinctes les pions que l'on peut deplacer mais avec le risque de se faire manger (ls1)et
les pions que l'on peut deplacer sans danger(ls2).Renvoie le nombre d'elements dans la liste 1*/
int possibilite_deplacement(damier *d, liste *ls1, liste *ls2) {
    int nb = 0;
    for(int i = 0; i < 10; i++) {
        for(int j = 0; j < 10; j++) {
            if(d->rb == 1) {
                if(d->pion_dm[i][j].couleur == 1) {
                    if(deplacement_possible(i, j, i-1, j+1, d) || deplacement_possible(i, j, i-1, j-1, d)) {
                        ajouter_elem(ls1, i, j);
                        nb++;
                        if(deplacement_possible(i, j, i-1, j+1, d) && test_deplacement_pion(i, j, i-2, j+2, d)) {
                            ajouter_elem(ls2, i, j);
                        }
                        else if(deplacement_possible(i, j, i-1, j-1, d) && test_deplacement_pion(i, j, i-2, j-2, d))
                            ajouter_elem(ls2, i, j);
           
                    }
                }
            }
            else {
                if(d->pion_dm[i][j].couleur == 0) {
                    if(deplacement_possible(i, j, i+1, j+1, d) || deplacement_possible(i, j, i+1, j-1, d)) {
                        ajouter_elem(ls1, i, j);
                        nb++;
                        if(deplacement_possible(i, j, i+1, j+1, d) && test_deplacement_pion(i, j, i+2, j+2, d)) {
                            ajouter_elem(ls2, i, j);
                        }
                        else if(deplacement_possible(i, j, i+1, j-1, d) && test_deplacement_pion(i, j, i+2, j-2, d))
                            ajouter_elem(ls2, i, j);
                    
                    }
                }
            }
        }
    }
    return nb;
}

/*Deplace aleatoirement un pion dans la liste 2 en priorite, si elle vide elle deplace un de la liste 1, si les 2 sont vides renvoie 0*/
int deplacement_aleatoire(damier *d, cpl *c1, cpl *c2) {
    liste *ls1 = initialiser_liste_vide();
    liste *ls2 = initialiser_liste_vide();
    int nb = possibilite_deplacement(d, ls1, ls2);

    if(nb == 0)
        return 0; //aucun deplacement n'est possible
    cpl tab_cpl1[ls1->nb];
    cpl tab_cpl2[ls2->nb];
    element *c = ls1->premier;
    int i = 0;
    while(c != NULL) {
        tab_cpl1[i].i = c->i;
        tab_cpl1[i].j = c->j;
        i++;
        c = c->suivant;
    }
    
    if(ls2->nb > 0) { //on deplace en priorité les pions ne pouvant être pris par l'adversaire si on les déplace
        i = 0;
        c = ls2->premier;
        while(c != NULL) {
            tab_cpl2[i].i = c->i;
            tab_cpl2[i].j = c->j;
            i++;
            c = c->suivant;
        }
    }
    srand(time(NULL));
    if(ls2->nb > 0) {
        int ind = rand() % ls2->nb; //on choisi un pion au hasard et une direction au hasard
        int droite_gauche = rand() % 2; //0 gauche, 1 droite 
        int x = tab_cpl2[ind].i;
        int y = tab_cpl2[ind].j;
        if(droite_gauche == 0) {
            if(d->rb == 1) {
                if(test_deplacement_pion(x, y, x-1, y-1, d)) {
                    deplacement_pion(x, y, x-1, y-1, d);
                    c1->i = x;
                    c1->j = y;
                    c2->i = x-1;
                    c2->j = y-1;
                }
                else {
                    deplacement_pion(x, y, x-1, y+1, d);
                    c1->i = x;
                    c1->j = y;
                    c2->i = x-1;
                    c2->j = y+1;
                }
            }
            else { //un seul deplacement est possible
                if(test_deplacement_pion(x, y, x+1, y-1, d)) {
                    deplacement_pion(x, y, x+1, y-1, d);
                    c1->i = x;
                    c1->j = y;
                    c2->i = x+1;
                    c2->j = y-1;
                }
                else { 
                    deplacement_pion(x, y, x+1, y+1, d);
                    c1->i = x;
                    c1->j = y;
                    c2->i = x+1;
                    c2->j = y+1;
                }
            }
        }
        
        else { // si non nous sommes obligé de déplacer un pion qui peut être pris par l'adversaire
            if(d->rb == 1) {
                if(test_deplacement_pion(x, y, x-1, y+1, d)) {
                    deplacement_pion(x, y, x-1, y+1, d);
                    c1->i = x;
                    c1->j = y;
                    c2->i = x-1;
                    c2->j = y+1;
                }
                else {
                    deplacement_pion(x, y, x-1, y-1, d);
                    c1->i = x;
                    c1->j = y;
                    c2->i = x-1;
                    c2->j = y-1;
                }
            }
            else {
                if(test_deplacement_pion(x, y, x+1, y+1, d)) {
                    deplacement_pion(x, y, x+1, y+1, d);
                    c1->i = x;
                    c1->j = y;
                    c2->i = x+1;
                    c2->j = y+1;
                }
                else {
                    deplacement_pion(x, y, x+1, y-1, d);
                    c1->i = x;
                    c1->j = y;
                    c2->i = x+1;
                    c2->j = y-1;
                }
            }
        }
    }
    else {
        srand(time(NULL));
        int ind = rand() % ls1->nb;
        int droite_gauche = rand() % 2; //0 gauche, 1 droite 
        int x = tab_cpl1[ind].i;
        int y = tab_cpl1[ind].j;

        if(droite_gauche == 0) {
            if(d->rb == 1) {
                if(test_deplacement_pion(x, y, x-1, y-1, d)) {
                    deplacement_pion(x, y, x-1, y-1, d);
                    c1->i = x;
                    c1->j = y;
                    c2->i = x-1;
                    c2->j = y-1;
                }
                else {
                    deplacement_pion(x, y, x-1, y+1, d);
                    c1->i = x;
                    c1->j = y;
                    c2->i = x-1;
                    c2->j = y+1;
                }
            }
            else {
                if(test_deplacement_pion(x, y, x+1, y-1, d)) {
                    deplacement_pion(x, y, x+1, y-1, d);
                    c1->i = x;
                    c1->j = y;
                    c2->i = x+1;
                    c2->j = y-1;
                }
                else {
                    deplacement_pion(x, y, x+1, y+1, d);
                    c1->i = x;
                    c1->j = y;
                    c2->i = x+1;
                    c2->j = y+1;
                }
            }
        }
        
        else {
            if(d->rb == 1) {
                if(test_deplacement_pion(x, y, x-1, y+1, d)) {
                    deplacement_pion(x, y, x-1, y+1, d);
                    c1->i = x;
                    c1->j = y;
                    c2->i = x-1;
                    c2->j = y+1;
                }
                else {
                    deplacement_pion(x, y, x-1, y-1, d);
                    c1->i = x;
                    c1->j = y;
                    c2->i = x-1;
                    c2->j = y-1;
                }
            }
            else {
                if(test_deplacement_pion(x, y, x+1, y+1, d)) {
                    deplacement_pion(x, y, x+1, y+1, d);
                    c1->i = x;
                    c1->j = y;
                    c2->i = x+1;
                    c2->j = y+1;
                }
                else {
                    deplacement_pion(x, y, x+1, y-1, d);
                    c1->i = x;
                    c1->j = y;
                    c2->i = x+1;
                    c2->j = y-1;
                }
            }
        }
    }

    printf(" (fct deplacmeent_aleatoire) contenu apres %d,apres  %d\n",d->pion_dm[c1->i][c1->j].couleur,d->pion_dm[c2->i][c2->j].couleur);

    return 1;
}

int algorithme(damier *d, liste *ls_deplacement) { //0 deplacement 1 prise
    printf("je suis dans fonction algortihme\n");
    cpl *c = rafle_damier(d);
    if(c->i != -1 && c->i != -1) { // prise obligatoire
    	printf("rafle\n");
        ajouter_elem(ls_deplacement, c->i, c->j);
        
        liste *ls = rafle_max(d, c->i, c->j);
        element *e = ls->premier;
        while(e != NULL) {
            c = prise(c->i, c->j, e->i, e->j, d);
            ajouter_elem(ls_deplacement, c->i, c->j);
            e = e->suivant;
        }
        return 1;
    }
    cpl *c1 = malloc(sizeof(cpl));
    cpl *c2 = malloc(sizeof(cpl));
    printf("(fct algorithme)verif protection \n");
    if(protection_possible(d, c1, c2)) {
        ajouter_debut(ls_deplacement, c2->i, c2->j);
        ajouter_debut(ls_deplacement, c1->i, c1->j);
        
        return 0;
    }

    else {
    	printf("(fct algorithme) aucune start -> deplacement aleatiore \n");
        cpl *c1 = malloc(sizeof(cpl));
        cpl *c2 = malloc(sizeof(cpl));
        deplacement_aleatoire(d, c1, c2);
        ajouter_debut(ls_deplacement, c2->i, c2->j);
        ajouter_debut(ls_deplacement, c1->i, c1->j);
        
        printf("(fct algorithme) cont 1 apres : %d, cont2 apres : %d\n", d->pion_dm[c1->i][c1->j].couleur,  d->pion_dm[c2->i][c2->j].couleur);
        return 0;
    }
    return 0;
    
}

int algorithme_defense(damier *d, int tab[], int n, int verif) {
    if(n < 2) {
    	printf("0\n");
        return 0;
    }
    if(!verif) {
    	printf("(fct algorithme_defense) deplacement simple\n");
        deplacement_pion_adverse(tab[0], tab[1], d);
        /*else
        	prise_adverse(tab[0], tab[1], d);*/

        cpl *c1 = malloc(sizeof(cpl));
        cpl *c2 = malloc(sizeof(cpl));
        c1 = man_to_coord(tab[0]);
        c2 = man_to_coord(tab[1]);
        printf("(fct algorithme_defense) contenu 1 :  %d,  ", d->pion_dm[c1->i][c1->j].couleur);

        printf("(fct algorithme_defense) contenu 2 : %d\n", d->pion_dm[c2->i][c2->j].couleur);
        printf("(fct algorithme_defense) j'ai fini de deplacer le pion adverse\n");
        return (1);

    }
    else {
    	printf("(fct algorithme_defense) prise\n");
        int i = 0;
        int j = 1;
        while(j < n) {
            printf("tab[i] = %u et tab[j] = %u\n",tab[i],tab[j]);
            if(!prise_adverse(tab[i], tab[j], d)) {
                return 0;
            }
            i++;
            j++;
        }
        return 1;
    }
    return 0;
}

