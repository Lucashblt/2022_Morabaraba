#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include "interface.h"

void SDL_ExitWhithError(const char *messages);

void tracer_plateau(char nomJoueur[][50], SDL_Renderer *renderer, SDL_Rect **Tab_Carre)
{
    changer_couleur(renderer, 0);
    int indice[3] = {0,0,0};

    // on calcule les cases
    for (int i=0; i<3; i++) {
        for(int j=0; j<3; j++){
            for(int k=0; k <3 ; k++){                //on rentre les coordonnées des boutons dans le tableau
                if  (i !=1 || j!=1){                  // on exclut le bouton central
                SDL_Rect carre;                       // on définit les boutons comme des carrés avec x et y les coordonnés et w et h les largeur et hauteur
                carre.x= 190 + k*75  + i*(200-k*75) ;
                carre.y= 40 + k*75 + j*(210- k*75) ;
                carre.w= 20 ;
                carre.h= 20 ;
                Tab_Carre[k][indice[k]] = carre;
                indice[k]++;
                }
            }
        }
    }

    // on place dans l'ordre les bouton pour faire en sorte que l'on commence le 0,0 en haut a gauche et
    // que l'on tourne dans le sens des aiguilles d'une montre
    for (int i = 0; i<3; i++){
        SDL_Rect carre;
        carre = Tab_Carre[i][4];
        Tab_Carre[i][4] = Tab_Carre[i][7];
        Tab_Carre[i][7] = Tab_Carre[i][1];
        Tab_Carre[i][1] = Tab_Carre[i][3];
        Tab_Carre[i][3] = Tab_Carre[i][6];
        Tab_Carre[i][6] = Tab_Carre[i][2];
        Tab_Carre[i][2] = Tab_Carre[i][5];
        Tab_Carre[i][5] = carre;
    }

    //on initalise une boucle dans laquelle on trace les 3 rectangles du plateau
    for (int i=0; i<3; i++) {
        SDL_Rect rectangle;
        rectangle.x = Tab_Carre[i][0].x +10;
        rectangle.y = Tab_Carre[i][0].y +10;
        rectangle.w = Tab_Carre[i][6].y - Tab_Carre[i][0].y -20;
        rectangle.h = Tab_Carre[i][2].x - Tab_Carre[i][0].x +20;
        if (SDL_RenderDrawRect(renderer, &rectangle) !=0)
            SDL_ExitWhithError("impossible de tracer le rectangle");

        for(int j=0; j<8; j++){
            // on trace les boutons à l'aide du tableau que nous avons rempli plus haut
            if (SDL_RenderFillRect(renderer, &Tab_Carre[i][j]) !=0)
                SDL_ExitWhithError("impossible de tracer le carre");
        }

        // on trace les traits qui relient les boutons du haut à ceux situés en haut du plus petit rectangle
        if (SDL_RenderDrawLine(renderer, (200 + i*200), 50 , (350+i*50), 200) !=0)
            SDL_ExitWhithError("impossible de tracer la ligne");
        // on trace les traits qui relient les boutons du bas a ceux situés en bas du plus petit rectangle
        if (SDL_RenderDrawLine(renderer, (200 + i*200), 470 , (350+i*50), 320) !=0)
           SDL_ExitWhithError("impossible de tracer la ligne");
    }
    // on trace le trait horizontal du milieu gauche
    if (SDL_RenderDrawLine(renderer, 200, 260, 350, 260) !=0)
        SDL_ExitWhithError("impossible de tracer la ligne");
    // on trace le trait horizontal du milieu droit
    if (SDL_RenderDrawLine(renderer, 450, 260, 600, 260) !=0)
        SDL_ExitWhithError("impossible de tracer la ligne");

    // on trace le bouton pause
    changer_couleur(renderer, 3);
    SDL_Rect bouton_pause;
    bouton_pause.x = 25;
    bouton_pause.y = 25;
    bouton_pause.w = 100;
    bouton_pause.h = 100;
    if (SDL_RenderFillRect(renderer, &bouton_pause) !=0)
        SDL_ExitWhithError("impossible de tracer le bouton_pause");

    // on trace les deux rectangle blanc du bouton pause
    changer_couleur(renderer, 0);
    for (int a =0; a<2; a++){
        SDL_Rect trait_pause;
        trait_pause.x = 47.5 + a*40;
        trait_pause.y = 35;
        trait_pause.w = 17.5;
        trait_pause.h = 75;
        if (SDL_RenderFillRect(renderer, &trait_pause) !=0)
            SDL_ExitWhithError("impossible de tracer le trait_pause");
    }
    SDL_RenderPresent(renderer);      // met a jour l'écran avec tout le rendu créé depuis le précédent appel de fonction
    return 0;
}

