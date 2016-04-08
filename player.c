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

void vitesse_player(_vaisseau *v_player, int sens) {
    if (sens == AVANT)
        v_player->vitesse += v_player->acceleration;
    if (sens == ARRIERE)
        v_player->vitesse -= v_player->acceleration;
}

void aff_player(SDL_Surface *ecran, SDL_Surface *surface_player, _vaisseau *v_player) {
    // Calcul des positions
    v_player->position.x += (v_player->vitesse)*cos(RADIANATION(v_player->rotation));
    v_player->position.y += (v_player->vitesse)*(-sin(RADIANATION(v_player->rotation)));

    // Affichage du joueur
    charge_niveau(ecran);
    surface_player = rotozoomSurface(surface_player, v_player->rotation, 1.0, 1);
    SDL_BlitSurface(surface_player, NULL, ecran, &(v_player->position));
    SDL_Flip(ecran);
}








