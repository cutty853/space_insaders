#ifndef DEF_INCLUDE
    #define DEF_INCLUDE
    #include <stdlib.h>
    #include <stdio.h>
    #include <SDL/SDL.h>
    #include "structure.h"
    #include "utilitaire.h"
#endif

int main ( int argc, char** argv )
{
    SDL_Surface *ecran=NULL, *rectangle=NULL;
    SDL_Rect position, pos_degrade;
    _degrade deg;
    deg.nuance=128;
    deg.taille_x=128;
    deg.taille_y=128;
    deg.sens=VERTICAL;
    position.x = (512 / 2) - (128 / 2);
    position.y = (512 / 2) - (128 / 2);
    pos_degrade.x=0;
    pos_degrade.y=0;

    if (SDL_Init(SDL_INIT_VIDEO) == -1) {
        fprintf(stderr, "Erreur lors du chargement de la SDL %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    ecran = SDL_SetVideoMode(512, 512, 32, SDL_HWSURFACE);
    if (ecran==NULL) {
        fprintf(stderr, "Erreur lors du chargement du mode video %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 255, 35, 80));
    rectangle = SDL_CreateRGBSurface(SDL_HWSURFACE, 70, 70, 32, 0, 0, 0, 0);
    SDL_FillRect(rectangle, NULL, SDL_MapRGB(rectangle->format, 35, 255, 0));
    SDL_BlitSurface(rectangle, NULL, ecran, &position);
    SDL_Flip(ecran);
    degrade(deg, ecran, pos_degrade);


    SDL_WM_SetCaption("Mon magnifique titre!", NULL);
    pause();

    SDL_Quit();
    return 0;
}