void clic_souris(SDL_Rect **Tab_Carre, SDL_Renderer *renderer, char nomJoueur[][50], int joueur, int nb_pion[], int nb_pion_a_placer, int ***Tab_j, int moulin, int tour)
{
    // on créé la variable qui servira a arrêter la boucle ou non, ainsi que nb qui est le nombre de pions posés pour les joueurs 1 et 2
    int continuer = 1, poservache = 0;

    // on initialise un tableau a 2 dimension qui nous servira a ne pas reformer un moulin lorsque l'on vient de le detruire
    int position[2][2] = {{-1,-1},{-1,-1}};
    int cliquer = 0, tuervache=0, deplacervache=0;

    char couleur[2][10] = {{"Rouge"},{"Bleu"}};
    SDL_Event event;                              // on initialise les événements

    if (SDL_Init(SDL_INIT_EVENTS) != 0)           // on initialise le système d'événements
        SDL_ExitWhithError("initialisation SDL");

    // on définit les positions de la souris
    SDL_Rect positionSouris;
    positionSouris.x = 0;
    positionSouris.y = 0;

    struct Point *Tab_Case_Possible; // on initialise une structure qui contiendra les coordonnées des cases où le joueur pourra se déplacer
    Tab_Case_Possible = malloc(3*sizeof(struct Point*)); // on commence par allouer 3 lignes au tableau

    if (moulin == 1)
        printf("A %s(couleur %s) d'enlever une vache adverse (suite a un moulin precedent)\n", nomJoueur[joueur], couleur[joueur]);
    else
        printf("A %s de debuter (couleur %s) \n", nomJoueur[joueur], couleur[joueur]);

    while (continuer == 1)
    {

        SDL_WaitEvent(&event); // on attend un clic de la souris
        if (nb_pion[0] == 12 && nb_pion[1] == 12 && nb_pion_a_placer == 0 && moulin == 0){
            continuer++;
            printf(" EGALITE : aucun des joueurs ne peut bouger \n");
        }

        switch(event.type)
        {
            case SDL_MOUSEBUTTONUP:            // si le bouton de la souris est relaché


            positionSouris.x = event.button.x;
            positionSouris.y = event.button.y; // on récupère les coordonnées de la souris
            cliquer = 0;

            // si on clique sur le bouton pause
            if (positionSouris.x >= 25 && positionSouris.x < 125 && positionSouris.y >= 25 && positionSouris.y < 125) { // on vérifie que le clic de souris est sur un bouton
                if (deplacervache == 0) {
                    printf("Vous avez mis en pause la partie \n");
                    //on cree un fichier ou lon detruit les valeurs qui y ete contenue
                    FILE* save=fopen("save.txt", "w+");
                    // on sauvegarde les valeurs utiles pour la reprise du jeu
                    fprintf(save,"%d %d %d %d %d %d %s %s \n", joueur, nb_pion[0], nb_pion[1], nb_pion_a_placer, moulin, tour, nomJoueur[0], nomJoueur[1]);
                    printf("La partie est sauvegardee\n");
                    for (int k = 0; k<2; k++){
                        for (int i = 0; i<3; i++){
                            for (int j = 0; j<8; j++){
                                fprintf(save,"%d ", Tab_j[k][i][j]);
                            }
                            fprintf(save ,"\n");
                        }
                        fprintf(save ,"\n");
                    }
                    fclose(save);
                    cliquer = 1 ;
                    continuer++;
                } else {
                    //on lui demande de finir l'action en cours lorsqu'il est en train de deplacer son pion
                    printf("Terminer l'action en cours \n");
                    cliquer = 1 ;
                }
            }

            for(int i=0 ; i<3; i++){
                for(int j=0 ; j<8; j++){

                    if (positionSouris.x >= Tab_Carre[i][j].x && positionSouris.x < Tab_Carre[i][j].x + Tab_Carre[i][j].w
                    && positionSouris.y >= Tab_Carre[i][j].y && positionSouris.y < Tab_Carre[i][j].y + Tab_Carre[i][j].h) { // on vérifie que le clic de souris est sur un bouton

                        if (moulin == 1) { // si il y a un moulin
                            if (joueur == 0)
                                tuervache = tuer_vache(Tab_Carre, renderer, Tab_j[1], i, j, &joueur); // si c'est au joueur 1 de jouer on fait appel a la fonction tuer vache pour qu'il puisse tuer une vache adverse
                            else
                                tuervache = tuer_vache(Tab_Carre, renderer, Tab_j[0], i, j, &joueur);
                            if (tuervache == 1){ // si la fonction tuer vache retourne 1 alors on va enlever un pion au joueur qui vient de perdre une vache
                                moulin = 0;
                                printf("A %s de jouer (couleur %s) \n", nomJoueur[joueur], couleur[joueur]);
                                nb_pion[joueur]--;
                            }else {
                                printf("%s, veuillez cliquer sur un pion adverse (hors moulin) \n", nomJoueur[joueur]);
                            }

                        } else if (deplacervache == 1){                                                // si deplacervache vaut 1 alors
                            // on regarde si le pion qui vient d'être sélectionné appartient a aucun des deux joueurs
                            if (Tab_j[0][i][j] == 0 && Tab_j[1][i][j] == 0){
                                if ((joueur == 0 && nb_pion[0] > 3) || (joueur == 1 && nb_pion[1] > 3)){      // on regarde si les joueurs ont plus de 3 pions
                                    for (int a =0 ; a<4; a++){
                                        if (Tab_Case_Possible[a].x == i && Tab_Case_Possible[a].y == j) // on regarde si le pion qu'il vient de sélectionner est un pion appartenant au tableau des cases proches au pions d'où il vient de cliquer
                                            poservache = 1;                                             // alors on place la vache
                                    }
                                }else{
                                    printf(" Vous pouvez faire volez votre vache \n");
                                    tour++;
                                    if (tour == 10){
                                        continuer++;
                                        //printf(" EGALITE : vous avez effectue 10 tour sans enlever aucun pions du plateau \n");
                                    }
                                    poservache = 1;    // si le pion qu'il vient de sélectionner n'appartient a personne et que le joueur a moins de 3 pions, alors il peut bouger n'importe où (voler)
                                }
                                if (poservache == 1){  // s'il peut poser un pion
                                    poservache = 0;    // on réinitialise poservache a 0
                                    if (position[joueur][0] != i || position[joueur][1] != j) {
                                        deplacervache = 0; // on reinitialise deplacer vache
                                        moulin = jeu(Tab_Carre, i, j, renderer, &joueur, Tab_j, position); // on fait appel a la fonction debut du jeu et on initialise moulin en fonction de ce qu'elle renvoie
                                        if (moulin == 0 && tour != 10) {                                                     // si il n'y a pas de moulin, on demande de déplacer un pion, sinon on tue une vache
                                            printf("%s : veuillez deplacer vos pions (couleur %s) \n", nomJoueur[joueur], couleur[joueur]);
                                        } else {
                                            printf("Bravo %s ! Vous avez un moulin \n", nomJoueur[joueur]);
                                            printf("A vous d'enlever une vache de votre adversaire\n");
                                        }

                                    } else {
                                       printf("Vous ne pouvez pas posez votre pion ici (vous venez de briser ce moulin au tour precedent) \n");
                                    }
                                } else
                                    printf("Selectionner une case proche \n");

                            }else
                                printf("Selectionner une case vide \n");
                        }else {
                            if (nb_pion_a_placer == 0) {     // si il n'y a plus de pions a placer

                                if (Tab_j[joueur][i][j] == 1){ // on regarde s'il y a un pion là ou il vient de cliquer pour le déplacer
                                    deplacervache = deplacement_vaches(Tab_Carre, i ,j, renderer, Tab_j, &joueur, Tab_Case_Possible, &nb_pion[joueur], position);
                                    if (deplacervache == 1){
                                        printf("veuillez redeplacer le pion sur une case proche \n");
                                    } else {
                                        printf("veuillez choisi un pion avec une case vide proche \n");
                                    }
                                }else
                                    printf("veuillez cliquer sur un de vos pions \n");
                            } else {
                                moulin = jeu(Tab_Carre, i, j, renderer, &joueur, Tab_j, position);
                                if (moulin == 0) {
                                    printf("A %s de jouer (couleur %s) \n", nomJoueur[joueur], couleur[joueur]);
                                } else {
                                    printf("Bravo %s ! Vous avez un moulin \n", nomJoueur[joueur]);
                                    printf("A vous d'enlever une vache de votre adversaire\n");
                                    tour = 0;
                                }
                                nb_pion_a_placer--;
                                if (nb_pion_a_placer == 0 && moulin == 0 && (nb_pion[0] != 12 || nb_pion[1] != 12)) // si on ne fait pas de moulin lors du dernier placement de pion
                                    printf("%s : veuillez deplacer vos pions (couleur %s) \n", nomJoueur[joueur], couleur[joueur]);
                            }
                        }
                    SDL_RenderPresent(renderer);     // met a jour l'écran avec tout le rendu créé depuis le précédent appel de fonction
                    if (nb_pion[0] == 2 || nb_pion[1] == 2) // si un des joueurs a moins de 3 pions on quitte la boucle
                        continuer++;
                    cliquer = 1 ;                    // on dit que son clic a bien fonctionné
                    if (tour == 10)
                        printf(" EGALITE : vous avez effectue 10 tour sans enlever aucun pions du plateau \n");
                    }
                }
            }
            if (cliquer == 0)                        // si son clic n'a pas fonctionné
                printf("Veuillez cliquer sur un bouton existant \n");
            break;
        }
    }

    if (nb_pion[0] < 3)
        printf(" Bravo %s : vous avez gagne\n ", nomJoueur[1]); // si le joueur1 a moins de 3 pions alors le joueur2 a gagné
    else if (nb_pion[1] < 3)
        printf(" Bravo %s : vous avez gagne\n ", nomJoueur[0]);

}

