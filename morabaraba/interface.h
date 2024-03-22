#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>

struct Point {
    int x;
    int y;
};

void tracer_plateau(char nomJoueur[][50], SDL_Renderer *renderer, SDL_Rect **Tab_Carre);

void clic_souris(SDL_Rect **Tab_Carre, SDL_Renderer *renderer, char nomJoueur[][50], int joueur, int nb_pion[], int nb_pion_a_placer, int ***Tab_j, int moulin, int tour);

void changer_couleur(SDL_Renderer *renderer, int couleur);

void SDL_ExitWhithError(const char *messages);
