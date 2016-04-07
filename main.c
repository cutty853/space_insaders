#ifndef DEF_INCLUDE
    #define DEF_INCLUDE
    #include <stdlib.h>
    #include <stdio.h>
    #include <SDL/SDL.h>
    #include <SDL/SDL_image.h>
    #include <SDL/SDL_ttf.h>
    #include "structure.h"
    #include "utilitaire.h"
    #include "jeu.h"
    #include "ia.h"
    #include "player.h"


    #include "constantes.h"
#endif


int main ( int argc, char** argv )
{
    SDL_Surface *ecran=NULL;
    int action;

    if (SDL_Init(SDL_INIT_VIDEO) == -1) {
        fprintf(stderr, "Erreur lors du chargement de la SDL %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    if(TTF_Init() == -1) {
        fprintf(stderr, "Erreur d'initialisation de TTF_Init : %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }
    ecran = SDL_SetVideoMode(0, 0, 32, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN);
    if (ecran==NULL) {
        fprintf(stderr, "Erreur lors du chargement du mode video %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    action = menu(ecran);
    switch (action) {
        case -1:
            exit(10000);
            break;
        case JOUER:
            charge_niveau(ecran);
            check_player_action(ecran);
            break;
        case SAUVEGARDER:
            break;
        case CHARGER:
            break;
        case QUITTER:
            break;
        default:
            break;
    }

    TTF_Quit();
    SDL_Quit();
    return 0;
}
