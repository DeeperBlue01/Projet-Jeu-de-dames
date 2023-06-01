#ifndef _ALG_
#define _ALG_

#include<stdlib.h>
#include<stdio.h>
#include "dames.h"

/*Fonction qui nous permet de verifier si un deplacement peut etre realisé ou non. Prends en parametre les coordonnées de depart, 
d'arrive et le damier.Renvoie 1 si c'est possible, renvoie 0 sinon*/
int deplacement_possible(int lg1, int cl1, int lg2, int cl2, damier *d);

/*Liste tous les pions à proteger des pions adverses. Prend en paramètre le damier et renvoie une liste*/
liste *protection_damier(damier *d_initial);

/*Verifie parmi la liste des pions à proteger si il est possible de les proteger. Si c'est le cas, il le fait et renvoie 1, 
Renvoie 0 sinon. Prend en parametre le damier et 2 couples pour pouvoir stocker les coordonnes de la case de depart et d'arrive*/
int protection_possible(damier *d, cpl *c1, cpl *c2);

/*Fonction qui stock dans 2 listes distinctes les pions que l'on peut deplacer mais avec le risque de se faire manger (ls1)et
les pions que l'on peut deplacer sans danger(ls2).Renvoie le nombre d'elements dans la liste 1*/
int possibilite_deplacement(damier *d, liste *ls1, liste *ls2);

/*Deplace aleatoirement un pion dans la liste 2 en priorite, si elle vide elle deplace un de la liste 1, si les 2 sont vides renvoie 0*/
int deplacement_aleatoire(damier *d, cpl *c1, cpl *c2);

/*Algorithme qui sert a attaque soit avec une prise, une protection, ou bien un deplacement aleatoire*/
int algorithme(damier *d, liste *ls_deplacement);

/*Algorithme qui sert a deplacer les pions en fonction des coups que l'adversaire nous envoie */
int algorithme_defense(damier *d, int tab[], int n, int verif);

#endif
