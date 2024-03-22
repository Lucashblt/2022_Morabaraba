#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include "interface.h"
#include "commandesjoueurs.h"


int main(int argc, char **argv)
{
    printf("- BIENVENUE DANS LE MORABARABA -\n\n");
    printf("- LES REGLES SONT : \n");
    printf(" [1] Veuillez placer l'un apres l'autre vos 12 vaches (pions) \n");
    printf(" [2] Si vous reussisez a aligner 3 vaches de maniere horizontale, verticale ou diagonale vous pouvez \n enlever un pion adverse \n");
    printf(" [3] Lorsque que vous aurez place vos 12 vaches chacun, vous devrez alors deplacer vos vaches sur une case a cote \n de la vache que vous avez choisi \n");
    printf(" [4] Lorsqu'un des joueurs a 3 vaches, il peut alors deplacer ses vaches n'importe ou sur une case vide \n");
    printf(" [5] Si aucun pion n'est enleve du plateau lorsqu'un des deux joueurs a moins de 3 vaches alors il y a egalite \n");
    printf(" [6] Un joueur gagne quand son adversaire n'a plus que 2 vaches \n\n");
    printf("- LES COMMANDES SONT : \n");
    printf(" [1] Pour poser une vache sur le plateau, il suffit de cliquer sur la case ou vous voulez jouer\n");
    printf(" [2] Pour enlever une vache adversaire lorsque vous venez de former un moulin, il suffit de cliquer sur \n la case ou se situe un pion adverse\n");
    printf(" [3] Pour deplacer otre vache, il suffit de cliquer sur la case ou se situe la vache que vous voulez deplacer \n et recliquer sur la cas ou vous voulez la deplacer\n");


    int choix_menu = 0; //initialise le choix du menu
    char nomJoueur[2][50];

    SDL_Window *window= NULL;
    SDL_Renderer *renderer= NULL;
    SDL_Rect **Tab_Carre;                    //on initialise un pointeur pour un tableau
    int ***Tab_j; // on initialise une structure qui contiendra les coordonnées des cases où le joueur pourra se déplacer
    int nb_pion[2], joueur, nb_pion_a_placer, moulin, tour; //on initialise les pions des 2 joueur, etc

    if (SDL_Init(SDL_INIT_VIDEO) != 0)            //on initialise la SDL pour le systeme video (affichage) +événements
        SDL_ExitWhithError("initialisation SDL"); // si cela ne s'initialise pas on revoie une erreur

    Tab_Carre = malloc(3*sizeof(SDL_Rect*)); // on initialise notre tableau à 2 dimmension ,on alloue 3 case
    for (int i=0; i<3; i++){
        Tab_Carre[i] = malloc(8*sizeof(SDL_Rect));
    }

    Tab_j = malloc(2*sizeof(int**)); // on commence par allouer 3 lignes au tableau
    for (int k=0; k<2;k++){
        Tab_j[k] = malloc(3*sizeof(int*));
        for (int i=0; i<3;i++){
            Tab_j[k][i] = malloc(8*sizeof(int));
            for (int j=0; j<8;j++){           // on initialise les tableaux a 0
                Tab_j[k][i][j] = 0;
            }
        }
    }

    printf("\n- - - - - - - MENU - - - - - - -\n\n");
    printf("   [1] COMMENCER UNE PARTIE \n   [2] REPRENDRE UNE PARTIE \n   [3] QUITTER    \n\n"); //affichage du menu
    scanf("%d",&choix_menu);  //sélection du choix désiré
    if(choix_menu == 1){
        //commencer une partie
        joueur = 0;
        nb_pion[0] = 12;
        nb_pion[1] = 12;
        nb_pion_a_placer = nb_pion[0] + nb_pion[1];
        moulin = 0;
        tour = 0;
        printf("Entrer le nom du joueur 1 : \n");
        scanf("%s", nomJoueur[0]);
        printf("Entrer le nom du joueur 2 : \n");
        scanf("%s", nomJoueur[1]);

        /*--------------------------------------------------------------*/
        // création de la fenêtre avec son nom, sa postion en x et en y ainsi que sa taille de 800 en largeur et 600 en longueur
        window = SDL_CreateWindow("Jeu du Morabaraba", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,800, 600, 0);
        if (window == NULL)
            SDL_ExitWhithError("creation de fenetre echouee"); // si la fenêtre ne se créé pas on revoie une erreur
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
         if (renderer == NULL)
            SDL_ExitWhithError("Creation rendu rechouee");     // si cela ne s'initialise pas on revoie une erreur

        tracer_plateau(nomJoueur, renderer, Tab_Carre);
        clic_souris(Tab_Carre, renderer, nomJoueur, joueur, nb_pion, nb_pion_a_placer, Tab_j, moulin, tour); // on fait appel à la fonction clic_souris
        SDL_Delay(1500);                  // laisse le fenetre SDL ouverte 5s (1000 = 1s)

        return 0;
    }
    else if(choix_menu == 2){ //reprendre une partie
        /*--------------------------------------------------------------*/
        // création de la fenêtre avec son nom, sa postion en x et en y ainsi que sa taille de 800 en largeur et 600 en longueur
        window = SDL_CreateWindow("Jeu du Morabaraba", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,800, 600, 0);
        if (window == NULL)
            SDL_ExitWhithError("creation de fenetre echouee"); // si la fenêtre ne se créé pas on revoie une erreur
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
         if (renderer == NULL)
            SDL_ExitWhithError("Creation rendu rechouee");     // si cela ne s'initialise pas on revoie une erreur

        FILE* save= fopen("save.txt", "r");
        int sauvegarde[7]= {0}; // on cree un tableau qui nous servira a chercher les valeurs dans notre fichier de sauvegarde
        if (save!= NULL){
            tracer_plateau(nomJoueur, renderer, Tab_Carre);
            fscanf(save,"%d %d %d %d %d %d %s %s", &sauvegarde[0], &sauvegarde[1],&sauvegarde[2], &sauvegarde[3],&sauvegarde[4], &sauvegarde[5], nomJoueur[0], nomJoueur[1]);
            printf("%d", sauvegarde[3]);
            joueur = sauvegarde[0];
            nb_pion[0]= sauvegarde[1];
            nb_pion[1]= sauvegarde[2];
            nb_pion_a_placer= sauvegarde[3];
            moulin= sauvegarde[4];
            tour = sauvegarde[5];
            //printf("%d %d %d %d %d %d %s %s \n", joueur, nb_pion[0], nb_pion[1], nb_pion_a_placer, moulin, tour, nomJoueur[0], nomJoueur[1]);
            for (int k =0; k<2; k++){
                for (int i =0; i<3; i++){
                    for(int j = 0; j<8; j++){
                        fscanf(save,"%d ", &Tab_j[k][i][j]);
                        if (Tab_j[k][i][j] == 1){
                            changer_couleur(renderer, k + 1);
                            if (SDL_RenderFillRect(renderer, &Tab_Carre[i][j]) !=0)
                                SDL_ExitWhithError("impossible de tracer la case en rouge");
                        }
                    }
                }
            }
            SDL_RenderPresent(renderer);      // met a jour l'écran avec tout le rendu créé depuis le précédent appel de fonction
            clic_souris(Tab_Carre, renderer, nomJoueur, joueur, nb_pion, nb_pion_a_placer, Tab_j, moulin, tour); // on fait appel à la fonction clic_souris
            SDL_Delay(1500);                  // laisse le fenetre SDL ouverte 5s (1000 = 1s)
        } else
            printf(" Vous n'avez pas de partie sauvegarde \n");
        return 0;

    }
    else if(choix_menu == 3){ //quitter
        return 0;
    }
    else
        return 0;//sécurité en cas de choix incorrect

    SDL_DestroyRenderer(renderer) ; // détruit le rendu créé pour la fenêtre
    SDL_DestroyWindow(window);      // détruit la fenêtre
    SDL_Quit();                     // quitte la SDL

}
