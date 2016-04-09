#ifndef DEF_INCLUDE
    #define DEF_INCLUDE
    #include <stdlib.h>
    #include <stdio.h>
    #include <SDL/SDL.h>
    #include <SDL/SDL_image.h>
    #include <SDL/SDL_ttf.h>
    #include <SDL/SDL_rotozoom.h>
    #include <math.h>
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

void aff_player(SDL_Surface *ecran, SDL_Surface *surface_player, _vaisseau *v_player, SDL_Surface* save_screen) {
    // Effacement de l'ancien joueur rempalce charge_niveau
    SDL_BlitSurface(save_screen, &(v_player->position), ecran, &(v_player->position));
    SDL_UpdateRect(ecran, v_player->position.x, v_player->position.y, v_player->position.w, v_player->position.h);

    // Calcul des positions
    v_player->position.x += (v_player->vitesse)*cos(RADIANATION(v_player->rotation));
    v_player->position.y += (v_player->vitesse)*(-sin(RADIANATION(v_player->rotation)));

    // Affichage du joueur
    surface_player = rotozoomSurface(surface_player, v_player->rotation, 1.0, 1);
    /* La rotation prend plus de place en fonction de l'angle, il serais donc judicieux
    de voir pour effectuer un décalage du joueur en fonction de l'angle, ou bien de lui prévoir une surface plus grande et
    de tout gérer (coordonnées, etc...) a partir de cette surface qui contiendra au final la surface de l'image.
    */
    SDL_BlitSurface(surface_player, NULL, ecran, &(v_player->position));
    SDL_UpdateRect(ecran, v_player->position.x, v_player->position.y, v_player->position.w, v_player->position.h);
}







