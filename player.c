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

void vitesse_joueur(_vaisseau *v_joueur, int sens) {
    if (sens == AVANT) {
        if (v_joueur->vitesse == 0) {
            v_joueur->vitesse = VITESSE_MIN_JOUEUR;
        } else {
            if ((v_joueur->vitesse + (v_joueur->acceleration/100.0)) < v_joueur->vitesse_max)
                v_joueur->vitesse += (v_joueur->acceleration/100.0);
        }
        if (v_joueur->vitesse < VITESSE_MIN_JOUEUR && v_joueur->vitesse > -VITESSE_MIN_JOUEUR) {
            v_joueur->vitesse = 0;
        }
    }
    if (sens == ARRIERE) {
        if (v_joueur->vitesse == 0) {
            v_joueur->vitesse = -VITESSE_MIN_JOUEUR;
        } else {
            if ((v_joueur->vitesse - (v_joueur->acceleration/100.0)) > -v_joueur->vitesse_max)
                v_joueur->vitesse -= (v_joueur->acceleration/100.0);
        }
        if (v_joueur->vitesse < VITESSE_MIN_JOUEUR && v_joueur->vitesse > -VITESSE_MIN_JOUEUR) {
            v_joueur->vitesse = 0;
        }
    }
}

void charge_sprite_explosion (_explosion *boom) {
    SDL_Surface *tmp_boom;
    SDL_Rect case_courante;
    int i;

    tmp_boom = IMG_Load("images/explosion.png");
    test_surface(tmp_boom, 150);
    init_pos (&case_courante, 0, 0);
    case_courante.w = 65;
    case_courante.h = 65;
    for (i=0 ; i<NB_SPRITES_EXPLOSION ; i++) {
        if ((i%5 == 0) && i!=0) {
            case_courante.y += 65;
            case_courante.x = 0;
        } else {
            case_courante.x += 65;
        }
        boom->sprite[i] = IMG_Load("images/transparent_explosion.png");
        test_surface(boom->sprite[i], 104+i);
        SDL_BlitSurface(tmp_boom, &case_courante, boom->sprite[i], NULL);
        SDL_SetAlpha(boom->sprite[i], SDL_SRCALPHA, 255);
    }
}

SDL_Rect* explosion_joueur (SDL_Surface *ecran, SDL_Surface *save_screen, _explosion *boom, _vaisseau *v_joueur) {
    SDL_Rect *pos_to_update;

    pos_to_update = malloc(sizeof(SDL_Rect)*2);
    init_pos(&(boom->position), v_joueur->position.x, v_joueur->position.y);
    SDL_BlitSurface(save_screen, &(v_joueur->position), ecran, &(v_joueur->position));
    SDL_BlitSurface(boom->sprite[boom->phase], NULL, ecran, &(boom->position));
    pos_to_update[0] = v_joueur->position;
    pos_to_update[1] = boom->position;

    return pos_to_update; // Penser a bien free avant la fin du jeu
}
