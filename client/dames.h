#ifndef _DMH_
#define _DMH_

#include<stdlib.h>
#include<stdio.h>


#define CASE_VIDE -1
#define PION_NOIR 0
#define PION_BLANC 1
#define DAME_NOIRE 2
#define DAME_BLANCHE 3


typedef struct {
	int ligne;
	int colonne;
	int manoury;
	int contenu_case; // Contient l'une des constantes (pion noir, blanc ...)
    int couleur; // 1 blanc, 0 noir, -1 si case vide
} cases_t;


typedef struct {
	cases_t pion_dm[10][10];
	int pions_blancs; // nb de pions
	int pions_noirs;
    int rb; //couleur du robot
    int ad;	//couleur de l'adversaire
} damier;

typedef struct elem_t {
    int i;
    int j;
    struct elem_t *suivant;
} element;

typedef struct liste_t {
    element *premier;
    element *dernier;
    int nb;
} liste;

typedef struct cpl_t { // coordonnées du pion advrese pris
    int i;
    int j;
} cpl;



/* Initialise un damier*/
void initialiser_damier(damier *my_damier, int rb, int ad);

/*Initialiser une liste vide*/
liste *initialiser_liste_vide();

/*Convertie Manoury en coordonnées*/
cpl* man_to_coord(int manoury);

/*Convertie les coordonnées en Manoury*/
int coord_to_man(cpl* coord);

/*Ajoute un element a la liste*/
void ajouter_elem(liste *ls, int i, int j);

/*Ajoute un element au début de la liste*/
void ajouter_debut(liste *ls, int i, int j);

/*Test si la case contient une dame ou un un pion et renvoie une liste chainée avec toutes les prises possibles
Cette fonction ne renvoie pas la raffle maximale  mais juste les pions qui peuvent être pris sur le moment, (ceux qui sont à proximité) */
liste *test_prise(int lg, int cl, damier *d);

/*Renvoie la valeur absolue de l'entier passe en parametre*/
int valeur_absolue(int x);

/* fonction qui teste si un pion peut être depalcé */
int test_deplacement_pion(int ligne1, int colonne1, int ligne2, int colonne2, damier *d);

/*Deplace un pion allié*/
int deplacement_pion(int ligne1, int colonne1, int ligne2, int colonne2, damier *d);

/*Deplace un pion adverse*/
int deplacement_pion_adverse(int man1, int man2, damier *d); // ADVERSAIRE

/*Effectue une prise adverse*/
int prise_adverse(int man1, int man2, damier *d); // ADVERSAIRE

/*Test si les coordonnees sont hors du damier*/
int test_sortie_cadre(int lg, int cl);

/*Copie le damier d1 dans le damier d2*/
void nouveau_damier(damier *d1, damier *d2);

/*Mets dans une liste la rafle maximum depuis la case de cordonnee lg;cl et la renvoie*/
liste *rafle_max(damier *d, int lg, int cl);

/*Effectue une prise allié et retourne le pion capturé*/
cpl *prise(int l, int c, int l2, int c2, damier *d);

/* fonction qui renvoie une liste de pions ayant une possibilité de prise sur le moment*/
liste *possibilite_prise(damier *d);

/* renvoie un couple d'indice (coordonnées du pion) ayant une rafle maximale par rapport à tous les autres pions 
l'algorithme devra recalculer cette rafle max*/
cpl *rafle_damier(damier *d);

#endif
