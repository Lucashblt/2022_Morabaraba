#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>


int jeu(SDL_Rect **Tab_Carre, int i, int j, SDL_Renderer *renderer, int *joueur, int ***Tab_j, int position[2][2]);

int deplacement_vaches(SDL_Rect **Tab_Carre, int i, int j, SDL_Renderer *renderer, int ***Tab_j, int *joueur, struct Point *Tab_Case_Possible, int *pionj, int position[2][2]);

int moulin(int **Tab_j, int i, int j);

int tuer_vache(SDL_Rect **Tab_Carre,SDL_Renderer *renderer, int **Tab_j, int i, int j, int *joueur);

int detection_vaches_hors_moulin(int **Tab_j);


