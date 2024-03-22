#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include "interface.h"
#include "commandesjoueurs.h"

int jeu(SDL_Rect **Tab_Carre, int i, int j, SDL_Renderer *renderer, int *joueur, int ***Tab_j, int position[2][2]){

    SDL_Event event;

    if (SDL_Init(SDL_INIT_EVENTS) != 0)           // on initialise le système d'événements
        SDL_ExitWhithError("initialisation SDL");

    if (Tab_j[0][i][j] == 1 || Tab_j[1][i][j] == 1) { // s'il y a déjà un pion sur cette case on renvoit 0
        return 0;
    }
    changer_couleur(renderer, *joueur + 1);                         // on change la couleur en fonction du joueur
    if (SDL_RenderFillRect(renderer, &Tab_Carre[i][j]) !=0)         // on trace le pion
        SDL_ExitWhithError("impossible de tracer le pion");
    Tab_j[*joueur][i][j] = 1;                                      // on remplit le tableau du joueur
    if (moulin(Tab_j[*joueur], i, j) == 1)                         // on fait appel à la fonction moulin et s'il y en a un on renvoie 1
        return 1;
    position[*joueur][0]=-1;                                        // on reinitialise la fonction postition joueur en x et y en fonction du joueur
    position[*joueur][1]=-1;
    if (*joueur == 0)
        *joueur = 1;          // on change de joueur
    else
        *joueur = 0;
    return 0;
}


int moulin(int **Tab_j, int i, int j){
    // on vérifie si un moulin est présent
    int nb=0;
    // on vérifie en diagonales et colonne
    for( int a =0; a<3; a++){
        nb += Tab_j[a][j]; // s'il vaut 1 alors il appartient au tableau
    }
    if (nb == 3)
        return 1;
    // on vérifie en lignes
    nb=0;
    if ((j%2) == 0 ) {
        // si j est pair (aux sommets des rectangles)
        if (j==6) {
            if (Tab_j[i][7] == 1 && Tab_j[i][0] == 1)
                return 1;
        } else {
            if (Tab_j[i][j+1] == 1 && Tab_j[i][j+2] == 1)
                return 1;
        }
        if (j==0) {
            if (Tab_j[i][7] == 1 && Tab_j[i][6] == 1)
                return 1;
        } else {
            if (Tab_j[i][j-1] == 1 && Tab_j[i][j-2] == 1)
                return 1;
        }
    }else{
        // si j est impair (aux milieux des arêtes)
        if (j==7){
            if (Tab_j[i][0] == 1 && Tab_j[i][j-1] == 1)
                return 1;
        }else
            if (Tab_j[i][j+1] == 1 && Tab_j[i][j-1] == 1)
                return 1;
    }
    return 0;
}

int tuer_vache(SDL_Rect **Tab_Carre,SDL_Renderer *renderer, int **Tab_j, int i, int j, int *joueur){

    // s'il y a un pion appartenant au joueur adverse et que l'autre a fait un moulin et qu'il n'y a pas de vaches en dehors du moulin:
    if ( (Tab_j[i][j] == 1 && moulin(Tab_j, i, j) == 0) || (Tab_j[i][j] == 1 && detection_vaches_hors_moulin(Tab_j) == 0)){
        Tab_j[i][j] = 0;      // on remet a 0 le bouton où était le pion
        changer_couleur(renderer, 0); // on retrace le bouton en blanc

        if (SDL_RenderFillRect(renderer, &Tab_Carre[i][j]) !=0)
                SDL_ExitWhithError("impossible de tracer la case en blanc");
        if (*joueur == 0)
            *joueur = 1;          // on change de joueur
        else
            *joueur = 0;
        return 1;
    }
    return 0;
}

int detection_vaches_hors_moulin(int **Tab_j){
    // on regarde si tous les vaches du joueurs sont dans des moulins ou non
    for (int i = 0; i<3; i++){
        for (int j = 0; j<8 ; j++){
            if ( Tab_j[i][j] == 1){
                // on fait appel à la fonction moulin et si il n'y a pas de moulin on lui dit qu'il y a des vaches en dehors des moulins
                if ( moulin(Tab_j, i, j) == 0)
                    return 1;
            }
        }
    }
    // si toutes les vaches sont dans des moulins on renvoie 0
    return 0;
}