void SDL_ExitWhithError(const char *messages){                // on créé la fonction qui sera appellée en cas de problème d'initialisation
    SDL_Log("ERREUR : %s > %s \n", messages, SDL_GetError()); // on ajoute un message a notre fonction pour savoir d'où vient le problème
    SDL_Quit();                                               // on quitte la SDL
    return EXIT_FAILURE ;
}

void changer_couleur(SDL_Renderer *renderer, int couleur){
    if (couleur == 0){
        if (SDL_SetRenderDrawColor(renderer, 255,255,255, SDL_ALPHA_OPAQUE) !=0) // si couleur = 0 alors on met la couleur blanche
            SDL_ExitWhithError("impossible de changer la couleur du rendu");
    } else if (couleur == 1){
        if (SDL_SetRenderDrawColor(renderer, 255,0,0, SDL_ALPHA_OPAQUE) !=0)     // si couleur = 1 on met la couleur rouge
            SDL_ExitWhithError("impossible de changer la couleur du rendu");
    }else if (couleur == 2) {
        if (SDL_SetRenderDrawColor(renderer, 0,0,255, SDL_ALPHA_OPAQUE) !=0)     // si couleur = 2 on met la couleur bleue
            SDL_ExitWhithError("impossible de changer la couleur du rendu");
    }else if (couleur == 3) {
        if (SDL_SetRenderDrawColor(renderer, 64, 124,255, SDL_ALPHA_OPAQUE) !=0)     // si couleur = 2 on met la couleur bleue clair
            SDL_ExitWhithError("impossible de changer la couleur du rendu");
    }
}