int deplacement_vaches(SDL_Rect **Tab_Carre, int i, int j, SDL_Renderer *renderer, int ***Tab_j, int *joueur, struct Point *Tab_Case_Possible, int *pionj, int position[2][2]){


    int deplacement_possible = 0;

    if (pionj < 3){
        deplacement_possible = 1;
    }else {
        // place dans un tableau toutes les cases cote a cote de la ou vient de cliquer le joueur
        if ( i == 0 && j == 0){
            Tab_Case_Possible[0].x = 0;
            Tab_Case_Possible[0].y = 7;
            Tab_Case_Possible[1].x = 0;
            Tab_Case_Possible[1].y = 1;
            Tab_Case_Possible[2].x = 1;
            Tab_Case_Possible[2].y = 0;
            Tab_Case_Possible[3].x = -1;
            Tab_Case_Possible[3].y = -1;
        } else if ( i == 0 && j == 7) {
            Tab_Case_Possible[0].x = 0;
            Tab_Case_Possible[0].y = 6;
            Tab_Case_Possible[1].x = 0;
            Tab_Case_Possible[1].y = 0;
            Tab_Case_Possible[2].x = 1;
            Tab_Case_Possible[2].y = 7;
            Tab_Case_Possible[3].x = -1;
            Tab_Case_Possible[3].y = -1;
        } else if ( i == 1 && j == 0){
            Tab_Case_Possible[0].x = 1;
            Tab_Case_Possible[0].y = 7;
            Tab_Case_Possible[1].x = 1;
            Tab_Case_Possible[1].y = 1;
            Tab_Case_Possible[2].x = 2;
            Tab_Case_Possible[2].y = 0;
            Tab_Case_Possible[3].x = 0;
            Tab_Case_Possible[3].y = 0;
        } else if ( i == 1 && j == 7) {
            Tab_Case_Possible[0].x = 1;
            Tab_Case_Possible[0].y = 6;
            Tab_Case_Possible[1].x = 1;
            Tab_Case_Possible[1].y = 0;
            Tab_Case_Possible[2].x = 2;
            Tab_Case_Possible[2].y = 7;
            Tab_Case_Possible[3].x = 0;
            Tab_Case_Possible[3].y = 7;
        } else if ( i == 2 && j == 0){
            Tab_Case_Possible[0].x = 2;
            Tab_Case_Possible[0].y = 7;
            Tab_Case_Possible[1].x = 2;
            Tab_Case_Possible[1].y = 1;
            Tab_Case_Possible[2].x = 1;
            Tab_Case_Possible[2].y = 0;
            Tab_Case_Possible[3].x = -1;
            Tab_Case_Possible[3].y = -1;
        } else if ( i == 2 && j == 7) {
            Tab_Case_Possible[0].x = 2;
            Tab_Case_Possible[0].y = 6;
            Tab_Case_Possible[1].x = 2;
            Tab_Case_Possible[1].y = 0;
            Tab_Case_Possible[2].x = 1;
            Tab_Case_Possible[2].y = 7;
            Tab_Case_Possible[3].x = -1;
            Tab_Case_Possible[3].y = -1;
        } else if ( i == 0) {
            Tab_Case_Possible[0].x = 0;
            Tab_Case_Possible[0].y = j+1;
            Tab_Case_Possible[1].x = 0;
            Tab_Case_Possible[1].y = j-1;
            Tab_Case_Possible[2].x = 1;
            Tab_Case_Possible[2].y = j;
            Tab_Case_Possible[3].x = -1;
            Tab_Case_Possible[3].y = -1;
        } else if ( i == 1) {
            Tab_Case_Possible[0].x = 1;
            Tab_Case_Possible[0].y = j+1;
            Tab_Case_Possible[1].x = 1;
            Tab_Case_Possible[1].y = j-1;
            Tab_Case_Possible[2].x = 2;
            Tab_Case_Possible[2].y = j;
            Tab_Case_Possible[3].x = 0;
            Tab_Case_Possible[3].y = j;
        } else if ( i == 2) {
            Tab_Case_Possible[0].x = 2;
            Tab_Case_Possible[0].y = j+1;
            Tab_Case_Possible[1].x = 2;
            Tab_Case_Possible[1].y = j-1;
            Tab_Case_Possible[2].x = 1;
            Tab_Case_Possible[2].y = j;
            Tab_Case_Possible[3].x = -1;
            Tab_Case_Possible[3].y = -1;
        }
    }
    // on regarde si ces cases sont déjà occupées ou non
    for (int k=0;k<4;k++) {
        if (Tab_Case_Possible[k].x >= 0) {
            if (Tab_j[0][Tab_Case_Possible[k].x][Tab_Case_Possible[k].y] == 0 &&
                 Tab_j[1][Tab_Case_Possible[k].x][Tab_Case_Possible[k].y] == 0)
                deplacement_possible = 1; // s'il y a au moins une case libre deplacement prend la valeur 1
        }
    }
    // si on peut deplacer une vahce on redessine la case en blanc
    if (deplacement_possible == 1) {
        changer_couleur(renderer, 0); // si deplacement = 1 on met la couleur blanche
        // si il retire une vache d'un moulin on enregistre les coordonnées du point
        if (moulin(Tab_j[*joueur], i, j) == 1 && position[*joueur][0] == -1) {
            position[*joueur][0] = i;
            position[*joueur][1] = j;
        }

        int moulin = 0;
        // on fait appel a la fonction debut du jeu et on initialise moulin en fonction de ce qu'elle renvoie
        moulin = jeu(Tab_Carre, i, j, renderer, &joueur, Tab_j, position);
        Tab_j[*joueur][i][j] = 0; // on réinitialise la case que l'on vient de cliquer dans le tableau du joueur
        if (SDL_RenderFillRect(renderer, &Tab_Carre[i][j]) !=0) // on trace en blanc la case
            SDL_ExitWhithError("impossible de tracer la case en blanc");
        return 1;
    }
    return 0;
}

/*
void PrintTableau(int **tabj, int k){
    printf("Tableau%d : \n", k);
    for (int i=0;i<3;i++){
        for (int j=0;j<8;j++){
            printf("%d ", tabj[i][j]);
        }
        printf("\n");
    }
}

void PrintTableauCase(SDL_Rect **tab){
    printf("Tableau des cases : \n");
    for (int i=0;i<3;i++){
        for (int j=0;j<8;j++){
            printf("%d ", tab[i][j].x);
        }
        printf("\n");
    }
}

*/
